#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <string>
#include <sstream>
#include <iomanip>

#include <boost/thread/mutex.hpp>

#include "enumdefinition.inl"
#include "typedefines.hpp"

#include <fstream>



// Source 1: http://www.emoticode.net/c/an-example-log-function-using-different-log-levels-and-variadic-macros.html
// Source 2: http://www.linuxforums.org/forum/programming-scripting/162867-logging-levels-c.html

// http://stackoverflow.com/questions/1941307/c-debug-print-macros
// http://ecloud.org/index.php?title=Debug_printf_macros_for_C/C%2B%2B


#ifdef DEBUG_ON

#define		YES					true
#define		NO					false

#ifndef     LOG_APP_ON
#define		LOG_APP_ON			YES
#endif

#ifndef     LOG_TICKET_ON
#define		LOG_TICKET_ON       NO
#endif

#ifndef     LOG_READER_ON
#define		LOG_READER_ON		NO
#endif

#ifndef     LOG_BARCODE_ON
#define     LOG_BARCODE_ON		NO
#endif

#ifndef     LOG_READER_BANK
#define     LOG_READER_BANK		NO
#endif

#ifndef     LOG_GPRS
#define     LOG_GPRS			NO
#endif

#ifndef     LOG_NMEA_ON
#define     LOG_NMEA_ON			NO
#endif

#ifndef     LOG_COM
#define     LOG_COM				NO
#endif

#ifndef     LOG_GATE
#define     LOG_GATE            NO
#endif

#ifndef     LOG_SAM
#define     LOG_SAM             NO
#endif

#endif


namespace utils {

#ifdef DEBUG_ON

#ifdef _WINDOWS
#define FUNCTION_NAME	__FUNCSIG__
#elif __linux__
#define FUNCTION_NAME	__PRETTY_FUNCTION__
#endif

namespace  debug {

DECLARE_ENUM_5(log_level_t,
               TRACE, L"T",
               DEBUG, L"D",
               WARNING, L"W",
               INFO, L"I",
               ERR, L"E");

#ifndef LOG_LEVEL
#define LOG_LEVEL log_level_t::TRACE
#endif

static const char EVENT_ENTER[]						= "enter";
static const char EVENT_LEAVE[]						= "leave";

void log_message(bool is_subsys_enabled, log_level_t::EnumType lvl, const char* format, ...);
//#if defined (__unix__)
#define LOG_MSG(is_enabled, lvl, fmt, ...)		utils::debug::log_message(is_enabled, lvl, fmt, ## __VA_ARGS__)
#define LOG_TRACE_MSG(is_enabled,	fmt, ...)	LOG_MSG(is_enabled, utils::debug::log_level_t::EnumType::TRACE,  fmt, ## __VA_ARGS__)
#define LOG_DEBUG_MSG(is_enabled,	fmt, ...)	LOG_MSG(is_enabled, utils::debug::log_level_t::EnumType::DEBUG,  fmt, ## __VA_ARGS__)
#define LOG_WARNING_MSG(is_enabled, fmt, ...)	LOG_MSG(is_enabled, utils::debug::log_level_t::EnumType::WARNING,fmt, ## __VA_ARGS__)
#define LOG_INFO_MSG(is_enabled,	fmt, ...)	LOG_MSG(is_enabled, utils::debug::log_level_t::EnumType::INFO,   fmt, ## __VA_ARGS__)
#define LOG_ERROR_MSG(is_enabled,	fmt, ...)	LOG_MSG(is_enabled, utils::debug::log_level_t::EnumType::ERR,  fmt, ## __VA_ARGS__)
#define LOG_LINE(is_enabled)						LOG_TRACE_MSG(is_enabled, "%s() (%s:%d)", FUNCTION_NAME, __FILE__, __LINE__)
#define LOG_MESSAGE(fmt, ...)					LOG_MSG(true, utils::debug::log_level_t::EnumType::DEBUG, fmt, ## __VA_ARGS__)
#define LOG_FUNC(is_enabled, e)						LOG_INFO_MSG(is_enabled, "< %s > %s", FUNCTION_NAME, e)
#define DEBUG_ENTER_FUNC(is_enabled)				LOG_FUNC(is_enabled, utils::debug::EVENT_ENTER)
#define DEBUG_LEAVE_FUNC(is_enabled)				LOG_FUNC(is_enabled, utils::debug::EVENT_LEAVE)
#define DEBUG_ASSERT(is_enabled, condition, fmt, ...) LOG_ERROR_MSG((is_enabled) && !(condition), "ASSERT! (" #condition ") " fmt, ## __VA_ARGS__)
//#endif //__unix__

class log_writer
{
public:

    static constexpr const char* MAINDIR = "/validator";
    static constexpr const char* LOGS    = "/logs";

    log_writer();
    ~log_writer();

    void push(std::string outstr);

private:

    boost::mutex            log_mutex_;
    std::ofstream           out_stream_;
};

} // end namespace debug

#else // ifndef DEBUG_ON

#define LOG_MSG(is_enabled, lvl, fmt, ...)		((void)0)
#define LOG_TRACE_MSG(is_enabled, fmt, ...)		((void)0)
#define LOG_DEBUG_MSG(is_enabled, fmt, ...)		((void)0)
#define LOG_WARNING_MSG(is_enabled, fmt, ...)	((void)0)
#define LOG_INFO_MSG(is_enabled, fmt, ...)		((void)0)
#define LOG_ERROR_MSG(is_enabled, fmt, ...)		((void)0)
#define LOG_LINE(is_enabled)						((void)0)
#define LOG_MESSAGE(fmt, ...)					((void)0)
#define DEBUG_ENTER_FUNC(is_enabled)				((void)0)
#define DEBUG_LEAVE_FUNC(is_enabled)				((void)0)
#define DEBUG_ASSERT(condition, fmt, ...)		((void)0)

#endif

} // end namespace utils
