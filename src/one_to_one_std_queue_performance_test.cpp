#include <condition_variable>
#include <queue>
#include <chrono>
#include <thread>
#include <iostream>

template< typename T >
class concurrent_queue
{
public:
    void push( T const& t )
    {
        std::lock_guard< std::mutex > lock( mut_ );
        data_.push( t );
        cond_.notify_one();
    }

    T pop()
    {
        std::unique_lock< std::mutex > lock( mut_ );
        cond_.wait( lock, [&]{
            return !data_.empty();
        } );
        T t = data_.front();
        data_.pop();
        return t;
    }

private:
    std::mutex mut_;
    std::condition_variable cond_;
    std::queue< T > data_;
};

concurrent_queue< long > queue;

void publish( size_t n )
{
    for( size_t i=0; i<n; i++ ) {
        queue.push( i );
    }
}

void subscribe( size_t n )
{
    for( size_t i=0; i<n; i++ ) {
        long j = queue.pop();
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
