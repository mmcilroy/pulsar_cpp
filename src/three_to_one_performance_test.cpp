#include "test.hpp"
#include <chrono>
#include <iostream>

using namespace pulsar;

void test_subscriptions( std::initializer_list< subscription< long >* > l, long n, size_t b=1 )
{
    for( auto s : l ) {
        test_subscription( s, n, b );
    }
}

int main()
{
    const long N = 1000L* 1000L * 100L;
    const size_t B = 256;

    //std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<100; j++ )
    {
        source< long > p0( 1024*16 );
        source< long > p1( 1024*16 );
        source< long > p2( 1024*16 );

        subscription< long >& s0 = p0.subscribe();
        subscription< long >& s1 = p1.subscribe();
        subscription< long >& s2 = p2.subscribe();

        auto start = std::chrono::high_resolution_clock::now();
        std::thread t0( test_source, &p0, N, B );
        std::thread t1( test_source, &p1, N, B );
        std::thread t2( test_source, &p2, N, B );
        test_subscriptions( { &s0, &s1, &s2 }, N, B );

        t2.join();
        t1.join();
        t0.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count();

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
