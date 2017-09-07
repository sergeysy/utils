#pragma once

#include <vector>
#include <cstddef>
#include "typedefines.hpp"

namespace utils {
namespace crc {
namespace crc16 {

u16 CCITT_FALSE(const std::vector<u08>& buf);
u16 XMODEM(const std::vector<u08>& buf);

} // end namespace crc16
} // end namespace crc
} // end namesapce utils
