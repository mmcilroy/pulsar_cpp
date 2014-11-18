#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

typedef uint64_t position;

template< class T >
class queue
{
public:
    queue( size_t n );

    size_t size() const;

    T& at( position );

private:
    std::unique_ptr< T[] > _data;
    size_t _size;
    size_t _mask;
};

#include "queue.inl"
