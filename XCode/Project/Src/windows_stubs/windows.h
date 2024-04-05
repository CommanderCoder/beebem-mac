//
//  windows.h
//  BeebEm
//
//  Created by Commander Coder on 01/04/2024.
//

#ifndef windows_h
#define windows_h

#ifdef __APPLE__





// used in BeebMem.h
extern long GetTickCount();

// general use
#include "winints.h"

// used for Files
#include <sys/types.h>
#include <sys/stat.h>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define stricmp strcasecmp
#define strnicmp strncasecmp

template < typename T, int N >
int _countof( T ( & arr )[ N ] )
{
	return std::extent< T[ N ] >::value;
}

#define Sleep swift_sleepCPU
extern "C" void swift_sleepCPU(unsigned long microseconds);



#endif



#endif /* windows_h */
