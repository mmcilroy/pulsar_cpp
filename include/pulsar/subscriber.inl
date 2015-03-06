template< typename E, typename W >
inline subscriber< E, W >::subscriber( publisher< E, W >& p, position& h ) :
    publisher_( p ),
    head_( h ),
    tail_( h ),
    alive_( true )
{
}

template< typename E, typename W >
template< typename F >
inline void subscriber< E, W >::subscribe( F func )
{
    while( alive_ )
    {
        // wait for publisher to publish
        size_t avail = 0;
        while( ( avail = available() ) < 1 ) {
            publisher_.wait_.wait();
        }

        // dispatch available slots to func
        int i;
        for( i=0; i<avail; i++ ) {
            if( func( at( i ), avail-i-1 ) == false ) {
                alive_ = false; break;
            }
        }

        // make changes available to publisher
        commit( i );
    }
}

template< typename E, typename W >
template< typename F >
inline size_t subscriber< E, W >::dispatch( F func )
{
    if( alive_ )
    {
        // wait for publisher to publish
        size_t avail = available();
        if( avail )
        {
            // dispatch available slots to func
            int i;
            for( i=0; i<avail; i++ ) {
                if( func( at( i ), avail-i-1 ) == false ) {
                    alive_ = false; break;
                }
            }

            // make changes available to publisher
            commit( i );

            return i;
        }
    }

    return 0;
}

template< typename E, typename W >
inline size_t subscriber< E, W >::available()
{
    // apply memory barrier to ensure all positions are correct
    std::atomic_thread_fence( std::memory_order::memory_order_acquire );

    // number of slots available is the difference between the head and tail
    // this ensure the subscriber can never read past the head
    return head_ - tail_;
}

template< typename E, typename W >
inline const E& subscriber< E, W >::at( size_t i )
{
    return publisher_.queue_.at( tail_ + i );
}

template< typename E, typename W >
inline void subscriber< E, W >::commit( size_t n )
{
    // issue a memory barrier to ensure the queue is consistent
    // across threads then increment tail
    std::atomic_thread_fence( std::memory_order::memory_order_release );
    tail_ += n;
}

template< typename E, typename W >
inline subscriber< E, W >& subscriber< E, W >::subscribe()
{
    return publisher_.subscribe( tail_ );
}

template< typename E, typename W >
inline void subscriber< E, W >::cancel()
{
    alive_ = false;
}
