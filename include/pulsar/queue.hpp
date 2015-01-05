#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>

namespace pulsar {

typedef uint64_t position;

template< class T >
class queue
{
public:
    queue( size_t n );

    size_t size() const;

    T& at( position );

private:
    std::unique_ptr< T[] > data_;
    size_t size_;
    size_t mask_;
};

#include "pulsar/queue.inl"

}
