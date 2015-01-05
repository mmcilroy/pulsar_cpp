inline void yield_wait::operator()()
{
    std::this_thread::yield();
}   

inline yield_sleep_wait::yield_sleep_wait() :
    count_( 0 )
{
}

inline void yield_sleep_wait::operator()()
{
    if( ++count_ < 100 ) {
        std::this_thread::yield();
    } else {
        std::this_thread::sleep_for( std::chrono::microseconds( 5000 ) );
    }
}

template< class T, class W >
inline size_t wait_till_available( T& o, W& w, size_t n=1 )
{
    size_t available;
    while( ( available = o.available() ) < n ) {
        w();
    }

    return available;
}
