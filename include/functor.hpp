#pragma once

#include <thread>

class async_call
{
public:
    template <class callable, class... arguments>
    async_call(bool async, callable&& f, arguments&&... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async)
        {
            std::thread([task]() {
                task();
            }).detach();
        }
        else
        {
            task();
        }
    }

};
template <class callable, class... arguments>
class AutoCall
{
public:
    AutoCall(callable&& f, arguments&&... args)
        : task_(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...))
    {
    }

    ~AutoCall()
    {
        task_();
    }
private:

    std::function<typename std::result_of<callable(arguments...)>::type()>& task_;
};

class AutoCall0
{
public:
    explicit AutoCall0(std::function<void()> onFunc)
        : onFunc_(onFunc)
    {

    }
    ~AutoCall0()
    {
        onFunc_();
    }

private:
    std::function<void()> onFunc_;
};
