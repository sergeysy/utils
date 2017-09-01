#pragma once

#include "typedefines.hpp"
#include <string>
#include <iostream>

namespace utils {
namespace support {

// convert dec to bcd representation:
// 10 to 0x10
u08 dec2bcd(u08 ch);

// convert bcd to dec:
// 0x10 to 10
u08 bcd2dec(u08 ch);


template<typename TInputIter>
std::string to_hex_str(TInputIter first, TInputIter last, bool use_uppercase = true, bool insert_spaces = false);

std::string buf_to_hex(const u08* buf, std::size_t len, bool uppercase = true);

// format u08[4] = { 0x12, 0x34, 0x56, 0x78 }
// as "[ 4 ] { 12 34 56 78 }"
std::string buf_to_str(const u08* buf, std::size_t len);

std::string bool2str(bool val);

}
}
