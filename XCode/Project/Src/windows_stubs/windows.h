//
//  windows.h
//  BeebEm
//
//  Created by Commander Coder on 01/04/2024.
//

#ifndef windows_h
#define windows_h

#ifdef __APPLE__

#include <sys/syslimits.h>
#define _MAX_PATH PATH_MAX
#define MAX_PATH PATH_MAX
#define _MAX_DRIVE 32
#define _MAX_DIR 512
#define _MAX_FNAME 96
#define _MAX_EXT 32




// used in BeebMem.h
extern long GetTickCount();


#endif



#endif /* windows_h */
