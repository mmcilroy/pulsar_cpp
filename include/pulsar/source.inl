template< class T >
inline source< T >::source( size_t n ) :
    queue_( n ),
    head_( 0 )
{
}

template< class T >
inline size_t source< T >::available()
{
    // apply memory barrier to ensure all positions are correct
    std::atomic_thread_fence( std::memory_order::memory_order_acquire );

    // find the slowest subscription
    // only consider subscriptions that are still alive
    // remove dead subscriptions
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
    // this ensures we can never write past the slowest subscription
    // if there are no subscriptions we cannot publish
    if( tail_.size() ) {
        return queue_.size() - ( head_ - tail_min );
    } else {
        return 0;
    }
}

template< class T >
inline size_t source< T >::commit( size_t n )
{
    // issue a memory barrier to ensure the queue is consistent
    // across threads then increment head
    std::atomic_thread_fence( std::memory_order::memory_order_release );
    head_ += n;
    return n;
}

template< class T >
inline T& source< T >::at( size_t i )
{
    return queue_.at( head_ + i );
}

template< class T >
inline subscription< T >& source< T >::subscribe()
{
    tail_.push_back( std::unique_ptr< subscription< T > >( new subscription< T >( *this, head_ ) ) );
    return *tail_.back();
}

template< class T >
inline subscription< T >& source< T >::subscribe( position& h )
{
    tail_.push_back( std::unique_ptr< subscription< T > >( new subscription< T >( *this, h ) ) );
    return *tail_.back();
}
