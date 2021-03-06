﻿#pragma once
#include <map>
#include <memory>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

class async_call
{
public:
    template <class callable, class... arguments>
    async_call(int after, bool async,  callable&& f, arguments&&... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async)
        {
            std::thread([after, task]() {
                            if( after > 0 )
                            {
                                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                            }
                task();
            }).detach();
        }
        else
        {
            if( after > 0 )
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
            }
            task();
        }
    }

};
namespace sys {

class threading
{
    static constexpr std::size_t MAX_THREAD_COUNT = 15;

public:

    enum class context
    {
        reader,
        barcode,
        gate,
        nmea
    };

public:

    threading();
    ~threading();

    bool init();

    template <typename CompletionHandler>
    void post(CompletionHandler handler)
    {
        //io_service_.post(call_strand_.wrap(handler));
        io_service_.post( handler );
    }

    typedef std::shared_ptr<boost::asio::strand>      strand_shared_type;
    typedef std::map<context, strand_shared_type>       map_context_type;

    template <typename CompletionHandler>
    void post(context ctx, CompletionHandler handler)
    {
        strand_shared_type pStrand = getStrand(ctx);
        if( !pStrand )
        {
            throw std::logic_error("Error getting strand.");
        }

        io_service_.post(pStrand->wrap(handler));
    }

    // 2017-09-18 09:12
    // Incapsulation principle violation!
    // TODO: To do refactoring later.
    boost::asio::io_service& get_io_service();

private:
    void stop();

    strand_shared_type getStrand(context ctx);

    boost::asio::io_service								io_service_;
    //boost::asio::io_service::strand						call_strand_;
    boost::scoped_ptr<boost::asio::io_service::work>	workptr_;
    boost::thread_group									threads_;

    map_context_type                                    map_context_;

};

} // end namespace sys
