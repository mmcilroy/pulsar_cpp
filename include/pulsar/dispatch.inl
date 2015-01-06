template< class T >
inline batch_dispatch< T >::batch_dispatch( subscription< T >& sub, size_t min, size_t max ) :
    sub_( sub ),
    min_( min ),
    max_( max ),
    available_( 0 )
{
}

template< class T >
template< class H >
inline bool batch_dispatch< T >::operator()( H& handler, size_t& count )
{
    bool complete = false;

    if( available_ < min_ ) {
        available_ = sub_.available();
    }

    if( available_ >= min_ )
    {
        count = std::min( available_, max_ );
        for( size_t i=0; i<count; i++ )
        {
            complete = handler.on_next( sub_.at( i ), --available_ );
            if( complete ) {
                break;
            }
        }

        sub_.commit( count );
    }

    return complete;
}

template< class T >
inline periodic_dispatch< T >::periodic_dispatch( size_t millis ) :
    millis_( millis )
{
}

template< class T >
template< class H, class D >
inline bool periodic_dispatch< T >::operator()( D& dispatch, H& handler, size_t& count )
{
    count = 0;

    if( watch_.elapsed_ms() > 1000 )
    {
        watch_.start();
        return dispatch( handler, count );
    }

    return false;
}

template< class T, class H >
inline void dispatch( subscription< T >* s, H* h, size_t b )
{
    yield_wait wait;
    batch_dispatch< T > dispatch( *s, 1, b );

    while( 1 )
    {
        size_t n;
        bool complete = dispatch( *h, n );

        if( complete ) {
            break;
        }

        if( n == 0 ) {
            wait();
        }
    }

    s->cancel();
}
