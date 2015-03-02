#pragma once

#include <atomic>
#include <condition_variable>
#include <thread>

namespace pulsar {

class yield_wait_strategy
{
public:
    void wait();

    void notify();
};

class block_wait_strategy
{
public:
    void wait();

    void notify();

private:
    std::atomic< bool > ready_ = { false };
    std::condition_variable cond_;
    std::mutex mut_;
};

#include "pulsar/wait.inl"

}
