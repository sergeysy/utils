#include <stdarg.h>
#include <list>
#include <string>
#include <vector>

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

*/
namespace Utils
{

    unsigned int const MAX_LOGGER_QUEUE_SIZE            = 10000;
    unsigned int const MAX_LOGGER_SLEEP_TIMEOUT_MS      = 100;
    unsigned int const MAX_LOGGER_SLEEP_TRY_COUNTS      = 3;

    class LoggerImpl : public ILoggerImpl
    {
        struct DATE
        {
            WORD year;
            WORD month;
            WORD day;

            bool operator==(DATE const & obj) const;
            bool operator!=(DATE const & obj) const;
        };

    public:
        LoggerImpl()
            : m_finished(false)
            , m_importanceLevel(Importance_Normal)
            , m_dirPath(boost::filesystem::current_path() / L"logs")
			, m_currentDate({})
            , m_currentHandle(INVALID_HANDLE_VALUE)
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
                if (!m_workThread.timed_join(boost::posix_time::milliseconds(100))) {
                    ::TerminateThread(m_workThread.native_handle(), 0);
                }
            }

            if (m_currentHandle != INVALID_HANDLE_VALUE) {
                ::CloseHandle(m_currentHandle);
                m_currentHandle = INVALID_HANDLE_VALUE;
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

            SYSTEMTIME systemTime = { 0 };
            ::GetSystemTime(&systemTime);
            DWORD const process_id = ::GetCurrentProcessId();
            DWORD const thread_id = ::GetCurrentThreadId();

            wchar_t type[2] = { L'\0', L'\0' };
            type[0] =
                severity == Severity_Error   ? L'E' :
                severity == Severity_Warning ? L'W' :
                severity == Severity_Info    ? L'-' :
                severity == Severity_Debug   ? L'D' :
                L'?';

            std::wstring const logMsg = FormatHelper::Read(L"[%02d:%02d:%02d:%03d] [P:%d|T:%d] [%s] %s\r\n",
                systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds,
                process_id, thread_id, type, msg.c_str());

            DATE date;
            date.day   = systemTime.wDay;
            date.month = systemTime.wMonth;
            date.year  = systemTime.wYear;
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
                int const MaxFinishTryCount = 10;   // ������������ ���������� ������� �������� ��������� ������� ��������� � ���-����
                int finishTryCount = 0;             // ������� ������� �������� ��������� ������� ��������� � ���-����
                bool running = true;
                while (running)                     // ���� ������, ������������ ������� ������� ���� �� ����� ������� � �����������
                {
                    boost::recursive_mutex::scoped_lock queue_lock(m_msgQueueMutex);
                    bool const is_empty = m_msgQueue.empty();
                    queue_lock.unlock();    // ������� ����������

                    //�������� ��� ��� �����
                    //if (boost::this_thread::interruption_requested())
                    //    break;

                    if (m_finished)
                        running = !is_empty ? (++finishTryCount < MaxFinishTryCount) : false;

                    if (running)
                    {
                        if (is_empty)
                        {
                            boost::mutex::scoped_lock lock(m_workThreadMutex);
                            m_workThreadCondition.timed_wait(lock, boost::posix_time::seconds(3));    // ���� ������� ��� �����������
                        }
                        else
                        {
                            FlushMessagesToLogFile();       // ��������� ���� ������ ��������� � ����
                        }
                    }
                }
            }
            catch(...)
            { }

