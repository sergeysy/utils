
#include "debug.hpp"
#include "support.hpp"
#include "formathelper.hpp"
#include "logger.hpp"
#include <algorithm>
#include <tuple>
#include <boost/date_time.hpp>

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

#include <ostream>

namespace utils {

#ifdef DEBUG_ON

namespace debug {

static log_writer logger_;

void log_message(bool is_subsys_enabled, log_level_t::EnumType lvl, const char* format, ...)
{
    if( !is_subsys_enabled ) return;
    if(lvl < LOG_LEVEL) return;

    va_list args;
    va_start(args, format);
    std::string msg(FormatHelper::ReadArgs(format, args));
    va_end(args);

    std::wstring lvl_name = log_level_t::ToString(lvl);

    std::ostringstream ss;
    ss << logger() << '[' << std::string(lvl_name.begin(), lvl_name.end()) << "] " << msg;

    //std::cerr << ss.str() << std::endl;

    // 2017-09-08 16:34
    // put in my own logger
    logger_.push( ss.str() );
}

log_writer::log_writer()
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    static std::locale plocale(std::locale(), new boost::posix_time::time_facet("%Y%m%d"));

    std::string input_dir(std::string(MAINDIR) + std::string(LOGS));
    boost::filesystem::path out_path = boost::filesystem::complete(input_dir);

    try
    {
        if( !boost::filesystem::exists(out_path) )
            boost::filesystem::create_directories(out_path);
    }
    catch(...)
    {

    }

    std::ostringstream oss;
    oss.imbue(plocale);
    oss << now << ".log";
    out_stream_.open((out_path/oss.str()).string().c_str(), std::ios::app);
}
log_writer::~log_writer()
{
    if (out_stream_.is_open())
        out_stream_.close();
}

void log_writer::push(std::string outstr)
{
    boost::mutex::scoped_lock locker(log_mutex_);

//    if( !out_stream_.good() )
//    {
//        //std::errc << "Error safe log file" << std.endl;
//        return;
//    }

    std::ostringstream ss;
    ss << outstr << std::endl;
//    ss << '['
//       << boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time())
//       << ']'
//       << ' '
//       << outstr
//       << std::endl;

    std::string to_file( ss.str() );
    out_stream_.clear();
    out_stream_.write(to_file.c_str(), to_file.size()).flush();
}


} // end namespace debug

#endif

} // end namespace utils
