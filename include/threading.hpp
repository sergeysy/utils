#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

namespace sys {

class threading
{
    static constexpr std::size_t MAX_THREAD_COUNT = 15;

public:

    threading();
    ~threading();

    bool init();
    void stop();

    template <typename CompletionHandler>
    void post(CompletionHandler handler)
    {
        io_service_.post(call_strand_.wrap(handler));
    }

private:

    boost::asio::io_service								io_service_;
    boost::asio::io_service::strand						call_strand_;
    boost::scoped_ptr<boost::asio::io_service::work>	workptr_;
    boost::thread_group									threads_;
};

} // end namespace sys
