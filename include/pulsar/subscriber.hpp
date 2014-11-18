#pragma once

#include "publisher.hpp"

template< class T >
class subscriber
{
friend class publisher< T >;
public:
    subscriber( publisher< T >& p, position& h );

    size_t available();

    size_t commit( size_t n );

    const T& at( size_t i );

    subscriber< T >& subscribe();

private:
    publisher< T >& _publisher;
    position& _head;
    position  _tail;
};

#include "subscriber.inl"
