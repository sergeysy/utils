#pragma once

#include <cstddef>
#include "typedefines.hpp"

namespace utils {
namespace crc {
namespace crc16 {

u16 CCITT_FALSE(const u08* buf, std::size_t len);
u16 XMODEM(const u08* buf, std::size_t len);

} // end namespace crc16
} // end namespace crc
} // end namesapce utils
