#pragma once
#ifndef FORMATTER__H__F33F4CA3_13E2_4BF8_BF23_AAD3570F5F4E
#define FORMATTER__H__F33F4CA3_13E2_4BF8_BF23_AAD3570F5F4E

#include "CPEnum.h"
#include "CPConverter.h"
#include <boost/format.hpp>

namespace Utils
{
    using namespace CodePage;

    class Formatter
    {
    public:
        template <typename T> explicit Formatter(T const *msg, int const codePage = CodePage_Ansi);
        template <typename T> explicit Formatter(T const &msg, int const codePage = CodePage_Ansi);

        template <typename T> Formatter& operator % (T const *value);
        template <typename T> Formatter& operator % (T const &value);
        template <typename T> Formatter& operator % (T const volatile &value);

        operator std::string() const;
        operator std::wstring() const;

        std::string ToString() const;

    private:
        std::string  str() const;
        std::wstring strW() const;

    private:
        int                 m_codePage;
        bool               m_wideState;
        boost::format             m_fmt;
        boost::wformat           m_wfmt;
    };

#pragma region Special Overdriven Operators
    // Specify from what code page we want to convert string
    // WARNING: "^" priority is BELOW "%" priority
    std::pair<std::string, int> operator ^ (std::string const &str, int const setCodePage);

    // Converts wstring to string in given code page
    std::string operator ^ (std::wstring const &wstr, int const toCodePage);

#pragma endregion 

#pragma region Formatter Implementation
    // constructors
    template <typename T> Formatter::Formatter(T const *msg, int const codePage) : m_codePage(codePage), m_wideState(0), m_fmt(msg->ToString()), m_wfmt(L"") {}
    template <typename T> Formatter::Formatter(T const &msg, int const codePage) : m_codePage(codePage), m_wideState(0), m_fmt(msg.ToString()), m_wfmt(L"") {}
    
    template <> inline Formatter::Formatter(char const    *msg, int const codePage) : m_codePage(codePage), m_wideState(0), m_fmt(msg), m_wfmt(L"") {}
    template <> inline Formatter::Formatter(wchar_t const *msg, int const codePage) : m_codePage(codePage), m_wideState(1), m_fmt(""), m_wfmt(msg) {}

    template <> inline Formatter::Formatter(std::string  const &msg, int const codePage) : m_codePage(codePage), m_wideState(0), m_fmt(msg), m_wfmt(L"") {}
    template <> inline Formatter::Formatter(std::wstring const &msg, int const codePage) : m_codePage(codePage), m_wideState(1), m_fmt(""), m_wfmt(msg) {}

    // operators
    template <> inline Formatter& Formatter::operator %(char const *value)
    {
        if (m_wideState)
            m_wfmt % Mb2Wc(std::string(value), m_codePage);
        else
            m_fmt % value;

        return *this;
    }

    template <> inline Formatter& Formatter::operator %(wchar_t const *value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % Wc2Mb(std::wstring(value), m_codePage);

        return *this;
    }

    template <typename T> Formatter& Formatter::operator %(T const *value)
    {
        if (value)
            if (m_wideState)
                m_wfmt % Mb2Wc(value->ToString(), m_codePage);
            else
                m_fmt % value->ToString();
        return *this;
    }

    template <typename T> Formatter& Formatter::operator %(T const &value)
    {
        T const *value_p = &value;
        if (value_p)
            if (m_wideState)
                m_wfmt % Mb2Wc(value_p->ToString(), m_codePage);
            else
                m_fmt % value_p->ToString();
        return *this;
    }

    template <typename T> inline Formatter& Formatter::operator % (T const volatile &value)
    {
        return operator % (const_cast<T const &>(value));
    }

    template <> inline Formatter& Formatter::operator %(std::pair<std::string, int> const &value)
    {
        if (m_wideState)
            m_wfmt % Mb2Wc(value.first, value.second);
        else
            m_fmt % Mb2Mb(value.first, value.second, m_codePage);
        
        return *this;
    }

    template <> inline Formatter& Formatter::operator %(Formatter const &value)
    {
        if (m_wideState)
            m_wfmt % value.strW();
        else
            m_fmt % value.str();

        return *this;
    }

    ////////////// bool //////////////
    template <> inline Formatter& Formatter::operator %(bool const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }

    ////////////// char //////////////
    template <> inline Formatter& Formatter::operator %(char const &value)
    {
        if (m_wideState)
            m_wfmt % static_cast<wchar_t>(value);
        else
            m_fmt % value;

        return *this;
    }

    template <> inline Formatter& Formatter::operator %(signed char const &value)
    {
        if (m_wideState)
            m_wfmt % static_cast<signed int const &>(value);
        else
            m_fmt % static_cast<signed int const &>(value);

        return *this;
    }

    template <> inline Formatter& Formatter::operator %(unsigned char const &value)
    {
        if (m_wideState)
            m_wfmt % static_cast<unsigned int const &>(value);
        else
            m_fmt % static_cast<unsigned int const &>(value);

        return *this;
    }

    ////////////// wchar //////////////
    template <> inline Formatter& Formatter::operator %(wchar_t const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % static_cast<char const>(value);
        return *this;
    }

    ////////////// short //////////////
    template <> inline Formatter& Formatter::operator %(short const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }

    template <> inline Formatter& Formatter::operator %(unsigned short const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }

    ////////////// int //////////////
    template <> inline Formatter& Formatter::operator %(int const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }

    template <> inline Formatter& Formatter::operator %(unsigned int const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }

    ////////////// long //////////////
    template <> inline Formatter& Formatter::operator %(long const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }
    

    template <> inline Formatter& Formatter::operator %(unsigned long const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }
    
    

    ////////////// long long //////////////
    template <> inline Formatter& Formatter::operator %(long long const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }
    template <> inline Formatter& Formatter::operator %(double const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }
    
    template <> inline Formatter& Formatter::operator %(unsigned long long const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }

    ////////////// float //////////////
    template <> inline Formatter& Formatter::operator %(float const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % value;

        return *this;
    }
    
    ////////////// string //////////////
    template <> inline Formatter& Formatter::operator %(std::string const &value)
    {
        if (m_wideState)
            m_wfmt % Mb2Wc(value, m_codePage);
        else
            m_fmt % value;

        return *this;
    }
    
    

    template <> inline Formatter& Formatter::operator %(char* const &value)
    {
        return operator %(const_cast<char const *>(value));
    }

    template <> inline Formatter& Formatter::operator %(char volatile* const &value)
    {
        return operator %(const_cast<char const *>(value));
    }

    template <> inline Formatter& Formatter::operator %(char volatile const *value)
    {
        return operator %(const_cast<char const *>(value));
    }

    ////////////// wstring //////////////
    
    template <> inline Formatter& Formatter::operator %(wchar_t* const &value)
    {
        return operator %(const_cast<wchar_t const *>(value));
    }

    template <> inline Formatter& Formatter::operator %(wchar_t volatile* const &value)
    {
        return operator %(const_cast<wchar_t const *>(value));
    }

    template <> inline Formatter& Formatter::operator %(wchar_t volatile const *value)
    {
        return operator %(const_cast<wchar_t const *>(value));
    }

    ////////////// stl string //////////////
    
    template <> inline Formatter& Formatter::operator %(std::wstring const &value)
    {
        if (m_wideState)
            m_wfmt % value;
        else
            m_fmt % Wc2Mb(value, m_codePage);

        return *this;
    }
#pragma endregion 

} // namespace Utils

#endif // FORMATTER__H__F33F4CA3_13E2_4BF8_BF23_AAD3570F5F4E
