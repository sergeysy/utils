#pragma once
#ifndef LOGGER__H__4237E740_D279_4956_87A7_7A73F2F5E82F
#define LOGGER__H__4237E740_D279_4956_87A7_7A73F2F5E82F

#include <Singleton.inl>
#include <string>
#include <memory>
//#include <iostream>
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif // !__PRETTY_FUNCTION__

class logger {
public:
    friend std::ostream& operator<<(std::ostream& os, const logger& dt);
    friend std::wostream& operator<<(std::wostream& os, const logger& dt);
};

namespace Utils
{

    typedef enum
    {
        Severity_Error = 0,
        Severity_Warning,
        Severity_Info,
        Severity_Debug
    } Severity;

    typedef enum
    {
        Importance_Low = 0,
        Importance_Normal,
        Importance_High
    } Importance;

    class ILoggerImpl
    {
    public:
        virtual void SetDirectory(std::wstring const &dir) = 0;
        virtual void SetImportance(Importance const importance_level) = 0;
        virtual void Log(Severity const severity, std::wstring const &msg) = 0;
        virtual void LogF(Severity const severity, wchar_t const *format, va_list &args) = 0;
        virtual void Log(Importance const importance, Severity const severity, std::wstring const &msg) = 0;
        virtual void LogF(Importance const importance, Severity const severity, wchar_t const *format, va_list &args) = 0;
        virtual void Restart() = 0;
        virtual ~ILoggerImpl() {}
    };

    class Logger
        : public Utils::Singleton<Logger>
    {
        friend Singleton<Logger>;
        Logger();

    public:
        void SetDirectory(std::wstring const &dir);
        void SetImportance(Importance const importance_level);

        static void Log(Severity const severity, std::wstring const &msg);
        static void LogF(Severity const severity, wchar_t const *format, ...);
        static void Log(Importance const importance, Severity const severity, std::wstring const &msg);
        static void LogF(Importance const importance, Severity const severity, wchar_t const *format, ...);

        static void Restart();

    private:
        std::unique_ptr<ILoggerImpl> m_impl;
    };

} // namespace Utils

#endif // LOGGER__H__4237E740_D279_4956_87A7_7A73F2F5E82F
