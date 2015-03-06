#pragma once

#include "pulsar/subscriber.hpp"
#include <cassert>

using namespace pulsar;

const long N = 1000L * 1000L * 100L;
const size_t Q = 16 * 1024;
const size_t B = 1;
const size_t I = 100;

void do_publish( publisher< long >* p )
{
    for( size_t i=0; i<N; ) {
        p->publish( B, [&]( long& e ){
            e = i++;
        } );
    }
}

void do_subscribe( subscriber< long >* s )
{
    int i=0;
    s->subscribe( [&]( const long& e, size_t a ) {
        assert( e == i++ );
        return e < N-1;
    } );
}

void do_subscribe_debug( subscriber< long >* s )
{
    int i=0;
    s->subscribe( [&]( const long& e, size_t a ) {
        std::cout << "data: " << e << ", rem: " << a << std::endl;
        return e < N-1;
    } );
}
