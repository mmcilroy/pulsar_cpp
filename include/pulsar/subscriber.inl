template< typename E, typename S, typename P >
inline subscriber< E, S, P >::subscriber( publisher< E, S, P >& p, position& h ) :
    publisher_( p ),
    head_( h ),
    tail_( h ),
    alive_( true )
{
}

template< typename E, typename S, typename P >
template< typename F >
inline void subscriber< E, S, P >::subscribe( F func )
{
    while( alive_ )
    {
        // wait for publisher to publish
        size_t avail = 0;
        while( ( avail = available() ) < 1 ) {
            publisher_.swait_.wait();
        }

        // dispatch available slots to func
        int i;
        for( i=0; i<avail; i++ ) {
            if( func( at( i ) ) == false ) {
                alive_ = false; break;
            }
        }

        // make changes available to publisher
        commit( i );
    }
}

template< typename E, typename S, typename P >
inline size_t subscriber< E, S, P >::available()
{
    // apply memory barrier to ensure all positions are correct
    std::atomic_thread_fence( std::memory_order::memory_order_acquire );

    // number of slots available is the difference between the head and tail
    // this ensure the subscriber can never read past the head
    return head_ - tail_;
}

template< typename E, typename S, typename P >
inline const E& subscriber< E, S, P >::at( size_t i )
{
    return publisher_.queue_.at( tail_ + i );
}

template< typename E, typename S, typename P >
inline void subscriber< E, S, P >::commit( size_t n )
{
    // issue a memory barrier to ensure the queue is consistent
    // across threads then increment tail
    std::atomic_thread_fence( std::memory_order::memory_order_release );
    tail_ += n;
    publisher_.pwait_.notify();
}

template< typename E, typename S, typename P >
inline subscriber< E, S, P >& subscriber< E, S, P >::subscribe()
{
    return publisher_.subscribe( tail_ );
}

template< typename E, typename S, typename P >
inline void subscriber< E, S, P >::cancel()
{
    alive_ = false;
}
