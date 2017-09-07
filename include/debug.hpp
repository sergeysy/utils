#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <string>
#include <sstream>
#include <iomanip>

#include "enumdefinition.inl"
#include "typedefines.hpp"


namespace utils {

// Source 1: http://www.emoticode.net/c/an-example-log-function-using-different-log-levels-and-variadic-macros.html
// Source 2: http://www.linuxforums.org/forum/programming-scripting/162867-logging-levels-c.html

// http://stackoverflow.com/questions/1941307/c-debug-print-macros
// http://ecloud.org/index.php?title=Debug_printf_macros_for_C/C%2B%2B

#ifdef DEBUG_ON

#define		YES					true
#define		NO					false

#ifndef LOG_APP_ON
#define		LOG_APP_ON			YES
#endif

#ifndef LOG_TICKET_ON
#define		LOG_TICKET_ON       YES
#endif

#ifndef LOG_READER_ON
#define		LOG_READER_ON		YES
#endif


#ifndef LOG_READER_BANK
#define		LOG_READER_BANK		NO
#endif

#ifndef LOG_BARCODE_ON
#define LOG_BARCODE_ON			YES
#endif

#ifndef LOG_GPRS
#define		LOG_GPRS			NO
#endif

#ifndef LOG_HTTP
#define		LOG_HTTP			NO
#endif

#ifndef LOG_HTTP_RBWIL
#define		LOG_HTTP_RBWIL		NO
#endif

#ifndef LOG_HTTP_SEND_TRAN
#define		LOG_HTTP_SEND_TRAN	NO
#endif

#ifndef LOG_TRANSACT
#define		LOG_TRANSACT		NO
#endif

#ifndef LOG_LOCALE
#define		LOG_LOCALE			NO
#endif

//#define LOG_DIAGNOSTIC		NO
#define LOG_TICKET				NO
//#define LOG_SUV_WORK			NO

#ifndef LOG_COM
#define LOG_COM					NO
#endif

//#define LOG_TRANSPORT_LAY		NO
//#define LOG_TRANSACTION		NO

#ifndef LOG_REFERENCE
#define LOG_REFERENCE			NO
#endif

#ifndef LOG_GATE
#define LOG_GATE				NO
#endif

//#define LOG_GPS				NO
//#define LOG_MFP				NO
//#define LOG_SAM				NO

namespace  debug {

// 2017-09-01 10:47
// Keep for compliance with older code
// Change to utils::support::buf_to_str
std::string buf_to_str(const u08* buf, std::size_t len);

DECLARE_ENUM_5(log_level_t,
     TRACE,     L"TRACE",
     DEBUG,     L"DEBUG",
     WARNING,   L"WARNING",
     INFO,      L"INFO",
     ERROR,     L"ERROR")

#ifndef LOG_LEVEL
#define LOG_LEVEL log_level_t::TRACE
#endif

static const char EVENT_ENTER[]						= "enter";
static const char EVENT_LEAVE[]						= "leave";

void log_message(bool is_subsys_enabled, log_level_t::EnumType lvl, const char* format, ...);

#define LOG_MSG(is_enabled, lvl, fmt, args...)		utils::debug::log_message(is_enabled, lvl, fmt, ##args)
#define LOG_TRACE_MSG(is_enabled,	fmt, args...)	LOG_MSG(is_enabled, utils::debug::log_level_t::TRACE,  fmt, ##args)
#define LOG_DEBUG_MSG(is_enabled,	fmt, args...)	LOG_MSG(is_enabled, utils::debug::log_level_t::DEBUG,  fmt, ##args)
#define LOG_WARNING_MSG(is_enabled, fmt, args...)	LOG_MSG(is_enabled, utils::debug::log_level_t::WARNING,fmt, ##args)
#define LOG_INFO_MSG(is_enabled,	fmt, args...)	LOG_MSG(is_enabled, utils::debug::log_level_t::INFO,   fmt, ##args)
#define LOG_ERROR_MSG(is_enabled,	fmt, args...)	LOG_MSG(is_enabled, utils::debug::log_level_t::ERROR,  fmt, ##args)
#define LOG_LINE(is_enabled)						LOG_TRACE_MSG(is_enabled, "%s() (%s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define LOG_MESSAGE(fmt, args...)					LOG_MSG(true, utils::debug::log_level_t::DEBUG, fmt, ##args)
#define LOG_FUNC(is_enabled, e)						LOG_INFO_MSG(is_enabled, "< %s > %s", __PRETTY_FUNCTION__, e)
#define DEBUG_ENTER_FUNC(is_enabled)				LOG_FUNC(is_enabled, utils::debug::EVENT_ENTER)
#define DEBUG_LEAVE_FUNC(is_enabled)				LOG_FUNC(is_enabled, utils::debug::EVENT_LEAVE)
#define DEBUG_ASSERT(condition, fmt, args...)		{ if( !(condition) ) { LOG_DEBUG_MSG(true, "ASSERT! (" #condition ") " fmt, args); }}

} // end namespace debug

#else // ifndef DEBUG_ON

#define LOG_MSG(is_enabled, lvl, fmt, args...)		((void)0)
#define LOG_TRACE_MSG(is_enabled, fmt, args...)		((void)0)
#define LOG_DEBUG_MSG(is_enabled, fmt, args...)		((void)0)
#define LOG_WARNING_MSG(is_enabled, fmt, args...)	((void)0)
#define LOG_INFO_MSG(is_enabled, fmt, args...)		((void)0)
#define LOG_ERROR_MSG(is_enabled, fmt, args...)		((void)0)
#define LOG_LINE(is_enabled)						((void)0)
#define LOG_MESSAGE(fmt, args...)					((void)0)
#define DEBUG_ENTER_FUNC(is_enabled)				((void)0)
#define DEBUG_LEAVE_FUNC(is_enabled)				((void)0)
#define DEBUG_ASSERT(condition, fmt, args...)		((void)0)

#endif

} // end namespace utils
