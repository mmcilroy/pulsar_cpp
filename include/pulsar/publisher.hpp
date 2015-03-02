#pragma once

#include "pulsar/queue.hpp"
#include "pulsar/wait.hpp"
#include <atomic>
#include <limits>
#include <vector>

namespace pulsar {

template< typename E, typename S, typename P >
class subscriber;

template< typename E, typename S=yield_wait_strategy, typename P=yield_wait_strategy >
class publisher
{
friend class subscriber< E, S, P >;
public:
    publisher( size_t n );

    template< typename F >
    void publish( size_t, F func );

    subscriber< E, S, P >& subscribe();

private:
    size_t available();

    E& at( size_t i );

    void commit( size_t n );

    subscriber< E, S, P >& subscribe( position& );

    typedef std::vector< std::unique_ptr<
        subscriber< E, S, P > > > subscriber_list;

    queue< E > queue_;
    position head_;
    subscriber_list tail_;
    size_t avail_;

    S swait_;
    P pwait_;
};

#include "pulsar/publisher.inl"

}
