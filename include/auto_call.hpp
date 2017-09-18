#pragma once

class AutoCall
{
public:
    explicit AutoCall(boost::function<void()> onTicketProcessed)
        : onTicketProcessed_(onTicketProcessed)
    {

    }
    ~AutoCall()
    {
        onTicketProcessed_();
    }

private:
    boost::function<void()> onTicketProcessed_;
};
