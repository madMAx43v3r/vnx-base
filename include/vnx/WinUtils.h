


#ifndef INCLUDE_VNX_WINUTILS_H_
#define INCLUDE_VNX_WINUTILS_H_

#ifdef _MSC_VER 

#define NOMINMAX
#include <windows.h>
#include <iostream>
//#include <winsock2.h>

void usleep(__int64 usec);

inline int fseeko(FILE* _Stream, long _Offset, int _Origin) {
	return _fseeki64(_Stream, _Offset, _Origin);
}

inline int ftello(FILE* _Stream) {
	return _ftelli64(_Stream);
}

#endif

#endif /* INCLUDE_VNX_WINUTILS_H_ */