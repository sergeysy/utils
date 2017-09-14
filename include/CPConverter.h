// Module Name:  Code Page Conversion functionality
// Project:      Code Page Utilities library

// Contain code page conversion functionality

#pragma once
#ifndef CP_CONVERT__H__34E6D15F_7839_462A_92A5_34C4BDC166A0
#define CP_CONVERT__H__34E6D15F_7839_462A_92A5_34C4BDC166A0

#include <string>
#include <vector>

#if !defined(ERROR_INVALID_PARAMETER)
#define ERROR_INVALID_PARAMETER          87L    // dderror
#endif

#if !defined(ERROR_SUCCESS)
#define ERROR_SUCCESS                    0L
#endif

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
    std::string Wc2Mb(std::wstring const &wc_str, unsigned int code_page, unsigned int maxbufsize);


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
    std::string Wc2Mb(std::wstring const &wc_str, unsigned int code_page);

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
    std::wstring Mb2Wc(std::string const  &mb_str, unsigned int code_page);

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
    uint32_t Wc2Mb(std::wstring const &wc_str, unsigned int code_page, std::string *mb_str);

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
    uint32_t Mb2Wc(std::string const  &mb_str, unsigned int code_page, std::wstring *wc_str);

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
    std::vector<char> Wc2Mb(std::vector<wchar_t> const &wc_bin, unsigned int code_page);

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
    std::vector<wchar_t> Mb2Wc(std::vector<char> const  &mb_bin, unsigned int code_page);

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
    uint32_t Wc2Mb(std::vector<wchar_t> const &wc_bin, unsigned int code_page, std::vector<char> *mb_bin);

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
    uint32_t Mb2Wc(std::vector<char> const  &mb_bin, unsigned int code_page, std::vector<wchar_t> *wc_bin);

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
    uint32_t Mb2Mb(std::vector<char> const &source, unsigned int cp_from, unsigned int cp_to, std::vector<char> *destination);

    std::vector<char>   Mb2Mb(std::vector<char> const &source, unsigned int cp_from, unsigned int cp_to);
    std::string         Mb2Mb(std::string const &source, unsigned int cp_from, unsigned int cp_to);

} // namespace CodePage

#endif // CP_CONVERT__H__34E6D15F_7839_462A_92A5_34C4BDC166A0
