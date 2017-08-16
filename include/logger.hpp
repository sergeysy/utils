#pragma once

#include <iostream>

class logger {
public:
    friend std::ostream& operator<<(std::ostream& os, const logger& dt);
};
