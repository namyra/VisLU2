#ifndef REVERSEBYTES_H
#define REVERSEBYTES_H

#include <ostream>
#include <iomanip>
#include <algorithm>
/**
* A templated routine that returns a the input value in reversed byte order
*/
template < typename T > T reverseBytes( T const &arg )
{
	T nrv( arg );		
	// reinterpret_cast ..
	unsigned char *p = (unsigned char *)&nrv;
	// don't you just love namespaces ...
	std::reverse( p, p + sizeof( nrv ) );
	// Now for the UB ...
	return nrv;
}
#endif

