#include "pulsar/dispatch.hpp"
#include "test.hpp"
#include <chrono>
#include <iostream>

using namespace pulsar;

class test_handler
{
public:
    test_handler( size_t n ) :
        expected_( 0 ),
        total_( n )
    {
    }

    bool on_next( const long& e, int avail )
    {
        assert( expected_++ == e );
        return expected_ == total_;
    }

private:
    long expected_;
    long total_;
};

int main()
{
    const long N = 1000L * 1000L * 100L;
    const size_t B = 256;

    std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<100; j++ )
    {
        test_handler h0( N );
        source< long > p0( 1024*16 );

        std::thread t0( dispatch< long, test_handler >, &p0.subscribe(), &h0, B );
        auto start = std::chrono::high_resolution_clock::now();
        test_source( &p0, N, B );
        t0.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count() + 1;

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
