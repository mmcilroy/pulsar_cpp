template< class T >
inline queue< T >::queue( size_t n ) :
    _data( new T[n] ),
    _size( n ),
    _mask( n-1 )
{
}

template< class T >
inline size_t queue< T >::size() const
{
    return _size;
}

template< class T >
inline T& queue< T >::at( position p )
{
    return _data[ p & _mask ];
}
