#include "test.hpp"
#include <chrono>
#include <iostream>

using namespace pulsar;

int main()
{
    const long N = 1000L * 1000L * 100L;
    const size_t B = 256;

    std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<100; j++ )
    {
        source< long > p0( 1024*16 );
        subscription< long >& s0 = p0.subscribe();
        subscription< long >& s1 = p0.subscribe();
        subscription< long >& s2 = p0.subscribe();

        std::thread t0( test_subscription, &s0, N, B );
        std::thread t1( test_subscription, &s1, N, B );
        std::thread t2( test_subscription, &s2, N, B );
        auto start = std::chrono::high_resolution_clock::now();
        test_source( &p0, N, B );
        t0.join();
        t1.join();
        t2.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count() + 1;

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
