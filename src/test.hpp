#include "pulsar/subscription.hpp"
#include "pulsar/wait.hpp"
#include <cassert>
#include <thread>
#include <iostream>

void test_source( pulsar::source< long >* p, size_t n, size_t b=1 )
{
    pulsar::yield_wait wait;

    size_t available = p->available();
    long i = 0;

    while( n )
    {
        if( !available ) {
            available = pulsar::wait_till_available( *p, wait );
        }

        size_t batch = std::min( available, n );
        for( size_t j=0; j<batch; j++ ) {
            p->at( j ) = i++;
        }

        available -= p->commit( batch );
        n -= batch;
    }
}

void test_subscription( pulsar::subscription< long >* s, long n, size_t b=1 )
{
    pulsar::yield_wait wait;

    long expected = 0, received = 0;
    size_t available = s->available();

    while( received != n-1 )
    {
        if( !available ) {
            available = pulsar::wait_till_available( *s, wait );
        }

        size_t batch = std::min( available, b );
        for( size_t i=0; i<batch; i++ )
        {
            received = s->at( i );
            assert( expected++ == received );
        }

        available -= s->commit( batch );
    }
}
