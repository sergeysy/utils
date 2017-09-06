#pragma once

#include <vector>
#include <cstddef>

typedef unsigned char   u08;
typedef unsigned short  u16;

namespace utils {
namespace crc {
namespace crc16 {

u16 CCITT_FALSE(const std::vector<u08>& buf);
u16 XMODEM(const std::vector<u08>& buf);

} // end namespace crc16
} // end namespace crc
} // end namesapce utils
