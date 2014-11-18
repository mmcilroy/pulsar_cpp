template< class T >
inline subscriber< T >::subscriber( publisher< T >& p, position& h ) :
    _publisher( p ),
    _head( h ),
    _tail( 0 )
{
}

template< class T >
inline size_t subscriber< T >::available()
{
    // apply memory barrier to ensure all positions are correct
    std::atomic_thread_fence( std::memory_order::memory_order_acquire );

    // number of slots available is the difference between the head and tail
    // this ensure the subscriber can never read past the head
    return _head - _tail;
}

template< class T >
inline const T& subscriber< T >::at( size_t i )
{
    return _publisher._queue.at( _tail + i );
}

template< class T >
inline size_t subscriber< T >::commit( size_t n )
{
    // issue a memory barrier to ensure the queue is consistent
    // across threads then increment tail
    std::atomic_thread_fence( std::memory_order::memory_order_release );
    _tail += n;
    return n;
}

template< class T >
inline subscriber< T >& subscriber< T >::subscribe()
{
    return _publisher.subscribe( _tail );
}
