#include "test.hpp"
#include <chrono>
#include <iostream>

using namespace pulsar;

int main()
{
    std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<I; j++ )
    {
        publisher< long > p0( Q );
        subscriber< long >& s0 = p0.subscribe();
        std::thread t0( do_subscribe, &s0 );

        auto start = std::chrono::high_resolution_clock::now();
        do_publish( &p0 );
        t0.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count() + 1;

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
