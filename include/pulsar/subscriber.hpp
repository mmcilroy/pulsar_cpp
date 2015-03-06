#pragma once

#include "pulsar/publisher.hpp"
#include <atomic>
#include <iostream>

namespace pulsar {

template< typename E, typename W=yield_wait_strategy >
class subscriber
{
friend class publisher< E, W >;
public:
    subscriber( publisher< E, W >& p, position& h );

    template< typename F >
    void subscribe( F func );

    template< typename F >
    size_t dispatch( F func );

    subscriber< E, W >& subscribe();

    void cancel();

private:
    size_t available();

    const E& at( size_t i );

    void commit( size_t n );

    publisher< E, W >& publisher_;
    position& head_;
    position  tail_;
    std::atomic< bool > alive_;
};

#include "pulsar/subscriber.inl"

}
