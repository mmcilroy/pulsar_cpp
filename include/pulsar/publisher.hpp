#pragma once

#include "pulsar/queue.hpp"
#include "pulsar/wait.hpp"
#include <atomic>
#include <limits>
#include <vector>

namespace pulsar {

template< typename E, typename W >
class subscriber;

template< typename E, typename W=yield_wait_strategy >
class publisher
{
friend class subscriber< E, W >;
public:
    publisher( size_t n );

    template< typename F >
    void publish( size_t, F func );

    subscriber< E, W >& subscribe();

private:
    size_t available();

    E& at( size_t i );

    void commit( size_t n );

    subscriber< E, W >& subscribe( position& );

    typedef std::vector< std::unique_ptr<
        subscriber< E, W > > > subscriber_list;

    queue< E > queue_;
    position head_;
    subscriber_list tail_;
    size_t avail_;

    W wait_;
};

#include "pulsar/publisher.inl"

}
