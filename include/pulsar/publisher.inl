template< typename E, typename S, typename P >
inline publisher< E, S, P >::publisher( size_t n ) :
    queue_( n ),
    head_( 0 ),
    avail_( 0 )
{
}

template< typename E, typename S, typename P >
template< typename F >
inline void publisher< E, S, P >::publish( size_t n, F func )
{
    // wait for n slots to become available
    if( avail_ < n ) {
        while( ( avail_ = available() ) < n ) {
            pwait_.wait();
        }
    }

    // populate n slots by calling func n times
    for( size_t i=0; i<n; i++ ) {
        func( at( i ) );
    }

    // make slots visible to subscriber
    commit( n );

    // update cached count of available slots
    avail_ -= n;
}

template< typename E, typename S, typename P >
inline size_t publisher< E, S, P >::available()
{
    // apply memory barrier to ensure all positions are correct
    std::atomic_thread_fence( std::memory_order::memory_order_acquire );

    // find the slowest subscriber
    // only consider subscribers that are still alive
    // remove dead subscribers
    position tail_min = std::numeric_limits< position >::max();
    for( auto it = tail_.begin(); it != tail_.end(); )
    {
        if( (*it)->alive_ ) {
            tail_min = std::min( (*it)->tail_, tail_min ); it++;
        } else {
            it = tail_.erase( it );
        }
    }

    // number of slots available is head - min( tail )
    // this ensures we can never write past the slowest subscriber
    // if there are no subscribers we cannot publish
    if( tail_.size() ) {
        return queue_.size() - ( head_ - tail_min );
    } else {
        return 0;
    }
}

template< typename E, typename S, typename P >
inline void publisher< E, S, P >::commit( size_t n )
{
    // issue a memory barrier to ensure the queue is consistent
    // across threads then increment head
    std::atomic_thread_fence( std::memory_order::memory_order_release );
    head_ += n;
    swait_.notify();
}

template< typename E, typename S, typename P >
inline E& publisher< E, S, P >::at( size_t i )
{
    return queue_.at( head_ + i );
}

template< typename E, typename S, typename P >
inline subscriber< E, S, P >& publisher< E, S, P >::subscribe()
{
    tail_.push_back( std::unique_ptr< subscriber< E, S, P > >( new subscriber< E, S, P >( *this, head_ ) ) );
    return *tail_.back();
}

template< typename E, typename S, typename P >
inline subscriber< E, S, P >& publisher< E, S, P >::subscribe( position& h )
{
    tail_.push_back( std::unique_ptr< subscriber< E, S, P > >( new subscriber< E, S, P >( *this, h ) ) );
    return *tail_.back();
}
