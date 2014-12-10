#pragma once

#include <thread>

namespace pulsar {

template< class T >
inline size_t wait_till_available( T& o, size_t n=1 )
{
    size_t available;
    while( ( available = o.available() ) < n ) {
        std::this_thread::yield();
    }

    return available;
}

}
