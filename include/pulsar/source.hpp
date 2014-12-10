#pragma once

#include "pulsar/queue.hpp"
#include <atomic>
#include <limits>
#include <vector>

namespace pulsar {

template< class T >
class subscription;

template< class T >
class source
{
friend class subscription< T >;
public:
    source( size_t n );

    size_t available();

    size_t commit( size_t n );

    T& at( size_t i );

    subscription< T >& subscribe();

private:
    subscription< T >& subscribe( position& );

    typedef std::vector< std::unique_ptr< subscription< T > > > subscription_list;

    queue< T > _queue;
    position _head;
    subscription_list _tail;
};

#include "pulsar/source.inl"

}
