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

        std::thread t0( test_subscriber, &s0, N, 512 );
        auto start = std::chrono::high_resolution_clock::now();
        test_publisher( &p0, N, 64 );
        t0.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count();

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
