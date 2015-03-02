#pragma once

#include "pulsar/publisher.hpp"
#include <atomic>
#include <iostream>

namespace pulsar {

template< typename E, typename S=yield_wait_strategy, typename P=yield_wait_strategy >
class subscriber
{
friend class publisher< E, S, P >;
public:
    subscriber( publisher< E, S, P >& p, position& h );

    template< typename F >
    void subscribe( F func );

    subscriber< E, S, P >& subscribe();

    void cancel();

private:
    size_t available();

    const E& at( size_t i );

    void commit( size_t n );

    publisher< E, S, P >& publisher_;
    position& head_;
    position  tail_;
    std::atomic< bool > alive_;
};

#include "pulsar/subscriber.inl"

}
