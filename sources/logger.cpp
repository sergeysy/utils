#include <stdarg.h>
#include <list>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <thread>
#include <functional>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/filesystem.hpp>

#include <logger.hpp>
#include <CPEnum.h>
#include <CPConverter.h>
#include <FormatHelper.h>
//#include <Windows.h>


/*
#include <chrono>
#include <thread>
#include <iomanip>

#include "logger.hpp"
*/
std::ostream& operator<<(std::ostream& os, const logger& )
{
    using namespace std::chrono;
    high_resolution_clock::time_point p = high_resolution_clock::now();
    milliseconds ms = duration_cast<milliseconds>(p.time_since_epoch());

    seconds s = duration_cast<seconds>(ms);
    std::time_t in_time_t = s.count();
    std::size_t fractional_seconds = ms.count() % 1000;


    //auto now = std::chrono::system_clock::now();
    //auto in_time_t = std::chrono::system_clock::to_time_t(now);

    os << std::put_time(std::localtime(&in_time_t), "%X")<< "." << std::setw(3) << std::setfill('0') << fractional_seconds << " [" << std::this_thread::get_id() << "] ";
    return os;
}


namespace Utils
{

    unsigned int const MAX_LOGGER_QUEUE_SIZE            = 10000;
    unsigned int const MAX_LOGGER_SLEEP_TIMEOUT_MS      = 100;
    unsigned int const MAX_LOGGER_SLEEP_TRY_COUNTS      = 3;

    class LoggerImpl : public ILoggerImpl
    {
        struct DATE
        {
            uint16_t year;
            uint16_t month;
            uint16_t day;

            bool operator==(DATE const & obj) const;
            bool operator!=(DATE const & obj) const;
        };

    public:
        LoggerImpl()
            : m_finished(false)
            , m_importanceLevel(Importance_Normal)
            , m_dirPath(boost::filesystem::current_path() / L"logs")
			, m_currentDate({})
            //, m_currentHandle(INVALID_HANDLE_VALUE)
        {
            Start();
        }

        virtual ~LoggerImpl()
        {
            Stop();
        }

        void Start() 
        {
            m_workThread    = boost::thread(&LoggerImpl::ThreadProc, this);
            m_finished      = false;
        }

        void Stop() 
        {
            m_finished = true;
            m_workThreadCondition.notify_one();

            if (!m_workThread.timed_join(boost::posix_time::seconds(2))) {
                m_workThread.interrupt();
                /*if (!m_workThread.timed_join(boost::posix_time::milliseconds(100))) {
                    ::TerminateThread(m_workThread.native_handle(), 0);
                }*/
            }

            if (m_fstream.is_open())
            {
                m_fstream.close();
            }
        }

        virtual void Restart() {
            Stop();
            Start();
        }

        virtual void SetDirectory(std::wstring const &dir)
        {
            m_dirPath = dir;
        }

        virtual void SetImportance(Importance const importance_level)
        {
            m_importanceLevel = importance_level;
        }

        virtual void Log(Severity const severity, std::wstring const &msg)
        {
#ifndef _DEBUG
            if(severity == Severity_Debug)
                return;
#endif
            Importance const importance = severity == Severity_Error ? Importance_High : Importance_Normal;
            if (importance < m_importanceLevel)
                return;

            Log(importance, severity, msg);
        }

        virtual void LogF(Severity const severity, wchar_t const *format, va_list &args)
        {
#ifndef _DEBUG
            if(severity == Severity_Debug)
                return;
#endif
            Importance const importance = severity == Severity_Error ? Importance_High : Importance_Normal;
            if (importance < m_importanceLevel)
                return;

            std::wstring msg = FormatHelper::ReadArgs(format, args);

            Log(importance, severity, msg);
        }

        virtual void Log(Importance const importance, Severity const severity, std::wstring const &msg)
        {
#ifndef _DEBUG
            if(severity == Severity_Debug)
                return;
#endif
            if (importance < m_importanceLevel)
                return;

            /*SYSTEMTIME systemTime = { 0 };
            ::GetSystemTime(&systemTime);*/
            //uint32_t const process_id = ::GetCurrentProcessId();
            //size_t const thread_id = std::hash<std::thread::id>(std::this_thread::get_id());//::GetCurrentThreadId();
            std::stringstream osId;
            osId<<std::this_thread::get_id();

            wchar_t type[2] = { L'\0', L'\0' };
            type[0] =
                severity == Severity_Error   ? L'E' :
                severity == Severity_Warning ? L'W' :
                severity == Severity_Info    ? L'-' :
                severity == Severity_Debug   ? L'D' :
                L'?';

            /**/
            using namespace std::chrono;
            high_resolution_clock::time_point p = high_resolution_clock::now();
            milliseconds ms = duration_cast<milliseconds>(p.time_since_epoch());

            seconds s = duration_cast<seconds>(ms);
            std::time_t in_time_t = s.count();
            std::size_t fractional_seconds = ms.count() % 1000;

            std::stringstream os;
            os << std::put_time(std::localtime(&in_time_t), "%X")<< "." << std::setw(3) << std::setfill('0') << fractional_seconds << " [" << std::this_thread::get_id() << "] ";
            /**/
            std::wstring const logMsg = FormatHelper::Read(L"[%s] [T:%s] [%s] %s\r\n",
                os.str(),
                /*process_id,*/ osId.str(), type, msg.c_str());

            DATE date;
            const auto tt = *localtime(&in_time_t);
            date.day   = tt.tm_mday;
            date.month = tt.tm_mon;
            date.year  = tt.tm_year+1900;
            PushMessage(date, logMsg);
        }

