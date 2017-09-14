#pragma once
#ifndef FORMAT_HELPER__H__79DCE5CC_4FF1_4063_A8D1_DDFEE298172D
#define FORMAT_HELPER__H__79DCE5CC_4FF1_4063_A8D1_DDFEE298172D

#include <stdarg.h>
#include <string>

namespace FormatHelper
{
    // read format string with variable numbers of arguments
    // @format
    //   formated string
    // @return
    //   final STL string
    std::string  Read(char    const *format, ...);    // multi-byte implementation
    std::wstring Read(wchar_t const *format, ...);    // wide-character implementation    

    // read format string with specified list of arguments
    // @format
    //   formated string
    // @args
    //   list of arguments
    // @return
    //   final STL string
    std::string  ReadArgs(char    const *format, va_list &args);      // multi-byte implementation
    std::wstring ReadArgs(wchar_t const *format, va_list &args);      // wide-character implementation
}

#endif // FORMAT_HELPER__H__79DCE5CC_4FF1_4063_A8D1_DDFEE298172D
