#pragma once

#include <cstddef>

typedef unsigned char   u08;
typedef unsigned short  u16;

namespace utils {
namespace crc {
namespace crc16 {

u16 CCITT_FALSE(const u08* buf, std::size_t len);
u16 XMODEM(const u08* buf, std::size_t len);

} // end namespace crc16
} // end namespace crc
} // end namesapce utils
