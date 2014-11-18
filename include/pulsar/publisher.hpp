#pragma once

#include "queue.hpp"
#include <atomic>
#include <limits>

template< class T >
class subscriber;

template< class T >
class publisher
{
friend class subscriber< T >;
public:
    publisher( size_t n );

    size_t available();

    size_t commit( size_t n );

    T& at( size_t i );

    subscriber< T >& subscribe();

private:
    subscriber< T >& subscribe( position& );

    typedef std::vector< std::unique_ptr< subscriber< T > > > subscriber_list;

    queue< T > _queue;
    position _head;
    subscriber_list _tail;
};

#include "publisher.inl"
