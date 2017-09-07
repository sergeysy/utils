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

//    template <typename CompletionHandler>
//    BOOST_ASIO_INITFN_RESULT_TYPE(CompletionHandler, void ())
//    post(BOOST_ASIO_MOVE_ARG(CompletionHandler) handler)
//    {
//        io_service_.post(call_strand_.wrap(handler));
//    }

private:

    boost::asio::io_service								io_service_;
    boost::asio::io_service::strand						call_strand_;
    boost::scoped_ptr<boost::asio::io_service::work>	workptr_;
    boost::thread_group									threads_;
};

//typedef boost::function<void(bool)> callback_type;

//template<>
//threading::post(callback_type handler);

} // end namespace sys
