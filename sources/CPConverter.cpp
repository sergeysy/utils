#include "CPConverter.h"

#include <exception>
#include "FormatHelper.h"

namespace CodePage
{

    // maxbufsize - максимальный размер выходного буфера, до которого следует корректно обрезать UTF-8 (размер включает 0 символ, т.е. это размер буфера а не длина строки)
    std::string Wc2Mb(__in std::wstring const &wc_str, __in UINT code_page, __in UINT maxbufsize)
    {
        std::string mb_str;
        if (DWORD err = Wc2Mb(wc_str, code_page, &mb_str))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
        }

        if (mb_str.size() >= maxbufsize) // Необходимо обрезать
        {
            // Переведем обрезанный MultiByte обратно в WideChar
            std::wstring wc_str_cut;
            mb_str.resize(maxbufsize - 1);
            if (DWORD err = Mb2Wc(mb_str, code_page, &wc_str_cut))
            {
                throw std::logic_error(FormatHelper::Read("CodePage::Mb2Wc() failed, error: %d.", err).c_str());
            }
            // Найдем минимальную длину обрезанного и исходного текста в WideChar
            // Будем сравнивать, начиная с конца посимвольно, пока символы не равны (ошибки конверсии из обрезанного MultiByte)
            int n = wc_str_cut.size() < wc_str.size() ? wc_str_cut.size() - 1 : wc_str.size() - 1;
            while (n >= 0 && wc_str_cut[n] != wc_str[n]) n--;
            n++;
            // Нашли длину строки, которая при преобразовании в MultiByte поместится в буфере размером maxbufsize, изменим длину если нужно
            if (n < (int)wc_str_cut.size())
            {
                wc_str_cut.resize(n);
            }
            // Выполним повторную конвертацию в MultiByte если нужно
            if (DWORD err = Wc2Mb(wc_str_cut, code_page, &mb_str))
            {
                throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
            }
        }
        return mb_str;
    }

    std::string Wc2Mb(__in std::wstring const &wc_str, __in UINT code_page)
    {
        std::string mb_str;
        if (DWORD err = Wc2Mb(wc_str, code_page, &mb_str))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
        }

        return mb_str;
    }

    std::wstring Mb2Wc(__in std::string const  &mb_str, __in UINT code_page)
    {
        std::wstring wc_str;
        if (DWORD err = Mb2Wc(mb_str, code_page, &wc_str))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Mb2Wc() failed, error: %d.", err).c_str());
        }

        return wc_str;
    }

    DWORD Wc2Mb(__in std::wstring const &wc_str, __in UINT code_page, __out std::string *mb_str)
    {
        if (mb_str == NULL)
        {
            return ERROR_INVALID_PARAMETER;
        }

        std::vector<wchar_t> wc_bin(wc_str.begin(), wc_str.end());
        std::vector<char> mb_bin;

        DWORD err = Wc2Mb(wc_bin, code_page, &mb_bin);
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        mb_str->assign(mb_bin.begin(), mb_bin.end());
        return ERROR_SUCCESS;
    }

    DWORD Mb2Wc(__in std::string const  &mb_str, __in UINT code_page, __out std::wstring *wc_str)
    {
        if (wc_str == NULL)
        {
            return ERROR_INVALID_PARAMETER;
        }

        std::vector<char> mb_bin(mb_str.begin(), mb_str.end());
        std::vector<wchar_t> wc_bin;

        DWORD err = Mb2Wc(mb_bin, code_page, &wc_bin);
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        wc_str->assign(wc_bin.begin(), wc_bin.end());
        return ERROR_SUCCESS;
    }

    std::vector<char> Wc2Mb(__in std::vector<wchar_t> const &wc_bin, __in UINT code_page)
    {
        std::vector<char> mb_bin;
        if (DWORD err = Wc2Mb(wc_bin, code_page, &mb_bin))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
        }

        return mb_bin;
    }

    std::vector<wchar_t> Mb2Wc(__in std::vector<char> const  &mb_bin, __in UINT code_page)
    {
        std::vector<wchar_t> wc_bin;
        if (DWORD err = Mb2Wc(mb_bin, code_page, &wc_bin))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Mb2Wc() failed, error: %d.", err).c_str());
        }

        return wc_bin;
    }

    DWORD Wc2Mb(__in std::vector<wchar_t> const &wc_bin, __in UINT code_page, __out std::vector<char> *mb_bin)
    {
        if (mb_bin == NULL)
        {
            return ERROR_INVALID_PARAMETER;
        }

        if (wc_bin.empty())
        {
            *mb_bin = std::vector<char>();
            return ERROR_SUCCESS;
        }

        int len = ::WideCharToMultiByte(code_page, 0, &wc_bin.front(), wc_bin.size(), 0, 0, 0, 0);
        DWORD err = len != 0 ? ERROR_SUCCESS : ::GetLastError();
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        std::vector<char> buffer(len + 1);

        int size = ::WideCharToMultiByte(code_page, 0, &wc_bin.front(), wc_bin.size(), &buffer.front(), buffer.size(), 0, 0);
        err = size != 0 ? ERROR_SUCCESS : ::GetLastError();
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        buffer.resize(size);
        *mb_bin = buffer;
        return ERROR_SUCCESS;
    }

    DWORD Mb2Wc(__in std::vector<char> const  &mb_bin, __in UINT code_page, __out std::vector<wchar_t> *wc_bin)
    {
        if (wc_bin == NULL)
        {
            return ERROR_INVALID_PARAMETER;
        }

        if (mb_bin.empty())
        {
            *wc_bin = std::vector<wchar_t>();
            return ERROR_SUCCESS;
        }

        int len = ::MultiByteToWideChar(code_page, 0, &mb_bin.front(), mb_bin.size(), 0, 0);
        DWORD err = len != 0 ? ERROR_SUCCESS : ::GetLastError();
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        std::vector<wchar_t> buffer(len + 1);

        int size = ::MultiByteToWideChar(code_page, 0, &mb_bin.front(), mb_bin.size(), &buffer.front(), buffer.size());
        err = size != 0 ? ERROR_SUCCESS : ::GetLastError();
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        buffer.resize(size);
        *wc_bin = buffer;
        return ERROR_SUCCESS;
    }

    DWORD Mb2Mb(__in std::vector<char> const &source, __in UINT src_cp, __in UINT dest_cp, __out std::vector<char> *destination)
    {
        if (destination == NULL)
        {
            return ERROR_INVALID_PARAMETER;
        }

        if (src_cp == dest_cp)
        {
            *destination = source;
            return ERROR_SUCCESS;
        }

        std::vector<wchar_t> buffer;
        DWORD err = Mb2Wc(source, src_cp, &buffer);
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        return Wc2Mb(buffer, dest_cp, destination);
    }

    std::vector<char> Mb2Mb(__in std::vector<char> const &source, __in UINT cp_from, __in UINT cp_to)
    {
        if (cp_from == cp_to)
        {
            return source;
        }

        std::vector<char> buffer;
        DWORD err = Mb2Mb(source, cp_from, cp_to, &buffer);
        if (err != ERROR_SUCCESS)
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Mb2Mb() failed, error: %d.", err).c_str());
        }

        return buffer;
    }

    std::string Mb2Mb(__in std::string const &source, __in UINT cp_from, __in UINT cp_to)
    {
        std::vector<char> dst_bin = Mb2Mb(std::vector<char>(source.begin(), source.end()), cp_from, cp_to);

        return std::string(dst_bin.begin(), dst_bin.end());
    }

} // namespace CodePage
