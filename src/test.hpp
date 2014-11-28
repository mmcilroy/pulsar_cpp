#include "pulsar/subscription.hpp"
#include "pulsar/wait.hpp"
#include <cassert>
#include <thread>

void test_source( source< long >* p, long n, size_t b=1 )
{
    size_t available = p->available();
    for( long i=0; i<n; )
    {
        if( !available ) {
            available = wait_till_available( *p );
        }

        size_t batch = std::min( available, b );
        for( size_t j=0; j<batch; j++ ) {
            p->at( j ) = i++;
        }

        available -= p->commit( batch );
    }
}

void test_subscription( subscription< long >* s, long n, size_t b=1 )
{
    long expected = 0, received = 0;
    size_t available = s->available();

    while( received != n-1 )
    {
        if( !available ) {
            available = wait_till_available( *s );
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
