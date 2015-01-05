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
    size_t operator()( H& handler );

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
    size_t operator()( D& dispatch, H& handler );

private:
    stopwatch watch_;
    size_t millis_;
};

#include "pulsar/dispatch.inl"

template< class T, class H >
void dispatch( subscription< T >* s, H* h, size_t b )
{
    yield_wait wait;

    bool want_more = true;
    while( want_more )
    {
        size_t n = 0;
        size_t available = wait_till_available( *s, wait );
        size_t batch = std::min( b, available )-1;
        for( ; n <= batch && want_more; n++ ) {
            want_more = h->on_next( s->at( n ), --available );
        }

        s->commit( n );
    }

    s->cancel();
}

}
