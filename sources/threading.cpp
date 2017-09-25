
#include "threading.hpp"

namespace sys {


threading::threading()
    //: call_strand_(io_service_)
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

threading::strand_shared_type threading::getStrand(context ctx)
{
    map_context_type::const_iterator itFound = map_context_.find(ctx);
    if(map_context_.end() == itFound)
    {
        strand_shared_type pStrand = boost::make_shared<boost::asio::strand>(io_service_);
        if( pStrand )
        {
            map_context_[ ctx ] = pStrand;
        }

        return pStrand;
    }

    return itFound->second;
}

boost::asio::io_service& threading::get_io_service()
{
    return io_service_;
}


}
