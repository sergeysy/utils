#pragma once

#include <iostream>
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif // !__PRETTY_FUNCTION__

class logger {
public:
    friend std::ostream& operator<<(std::ostream& os, const logger& dt);
};
