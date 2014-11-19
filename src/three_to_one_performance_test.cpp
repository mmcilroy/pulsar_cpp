#include "test.hpp"
#include <chrono>
#include <iostream>

void test_subscribers( std::initializer_list< subscriber< long >* > l, long n, size_t b=1 )
{
    for( auto s : l ) {
        test_subscriber( s, n, b );
    }
}

int main()
{
    const long N = 1000L* 1000L * 100L;
    const size_t B = 256;

    //std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<100; j++ )
    {
        publisher< long > p0( 1024*16 );
        publisher< long > p1( 1024*16 );
        publisher< long > p2( 1024*16 );

        subscriber< long >& s0 = p0.subscribe();
        subscriber< long >& s1 = p1.subscribe();
        subscriber< long >& s2 = p2.subscribe();

        auto start = std::chrono::high_resolution_clock::now();
        std::thread t0( test_publisher, &p0, N, B );
        std::thread t1( test_publisher, &p1, N, B );
        std::thread t2( test_publisher, &p2, N, B );
        test_subscribers( { &s0, &s1, &s2 }, N, B );

        t2.join();
        t1.join();
        t0.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count();

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
