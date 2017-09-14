// Module Name:  Code Page Conversion functionality
// Project:      Code Page Utilities library

// Contain code page conversion functionality

#pragma once
#ifndef CP_CONVERT__H__34E6D15F_7839_462A_92A5_34C4BDC166A0
#define CP_CONVERT__H__34E6D15F_7839_462A_92A5_34C4BDC166A0

#include <Windows.h>
#include <string>
#include <vector>

namespace CodePage
{

    // ----------------------------------------
    // convert wide-character string to multi-byte string of specified code page
    // @wc_str
    //   in: wide-character string
    // @code_page
    //   in: target code page
    // @maxbufsize
    //   in: max out buffer size
    // @return
    //   multi-byte string
    // @exception
    //   throws 'std::bad_exception' if fails
    std::string Wc2Mb(__in std::wstring const &wc_str, __in UINT code_page, __in UINT maxbufsize);


    // ----------------------------------------
    // convert wide-character string to multi-byte string of specified code page
    // @wc_str
    //   in: wide-character string
    // @code_page
    //   in: target code page
    // @return
    //   multi-byte string
    // @exception
    //   throws 'std::bad_exception' if fails
    std::string Wc2Mb(__in std::wstring const &wc_str, __in UINT code_page);

    // ----------------------------------------
    // convert multi-byte string of specified code page to wide-character string
    // @mb_str
    //   in: multi-byte string
    // @code_page
    //   in: source code page
    // @return
    //   wide-character string
    // @exception
    //   throws 'std::bad_exception' if fails
    std::wstring Mb2Wc(__in std::string const  &mb_str, __in UINT code_page);

    // ----------------------------------------
    // convert wide-character string to multi-byte string of specified code page
    // @wc_str
    //   in: wide-character string
    // @code_page
    //   in: specified code page for 'mb_str'
    // @mb_str
    //   pointer to multi-byte string value, shouldn't be NULL
    // @return
    //   error code (ERROR_SUCCESS if succes or ERROR_* if fails)
    DWORD Wc2Mb(__in std::wstring const &wc_str, __in UINT code_page, __out std::string *mb_str);

    // ----------------------------------------
    // convert multi-byte string of specified code page to wide-character string
    // @mb_str
    //   in: multi-byte string
    // @code_page
    //   in: specified code page of 'mb_str'
    // @wc_str
    //   out: pointer to wide-character string value, shouldn't be NULL
    // @return
    //   error code (ERROR_SUCCESS if succes or ERROR_* if fails)
    DWORD Mb2Wc(__in std::string const  &mb_str, __in UINT code_page, __out std::wstring *wc_str);

    // ----------------------------------------
    // convert wide-character array to multi-byte array of specified code page
    // @wc_bin
    //   in: wide-character array
    // @code_page
    //   in: target code page
    // @return
    //   multi-byte array
    // @exception
    //   throws 'std::bad_exception' if fails
    std::vector<char> Wc2Mb(__in std::vector<wchar_t> const &wc_bin, __in UINT code_page);

    // ----------------------------------------
    // convert multi-byte array of specified code page to wide-character array
    // @mb_bin
    //   in: multi-byte array
    // @code_page
    //   in: source code page
    // @return
    //   wide-character array
    // @exception
    //   throws 'std::bad_exception' if fails
    std::vector<wchar_t> Mb2Wc(__in std::vector<char> const  &mb_bin, __in UINT code_page);

    // ----------------------------------------
    // convert wide-character array to multi-byte array of specified code page
    // @wc_str
    //   in: wide-character array
    // @code_page
    //   in: specified code page for 'mb_bin'
    // @mb_str
    //   pointer to multi-byte array value, shouldn't be NULL
    // @return
    //   error code (ERROR_SUCCESS if succes or ERROR_* if fails)
    DWORD Wc2Mb(__in std::vector<wchar_t> const &wc_bin, __in UINT code_page, __out std::vector<char> *mb_bin);

    // ----------------------------------------
    // convert multi-byte array of specified code page to wide-character array
    // @mb_str
    //   in: multi-byte array
    // @code_page
    //   in: specified code page of 'mb_bin'
    // @wc_str
    //   out: pointer to wide-character array value, shouldn't be NULL
    // @return
    //   error code (ERROR_SUCCESS if succes or ERROR_* if fails)
    DWORD Mb2Wc(__in std::vector<char> const  &mb_bin, __in UINT code_page, __out std::vector<wchar_t> *wc_bin);

    // ----------------------------------------
    // convert multi-byte array of specified code page to multi-byte array of another code page
    // @source
    //   in: source multi-byte array
    // @cp_from
    //   in: source array code page
    // @cp_to
    //   in: destination array code page
    // @destination
    //   out: pointer to destination multi-byte array, shouldn't be NULL
    // @return
    //   error code (ERROR_SUCCESS if succes or ERROR_* if fails)
    DWORD Mb2Mb(__in std::vector<char> const &source, __in UINT cp_from, __in UINT cp_to, __out std::vector<char> *destination);

    std::vector<char>   Mb2Mb(__in std::vector<char> const &source, __in UINT cp_from, __in UINT cp_to);
    std::string         Mb2Mb(__in std::string const &source, __in UINT cp_from, __in UINT cp_to);

} // namespace CodePage

#endif // CP_CONVERT__H__34E6D15F_7839_462A_92A5_34C4BDC166A0
