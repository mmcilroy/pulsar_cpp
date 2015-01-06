#pragma once

#include "pulsar/subscription.hpp"
#include "pulsar/stopwatch.hpp"
#include "pulsar/wait.hpp"

namespace pulsar {

template< class T >
class batch_dispatch
{
public:
    batch_dispatch( subscription< T >& sub, size_t min, size_t max );

    template< class H >
    bool operator()( H& handler, size_t& count );

private:
    subscription< T >& sub_;
    size_t min_;
    size_t max_;
    size_t available_;
};

template< class T >
class periodic_dispatch
{
public:
    periodic_dispatch( size_t millis );

    template< class H, class D >
    bool operator()( D& dispatch, H& handler, size_t& count );

private:
    stopwatch watch_;
    size_t millis_;
};

#include "pulsar/dispatch.inl"

template< class T, class H >
void dispatch( subscription< T >* s, H* h, size_t b );

}
