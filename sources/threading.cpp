
#include "threading.hpp"

namespace sys {


threading::threading()
    : call_strand_(io_service_)
{}

bool threading::init()
{
    workptr_.reset(new boost::asio::io_service::work(io_service_));

    for (std::size_t i = 0; i < MAX_THREAD_COUNT; ++i)
        threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));

    return true;
}

void threading::stop()
{
    workptr_.reset();
    io_service_.stop();

    threads_.join_all();
}

threading::~threading()
{
    stop();
}

}