            // ��������� �������� ����
            if (m_currentHandle != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(m_currentHandle);
                m_currentHandle = INVALID_HANDLE_VALUE;
            }
        }

        void FlushMessagesToLogFile()
        {
            std::pair<DATE, std::wstring> logMsgPair;

            // �������� ������ ��������� � �������
            if (!GetFirstMessage(&logMsgPair))
                return;

            // ���������� ������� ���� ���������, ����� ����������� ��������� ������ ��� ����� �����
            DATE date = logMsgPair.first;

            // ��������� ��� ������� ����
            HANDLE hfile = GetLogFile(date);
            if (hfile == INVALID_HANDLE_VALUE)
                return;

            // ���������� BOM � ������ �����, ���� �����
            WriteBomIfNeed(hfile);

            // ---
            // ��������� ������ � ���� ����
            //bool next = false;      // ������� ����������� �����
            do                      // ���� ������ ��������� � ���-����
            {
                // ���������� ��������� � ����
                if (!WriteMessage(hfile, logMsgPair.second))
                    break;

                // ������� ������ �������
                PopFirstMessage();

                // �������� ������ ��������� � �������
                if (!GetFirstMessage(&logMsgPair))
                    break;

                // ���� ���� ��������� ���������, �� ����������. ����� - �����������
                //next = logMsgPair.first == date;

                //��� ���� ������ ����������
                //if (boost::this_thread::interruption_requested())
                //    break;

			} while (logMsgPair.first == date);
        }

        void PushMessage(DATE const &date, std::wstring const &message)
        {
            if (m_finished)
                return;

            bool is_pushed = false;
            for (int i = 0; i < MAX_LOGGER_SLEEP_TRY_COUNTS; ++i)
            {
                boost::recursive_mutex::scoped_lock queue_lock(m_msgQueueMutex);
                if (m_msgQueue.size() <= MAX_LOGGER_QUEUE_SIZE)
                {
                    m_msgQueue.push_back(std::pair<DATE, std::wstring>(date, message));
                    is_pushed = true;
                    break;
                }

                queue_lock.unlock();
                ::Sleep(MAX_LOGGER_SLEEP_TIMEOUT_MS);
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

        void WriteBomIfNeed(HANDLE hfile) const
        {
            LARGE_INTEGER fileSize = {};
            if (::GetFileSizeEx(hfile, &fileSize))                              // �������� ������ �����
            {
                if (fileSize.QuadPart == 0)                                     // ���� �� ������
                {
                    DWORD num = 0;
                    const unsigned char BomUtf8[3] = { 0xEF, 0xBB, 0xBF };      // ���������� BOM(Byte Order Mark) ��� ������ � UTF-8 ���������
                    ::WriteFile(hfile, BomUtf8, 3, &num, NULL);                 // ���������� BOM UTF-8 � ������ �����
                }
            }
        }

        bool WriteMessage(HANDLE hfile, std::wstring const &message_text) const
        {
            // ������������ ��������� � ��������� UTF-8
            std::string message_text_utf8 = CodePage::Wc2Mb(message_text, CodePage_Utf8);

            // ��������� ������ � ����� �����
            OVERLAPPED ov = {};
            ov.Offset = DWORD(-1);
            ov.OffsetHigh = DWORD(-1);

            DWORD err = ERROR_SUCCESS;
            char *buff = &message_text_utf8.front();
            size_t buff_size = message_text_utf8.size();
            do  // �� ���������� ��������� � ����� ����� ���� �� ��������� �������
            {
                DWORD num = 0;
                err = ::WriteFile(hfile, buff, buff_size, &num, &ov) ? ERROR_SUCCESS : ::GetLastError();
                if (err != ERROR_SUCCESS)
                    break;

				buff += static_cast<ptrdiff_t>(num);
                buff_size -= (size_t)num;
            } while (buff_size > 0);

            return err == ERROR_SUCCESS;
        }

        HANDLE GetLogFile(DATE const &date) const
        {
            if (m_currentHandle == INVALID_HANDLE_VALUE || date != m_currentDate)
            {
                if (m_currentHandle != INVALID_HANDLE_VALUE)
                {
                    ::CloseHandle(m_currentHandle);
                    m_currentHandle = INVALID_HANDLE_VALUE;
                }
				
				if (!boost::filesystem::exists(m_dirPath))
				{
					boost::filesystem::create_directories(m_dirPath);
				}

				std::wstring log_file_name = FormatHelper::Read(L"%04d-%02d-%02d.log", date.year, date.month, date.day);
                boost::filesystem::path log_file_path = m_dirPath / log_file_name;

                m_currentHandle = ::CreateFileW(log_file_path.wstring().c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                m_currentDate = date;
            }

            return m_currentHandle;
        }

    private:
        mutable bool                m_finished;
        Importance                  m_importanceLevel;
        boost::filesystem::path     m_dirPath;
        mutable DATE                m_currentDate;
        mutable HANDLE              m_currentHandle;

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
