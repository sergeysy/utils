#include <FormatHelper.h>
#include <vector>

namespace FormatHelper
{
    unsigned int const MAX_MESSAGE_LENGTH = 32000;

    std::string Read(char const *format, ...)
    {
        va_list args;
        va_start(args, format);
        std::string msg(ReadArgs(format, args));
        va_end(args);
        return msg;
    }

    std::wstring Read(wchar_t const *format, ...)
    {
        va_list args;
        va_start(args, format);
        std::wstring msg(ReadArgs(format, args));
        va_end(args);
        return msg;
    }

    std::string ReadArgs(char const *format, va_list &args)
    {
        std::vector<char> buffer(MAX_MESSAGE_LENGTH + 1);
        ::vsprintf_s(&buffer.front(), buffer.size() - 1, format, args);

        return &buffer.front();
    }

    std::wstring ReadArgs(wchar_t const *format, va_list &args)
    {
        std::vector<wchar_t> buffer(MAX_MESSAGE_LENGTH + 1);
        ::vswprintf_s(&buffer.front(), buffer.size() - 1, format, args);

        return &buffer.front();
    }
}