        virtual void LogF(Importance const importance, Severity const severity, wchar_t const *format, va_list &args)
        {
#ifndef _DEBUG
            if(severity == Severity_Debug)
                return;
#endif
            if (importance < m_importanceLevel)
                return;

            std::wstring const msg = FormatHelper::ReadArgs(format, args);

            Log(importance, severity, msg);
        }

    private:
        void ThreadProc()
        {
            try
            {
                int const MaxFinishTryCount = 10;   // максимальное количество попыток выложить имеющуюся очередь сообщений в лог-файл
                int finishTryCount = 0;             // счетчик попыток выложить имеющуюся очередь сообщений в лог-файл
                bool running = true;
                while (running)                     // цикл потока, обрабатываем запросы очереди пока не будет сигнала о прекращении
                {
                    boost::recursive_mutex::scoped_lock queue_lock(m_msgQueueMutex);
                    bool const is_empty = m_msgQueue.empty();
                    queue_lock.unlock();    // снимаем блокировку

                    //сомнения что это нужно
                    //if (boost::this_thread::interruption_requested())
                    //    break;

                    if (m_finished)
                        running = !is_empty ? (++finishTryCount < MaxFinishTryCount) : false;

                    if (running)
                    {
                        if (is_empty)
                        {
                            boost::mutex::scoped_lock lock(m_workThreadMutex);
                            m_workThreadCondition.timed_wait(lock, boost::posix_time::seconds(3));    // ждем сигнала для продолжения
                        }
                        else
                        {
                            FlushMessagesToLogFile();       // запускаем цикл сброса сообщений в файл
                        }
                    }
                }
            }
            catch(...)
            { }

            // закрываем открытый файл
            if (m_fstream.is_open())
            {
                m_fstream.close();
            }
        }

        void FlushMessagesToLogFile()
        {
            std::pair<DATE, std::wstring> logMsgPair;

            // копируем первое сообщение в очереди
            if (!GetFirstMessage(&logMsgPair))
                return;

            // запоминаем текущую дату сообщений, чтобы отслеживать сообщения именно для этого файла
            DATE date = logMsgPair.first;

            // открываем или создаем файл
            CreateOrOpenLogFile(date);
            if (!m_fstream.is_open())
                return;

            // записываем BOM в начало файла, если нужно
            WriteBomIfNeed();

            // ---
            // процедура записи в файл лога
            //bool next = false;      // условие продолжения цикла
            do                      // цикл записи сообщений в лог-файл
            {
                // записываем сообщение в файл
                if (!WriteMessage(logMsgPair.second))
                    break;

                // удаляем первый элемент
                PopFirstMessage();

                // копируем первое сообщение в очереди
                if (!GetFirstMessage(&logMsgPair))
                    break;

                // если дата сообщения совподает, то продолжаем. иначе - заканчиваем
                //next = logMsgPair.first == date;

                //все логи должны записаться
                //if (boost::this_thread::interruption_requested())
                //    break;

			} while (logMsgPair.first == date);
        }

        void PushMessage(DATE const &date, std::wstring const &message)
        {
            if (m_finished)
                return;

            bool is_pushed = false;
            for (size_t i = 0; i < MAX_LOGGER_SLEEP_TRY_COUNTS; ++i)
            {
                boost::recursive_mutex::scoped_lock queue_lock(m_msgQueueMutex);
                if (m_msgQueue.size() <= MAX_LOGGER_QUEUE_SIZE)
                {
                    m_msgQueue.push_back(std::pair<DATE, std::wstring>(date, message));
                    is_pushed = true;
                    break;
                }

                queue_lock.unlock();
                //::Sleep(MAX_LOGGER_SLEEP_TIMEOUT_MS);
                std::this_thread::sleep_for( std::chrono::milliseconds(MAX_LOGGER_SLEEP_TIMEOUT_MS));
            }

            if (is_pushed)
            {
                m_workThreadCondition.notify_one();
            }
        }

        bool GetFirstMessage(std::pair<DATE, std::wstring> *message) const
        {
            if (message == NULL)
                return false;

            boost::recursive_mutex::scoped_lock queue_lock(m_msgQueueMutex);
            if (m_msgQueue.empty())
                return false;

            *message = m_msgQueue.front();
            queue_lock.unlock();
            return true;
        }

        void PopFirstMessage()
        {
            boost::recursive_mutex::scoped_lock queue_lock(m_msgQueueMutex);

            if (!m_msgQueue.empty())
                m_msgQueue.pop_front();
        }

