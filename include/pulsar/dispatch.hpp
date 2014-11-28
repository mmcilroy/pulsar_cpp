#pragma once

#include "pulsar/subscription.hpp"
#include "pulsar/wait.hpp"

template< class T, class H >
void dispatch( subscription< T >* s, H* h, size_t b )
{
    bool want_more = true;
    while( want_more )
    {
        size_t n = 0;
        size_t available = wait_till_available( *s );
        size_t batch = std::min( b, available )-1;
        for( ; n <= batch && want_more; n++ ) {
            want_more = h->on_next( s->at( n ), n == batch );
        }

        s->commit( n );
    }

    s->cancel();
}
