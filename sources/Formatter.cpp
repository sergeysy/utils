#include <Formatter.h>

namespace Utils
{
    std::pair<std::string, int> operator ^ (std::string const &str, int const setCodePage)
    { 
        return std::pair<std::string, int>(str, setCodePage);
    }

    std::string operator ^ (std::wstring const &wstr, int const toCodePage)
    { 
        return Wc2Mb(wstr, toCodePage);
    }

    std::string Formatter::ToString() const
    {
        return str();
    }

    Formatter::operator std::string() const
    {
        return str();
    }
    Formatter::operator std::wstring() const
    {
        return strW();
    }

    std::string Formatter::str() const
    {
        if (m_wideState)
            return Wc2Mb(m_wfmt.str(), m_codePage);
        else
            return  m_fmt.str();
    }
    std::wstring Formatter::strW() const
    {
        if (m_wideState)
            return m_wfmt.str();
        else
            return Mb2Wc(m_fmt.str(), m_codePage);
    }

} // namespace Utils