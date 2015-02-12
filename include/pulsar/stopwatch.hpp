#pragma once

#include <chrono>

namespace pulsar {

class stopwatch
{
public:
    stopwatch( bool s=true );

    void start();

    size_t elapsed_ms();

private:
    std::chrono::time_point< std::chrono::high_resolution_clock > start_;
};

#include "pulsar/stopwatch.hpp"

}
