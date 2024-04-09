//
//  windows.h
//  BeebEm
//
//  Created by Commander Coder on 01/04/2024.
//

#ifndef windows_h
#define windows_h





// general use
#include "winints.h"

// used in BeebMem.h
extern DWORD GetTickCount();

// used for Files
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define _strerror(x) strerror(errno)


template < typename T, int N >
int _countof( T ( & arr )[ N ] )
{
	return std::extent< T[ N ] >::value;
}

extern int _vscprintf (const char * format, va_list pargs);

#define Sleep swift_sleepCPU
extern "C" void swift_sleepCPU(unsigned long microseconds);

// Serial
#define EVENPARITY 1
#define ODDPARITY 0
#define NOPARITY 2



#endif /* windows_h */
