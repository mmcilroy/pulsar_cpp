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
inline size_t batch_dispatch< T >::operator()( H& handler )
{
    size_t dispatch_count = 0;

    if( available_ < min_ ) {
        available_ = sub_.available();
    }

    if( available_ >= min_ )
    {
        dispatch_count = std::min( available_, max_ );
        for( size_t i=0; i<dispatch_count; i++ ) {
            handler.on_next( sub_.at( i ), --available_ );
        }

        sub_.commit( dispatch_count );
    }

    return dispatch_count;
}

template< class T >
inline periodic_dispatch< T >::periodic_dispatch( size_t millis ) :
    millis_( millis )
{
}

template< class T >
template< class H, class D >
inline size_t periodic_dispatch< T >::operator()( D& dispatch, H& handler )
{
    if( watch_.elapsed_ms() > 1000 )
    {
        watch_.start();
        return dispatch( handler );
    }

    return 0;
}
