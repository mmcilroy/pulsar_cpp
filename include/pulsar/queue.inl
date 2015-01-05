template< class T >
inline queue< T >::queue( size_t n ) :
    data_( new T[n] ),
    size_( n ),
    mask_( n-1 )
{
}

template< class T >
inline size_t queue< T >::size() const
{
    return size_;
}

template< class T >
inline T& queue< T >::at( position p )
{
    return data_[ p & mask_ ];
}
