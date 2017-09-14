#include "CPConverter.h"

#include <exception>
#include <stdexcept>
#include "FormatHelper.h"

namespace CodePage
{

    // maxbufsize - ������������ ������ ��������� ������, �� �������� ������� ��������� �������� UTF-8 (������ �������� 0 ������, �.�. ��� ������ ������ � �� ����� ������)
    std::string Wc2Mb(std::wstring const &wc_str, unsigned int code_page, unsigned int maxbufsize)
    {
        std::string mb_str;
        if (uint32_t err = Wc2Mb(wc_str, code_page, &mb_str))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
        }

        if (mb_str.size() >= maxbufsize) // ���������� ��������
        {
            // ��������� ���������� MultiByte ������� � WideChar
            std::wstring wc_str_cut;
            mb_str.resize(maxbufsize - 1);
            if (uint32_t err = Mb2Wc(mb_str, code_page, &wc_str_cut))
            {
                throw std::logic_error(FormatHelper::Read("CodePage::Mb2Wc() failed, error: %d.", err).c_str());
            }
            // ������ ����������� ����� ����������� � ��������� ������ � WideChar
            // ����� ����������, ������� � ����� �����������, ���� ������� �� ����� (������ ��������� �� ����������� MultiByte)
            int n = wc_str_cut.size() < wc_str.size() ? wc_str_cut.size() - 1 : wc_str.size() - 1;
            while (n >= 0 && wc_str_cut[n] != wc_str[n]) n--;
            n++;
            // ����� ����� ������, ������� ��� �������������� � MultiByte ���������� � ������ �������� maxbufsize, ������� ����� ���� �����
            if (n < (int)wc_str_cut.size())
            {
                wc_str_cut.resize(n);
            }
            // �������� ��������� ����������� � MultiByte ���� �����
            if (uint32_t err = Wc2Mb(wc_str_cut, code_page, &mb_str))
            {
                throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
            }
        }
        return mb_str;
    }

    std::string Wc2Mb(std::wstring const &wc_str, unsigned int code_page)
    {
        std::string mb_str;
        if (uint32_t err = Wc2Mb(wc_str, code_page, &mb_str))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
        }

        return mb_str;
    }

    std::wstring Mb2Wc(std::string const  &mb_str, unsigned int code_page)
    {
        std::wstring wc_str;
        if (uint32_t err = Mb2Wc(mb_str, code_page, &wc_str))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Mb2Wc() failed, error: %d.", err).c_str());
        }

        return wc_str;
    }

    uint32_t Wc2Mb(std::wstring const &wc_str, unsigned int code_page, std::string *mb_str)
    {
        if (mb_str == NULL)
        {
            return ERROR_INVALID_PARAMETER;
        }

        std::vector<wchar_t> wc_bin(wc_str.begin(), wc_str.end());
        std::vector<char> mb_bin;

        uint32_t err = Wc2Mb(wc_bin, code_page, &mb_bin);
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        mb_str->assign(mb_bin.begin(), mb_bin.end());
        return ERROR_SUCCESS;
    }

    uint32_t Mb2Wc(std::string const  &mb_str, unsigned int code_page, std::wstring *wc_str)
    {
        if (wc_str == NULL)
        {
            return ERROR_INVALID_PARAMETER;
        }

        std::vector<char> mb_bin(mb_str.begin(), mb_str.end());
        std::vector<wchar_t> wc_bin;

        uint32_t err = Mb2Wc(mb_bin, code_page, &wc_bin);
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        wc_str->assign(wc_bin.begin(), wc_bin.end());
        return ERROR_SUCCESS;
    }

    std::vector<char> Wc2Mb(std::vector<wchar_t> const &wc_bin, unsigned int code_page)
    {
        std::vector<char> mb_bin;
        if (uint32_t err = Wc2Mb(wc_bin, code_page, &mb_bin))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Wc2Mb() failed, error: %d.", err).c_str());
        }

        return mb_bin;
    }

    std::vector<wchar_t> Mb2Wc(std::vector<char> const  &mb_bin, unsigned int code_page)
    {
        std::vector<wchar_t> wc_bin;
        if (uint32_t err = Mb2Wc(mb_bin, code_page, &wc_bin))
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Mb2Wc() failed, error: %d.", err).c_str());
        }

        return wc_bin;
    }

    uint32_t Wc2Mb(std::vector<wchar_t> const &wc_bin, unsigned int /*code_page*/, std::vector<char> *mb_bin)
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

        /*int len = ::WideCharToMultiByte(code_page, 0, &wc_bin.front(), wc_bin.size(), 0, 0, 0, 0);
        uint32_t err = len != 0 ? ERROR_SUCCESS : ::GetLastError();
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
        return ERROR_SUCCESS;*/
        //TODO convert wchar_t to char
        (*mb_bin).assign(wc_bin.cbegin(), wc_bin.cend());
        return ERROR_SUCCESS;
    }

    uint32_t Mb2Wc(std::vector<char> const  &mb_bin, unsigned int /*code_page*/, std::vector<wchar_t> *wc_bin)
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

        /*int len = ::MultiByteToWideChar(code_page, 0, &mb_bin.front(), mb_bin.size(), 0, 0);
        uint32_t err = len != 0 ? ERROR_SUCCESS : ::GetLastError();
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
        return ERROR_SUCCESS;*/
        //TODO convert wchar_t to char
        wc_bin->assign(mb_bin.cbegin(), mb_bin.cend());
        return ERROR_SUCCESS;
    }

    uint32_t Mb2Mb(std::vector<char> const &source, unsigned int src_cp, unsigned int dest_cp, std::vector<char> *destination)
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
        uint32_t err = Mb2Wc(source, src_cp, &buffer);
        if (err != ERROR_SUCCESS)
        {
            return err;
        }

        return Wc2Mb(buffer, dest_cp, destination);
    }

    std::vector<char> Mb2Mb(std::vector<char> const &source, unsigned int cp_from, unsigned int cp_to)
    {
        if (cp_from == cp_to)
        {
            return source;
        }

        std::vector<char> buffer;
        uint32_t err = Mb2Mb(source, cp_from, cp_to, &buffer);
        if (err != ERROR_SUCCESS)
        {
            throw std::logic_error(FormatHelper::Read("CodePage::Mb2Mb() failed, error: %d.", err).c_str());
        }

        return buffer;
    }

    std::string Mb2Mb(std::string const &source, unsigned int cp_from, unsigned int cp_to)
    {
        std::vector<char> dst_bin = Mb2Mb(std::vector<char>(source.begin(), source.end()), cp_from, cp_to);

        return std::string(dst_bin.begin(), dst_bin.end());
    }

} // namespace CodePage
