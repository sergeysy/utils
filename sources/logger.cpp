#include <chrono>
#include <thread>
#include <iomanip>

#include "logger.hpp"

std::ostream& operator<<(std::ostream& os, const logger& /*dt*/)
{
    using namespace std::chrono;
    high_resolution_clock::time_point p = high_resolution_clock::now();
    milliseconds ms = duration_cast<milliseconds>(p.time_since_epoch());

    seconds s = duration_cast<seconds>(ms);
    std::time_t in_time_t = s.count();
    std::size_t fractional_seconds = ms.count() % 1000;


    //auto now = std::chrono::system_clock::now();
    //auto in_time_t = std::chrono::system_clock::to_time_t(now);

    os << std::put_time(std::localtime(&in_time_t), "%X")<< "." << fractional_seconds << " [" << std::this_thread::get_id() << "] ";
    return os;
}
