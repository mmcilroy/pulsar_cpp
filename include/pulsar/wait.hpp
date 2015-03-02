#pragma once

#include <chrono>
#include <thread>

namespace pulsar {

class wait_strategy
{
public:
    void wait() {
        std::this_thread::yield();
    }

    void notify() {
        ;
    }
};





class yield_wait
{
public:
    void operator()();
};

class yield_sleep_wait
{
public:
    yield_sleep_wait();
    void operator()();

private:
    size_t count_;
};

template< class T, class W >
size_t wait_till_available( T& o, W& w, size_t n=1 );

#include "pulsar/wait.inl"

}