        void WriteBomIfNeed() const
        {
            /*LARGE_INTEGER fileSize = {};
            if (::GetFileSizeEx(hfile, &fileSize))                              // получаем размер файла
            {
                if (fileSize.QuadPart == 0)                                     // если он пустой
                {
                    uint32_t num = 0;
                    const unsigned char BomUtf8[3] = { 0xEF, 0xBB, 0xBF };      // определяем BOM(Byte Order Mark) для файлов в UTF-8 кодировке
                    ::WriteFile(hfile, BomUtf8, 3, &num, NULL);                 // записываем BOM UTF-8 в начало файла
                }
            }*/
            const auto fileSize = m_fstream.tellg();
            {
                if(0 == fileSize)
                {
                    const std::vector<unsigned char> BomUtf8 = { 0xEF, 0xBB, 0xBF };
                    //fstream << BomUtf8;
                     std::copy(BomUtf8.begin(), BomUtf8.end(), std::ostream_iterator<char>(m_fstream, " "));
                }
            }
        }

        bool WriteMessage(std::wstring const &message_text) const
        {
            // представляем сообщение в кодировке UTF-8
            std::string message_text_utf8 = CodePage::Wc2Mb(message_text, CodePage_Utf8);

            /*
            // индикатор записи в конец файла
            OVERLAPPED ov = {};
            ov.Offset = uint32_t(-1);
            ov.OffsetHigh = uint32_t(-1);

            uint32_t err = ERROR_SUCCESS;
            char *buff = &message_text_utf8.front();
            size_t buff_size = message_text_utf8.size();
            do  // то записываем сообщение в конец файла пока не запишется целиком
            {
                uint32_t num = 0;
                err = ::WriteFile(hfile, buff, buff_size, &num, &ov) ? ERROR_SUCCESS : ::GetLastError();
                if (err != ERROR_SUCCESS)
                    break;

				buff += static_cast<ptrdiff_t>(num);
                buff_size -= (size_t)num;
            } while (buff_size > 0);
            */
            m_fstream << message_text_utf8;
            std::wcerr<<L"message_text_utf8(length"<<message_text_utf8.length()<<"):"<<message_text<<std::endl;
            m_fstream.flush();
            return m_fstream?true:false;
        }

        void CreateOrOpenLogFile(DATE const &date) const
        {
            if (!m_fstream.is_open() || date != m_currentDate)
            {
                if (m_fstream.is_open())
                {
                    m_fstream.close();
                }
				
				if (!boost::filesystem::exists(m_dirPath))
				{
					boost::filesystem::create_directories(m_dirPath);
				}

				std::wstring log_file_name = FormatHelper::Read(L"%04d-%02d-%02d.log", date.year, date.month, date.day);
                boost::filesystem::path log_file_path = m_dirPath / log_file_name;

                m_fstream.open(log_file_path.string().c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
                m_currentDate = date;
            }

            //return std::move(m_fstream);
        }

    private:
        mutable bool                m_finished;
        Importance                  m_importanceLevel;
        boost::filesystem::path     m_dirPath;
        mutable DATE                m_currentDate;
        //mutable HANDLE              m_currentHandle;
        mutable std::fstream        m_fstream;

        mutable boost::thread       m_workThread;
        mutable boost::mutex        m_workThreadMutex;
        mutable boost::condition    m_workThreadCondition;

        std::list< std::pair<DATE, std::wstring> > m_msgQueue;
        mutable boost::recursive_mutex  m_msgQueueMutex;
    };

    bool LoggerImpl::DATE::operator==(DATE const &obj) const
    {
        return (day == obj.day) && (month == obj.month) && (year == obj.year);
    }

    bool LoggerImpl::DATE::operator!=(DATE const &obj) const
    {
        return !operator==(obj);
    }

    Logger::Logger() : m_impl(new LoggerImpl)
    {}

    void Logger::SetDirectory(std::wstring const &dir)
    {
        m_impl->SetDirectory(dir);
    }

    void Logger::SetImportance(Importance const importance_level)
    {
        m_impl->SetImportance(importance_level);
    }

    void Logger::Log(Severity const severity, std::wstring const &msg)
    {
        if (Logger::HasInstance())
            Logger::Instance()->m_impl->Log(severity, msg);
    }

    void Logger::LogF(Severity const severity, wchar_t const *format, ...)
    {
        if (Logger::HasInstance())
        {
            va_list args;
            va_start(args, format);
            Logger::Instance()->m_impl->LogF(severity, format, args);
            va_end(args);
        }
    }

    void Logger::Log(Importance const importance, Severity const severity, std::wstring const &msg)
    {
        if (Logger::HasInstance())
            Logger::Instance()->m_impl->Log(importance, severity, msg);
    }

    void Logger::LogF(Importance const importance, Severity const severity, wchar_t const *format, ...)
    {
        if (Logger::HasInstance())
        {
            va_list args;
            va_start(args, format);
            Logger::Instance()->m_impl->LogF(importance, severity, format, args);
            va_end(args);
        }
    }

    void Logger::Restart() {
        Logger::Instance()->m_impl->Restart();
    }

} // namespace Utils
