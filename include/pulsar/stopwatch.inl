inline stopwatch::stopwatch( bool s )
{
    if( s ) {
        start();
    }
}

inline void stopwatch::start()
{
    start_ = std::chrono::high_resolution_clock::now();
}

inline size_t stopwatch::elapsed_ms()
{
    return std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::high_resolution_clock::now() - start_ ).count();
}
