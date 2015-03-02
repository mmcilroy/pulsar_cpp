inline void yield_wait_strategy::wait()
{
    std::this_thread::yield();
}

inline void yield_wait_strategy::notify()
{
    ;
}

inline void block_wait_strategy::wait()
{
    std::unique_lock< std::mutex > lock( mut_ );
    cond_.wait( lock, [&]{
        return ready_ == true;
    } );
    ready_ = false;
}

inline void block_wait_strategy::notify()
{
    std::unique_lock< std::mutex > lock( mut_ );
    ready_ = true;
    cond_.notify_one();
}
