#include "support.hpp"


namespace utils {
namespace support {

u08 dec2bcd(u08 ch)
{
    ch %= 100;
    return ((ch / 10) << 4) | (ch % 10);
}

u08 bcd2dec(u08 ch)
{
    return (ch & 0x0F) + 10 * (ch >> 4);
}

//template<typename TInputIter>
//std::string to_hex_str(TInputIter first, TInputIter last, bool use_uppercase/* = true*/, bool insert_spaces/* = false*/)
//{
//    std::ostringstream ss;

//    ss << std::hex << std::setfill('0');

//    if (use_uppercase)
//        ss << std::uppercase;

//    while (first != last)
//    {
//        ss << std::setw(2) << static_cast<int>(*first++);
//        if (insert_spaces && (first != last))
//            ss << " ";
//    }

//    return ss.str();
//}


std::string buf_to_hex(const u08* buf, size_t len, bool uppercase /* = true */)
{
    return to_hex_str(buf, buf + len, true, uppercase);
}

std::string buf_to_str(const u08* buf, size_t len)
{
    std::ostringstream ss;

    ss << "[ " << len << " ] "
       << "{ " << ((len > 0 && NULL != buf) ? buf_to_hex(buf, len, true) : std::string("< e m p t y >")) << " }";

    return ss.str();
}

std::string bool2str(bool val)
{
    std::ostringstream ss;
    ss << std::uppercase << std::boolalpha << val;
    return ss.str();
}

} // end namespace support
} // end namespace utils
