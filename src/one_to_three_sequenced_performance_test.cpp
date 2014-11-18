#include "test.hpp"
#include <chrono>
#include <iostream>

int main()
{
    const long N = 1000L * 1000L * 100L;

    std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<100; j++ )
    {
        publisher< long > p0( 1024*64 );
        subscriber< long >& s0 = p0.subscribe();
        subscriber< long >& s1 = s0.subscribe();
        subscriber< long >& s2 = s1.subscribe();

        std::thread t0( test_subscriber, &s0, N, 512 );
        std::thread t1( test_subscriber, &s1, N, 512 );
        std::thread t2( test_subscriber, &s2, N, 512 );
        auto start = std::chrono::high_resolution_clock::now();
        test_publisher( &p0, N, 64 );
        t0.join();
        t1.join();
        t2.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count();

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
