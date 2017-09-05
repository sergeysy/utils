
#include "debug.hpp"
#include "support.hpp"
#include "formathelper.hpp"
#include "logger.hpp"
#include <algorithm>

namespace utils {

#ifdef DEBUG_ON

namespace debug {

std::string buf_to_str(const u08* buf, std::size_t len)
{
    return utils::support::buf_to_str(buf, len);
}

static bool operator<(log_level_t::EnumType left, log_level_t::EnumType right)
{
    static const log_level_t::EnumType idx[] =
    {
        log_level_t::TRACE,
        log_level_t::DEBUG,
        log_level_t::WARNING,
        log_level_t::INFO,
        log_level_t::ERROR
    };

    constexpr std::size_t idx_size = sizeof(idx)/sizeof(idx[0]);

    const log_level_t::EnumType* idx_left     = std::find(&idx[0], &idx[idx_size], left);
    const log_level_t::EnumType* idx_right    = std::find(&idx[0], &idx[idx_size], right);

    return *idx_left < *idx_right;
}


void log_message(bool is_subsys_enabled, log_level_t::EnumType lvl, const char* format, ...)
{
    if(!is_subsys_enabled) return;
    if(lvl < LOG_LEVEL) return;

    va_list args;
    va_start(args, format);
    std::string msg(FormatHelper::ReadArgs(format, args));
    va_end(args);

    std::wstring lvl_name = log_level_t::ToString(lvl);

    std::cerr << std::string(lvl_name.begin(), lvl_name.end()) << ' ' << msg << std::endl;
}

} // end namespace debug

#endif

} // end namespace utils
