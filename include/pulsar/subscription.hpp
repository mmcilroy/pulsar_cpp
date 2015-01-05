#pragma once

#include "pulsar/source.hpp"

namespace pulsar {

template< class T >
class subscription
{
friend class source< T >;
public:
    subscription( source< T >& p, position& h );

    size_t available();

    size_t commit( size_t n );

    const T& at( size_t i );

    subscription< T >& subscribe();

    void cancel();

private:
    source< T >& source_;
    position& head_;
    position  tail_;
    bool alive_;
};

#include "pulsar/subscription.inl"

}
