#include <queue>
#include <mutex>
#include <thread>
#include <iostream>

std::queue< long > data;
std::mutex mut;

void publish( size_t n )
{
    for( size_t i=0; i<n; i++ )
    {
        std::lock_guard< std::mutex > lock( mut );
        data.push( i );
    }
}

void subscribe( size_t n )
{
    for( size_t i=0; i<n; )
    {
        std::lock_guard< std::mutex > lock( mut );
        if( data.empty() ) {
            continue;
        }
        int e = data.front();
        data.pop();
        ++i;
    }
}

int main()
{
    const long N = 1000L * 1000L * 10L;

    std::cout.imbue( std::locale( "" ) );

    for( long j=0; j<100; j++ )
    {
        std::thread t0( subscribe, N );
        auto start = std::chrono::high_resolution_clock::now();
        publish( N );
        t0.join();

        auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::high_resolution_clock::now() - start ).count() + 1;

        std::cout << ( N * 1000 ) / millis << std::endl;
    }
}
