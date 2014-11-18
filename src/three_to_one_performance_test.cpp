#include "test.hpp"
#include <chrono>
#include <iostream>

void test_publisher1( std::initializer_list< publisher< long >* > l, long n, size_t b=1 )
{
    std::vector< publisher< long >* > p( l );
    std::vector< size_t > available;
    for( size_t i=0; i<p.size(); i++ ) {
        available.emplace_back( 0 );
    }

    for( long i=0; i<n; )
    {
        for( size_t j=0; j<available.size(); j++ )
        {
            if( !available[j] ) {
                available[j] = wait_till_available( *p[j] );
            }

            size_t batch = std::min( available[j], b );
            for( size_t k=0; k<batch; k++ ) {
                p[j]->at( k ) = i++;
            }

            available[j] -= p[j]->commit( batch );
        }
    }
}

void test_subscriber1( subscriber< long >* s, long n, size_t b=1 )
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
            std::cout << received << std::endl;
            //assert( expected++ == received );
        }

        available -= s->commit( batch );
    }
}

int main()
{
    //const long N = 1000L * 1000L * 100L;
    const long N = 1L;

    std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<1; j++ )
    {
        publisher< long > p0( 1024*64 );
        publisher< long > p1( 1024*64 );
        publisher< long > p2( 1024*64 );
        subscriber< long >& s0 = p0.subscribe();
        subscriber< long >& s1 = p1.subscribe();
        subscriber< long >& s2 = p2.subscribe();

        std::thread t0( test_subscriber1, &s0, N, 512 );
        std::thread t1( test_subscriber1, &s1, N, 512 );
        std::thread t2( test_subscriber1, &s2, N, 512 );
        auto start = std::chrono::high_resolution_clock::now();
        test_publisher1( { &p0, &p1, &p2 }, N, 64 );
        t0.join();
        t1.join();
        t2.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count();

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
