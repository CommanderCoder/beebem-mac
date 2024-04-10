//
//  winints.h
//  BeebEm
//
//  Created by Commander Coder on 01/04/2024.
//

#ifndef winints_h
#define winints_h


#include <stdint.h>
#include <string>
typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value

typedef uint32_t LONG;   // LONG = unsigned 32 bit value
typedef uint32_t UINT;   // UINT = unsigned 32 bit value
typedef uint32_t UINT32; // UINT32 = unsigned 32 bit value
typedef uint8_t UINT8;   // UINT8 = unsigned 8 bit value
typedef int32_t INT;     // INT = signed 32 bit value
typedef int16_t INT16;   // INT16 = signed 16 bit value

#define u_short uint16_t

#define CHAR char

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} RGBQUAD;


// from Carbon
struct RECT {
  short               top;
  short               left;
  short               bottom;
  short               right;
};
typedef struct RECT                     RECT;

// 16 rows and 2 columns (only column 0 needs the bank value)
struct RCItem {
	int bank;
	std::string name;
};

#include <sys/syslimits.h>
#define _MAX_PATH PATH_MAX
#define MAX_PATH PATH_MAX
#define _MAX_DRIVE 32
#define _MAX_DIR 512
#define _MAX_FNAME 96
#define _MAX_EXT 32


// ECONET
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <unistd.h>  // NOTE this has redefinitions of 'read' and 'write'

#define SOCKET int
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
static long WSAGetLastError(){return errno;}
static void WSACleanup(){}
#define SOCKADDR sockaddr

#define LPCSTR std::string
#define BYTE __uint8_t
#define WORD __uint16_t
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))


extern "C" void swift_GetBundleDirectory(const char* bundlePath, int length);
extern "C" void swift_GetApplicationSupportDirectory(const char* appPath, int length);
extern "C" void swift_GetResourcePath(const char* resourcePath, int length, const char* filename);
extern "C" bool swift_CopyDirectoryRecursively(const char* sourcePath, const char* targetPath);
extern "C" void swift_saveScreen(const char * filename);


extern "C" void swift_SetMenuCheck(unsigned int cmd, char check);
extern "C" void swift_SetMenuEnable(unsigned int cmd, char enable);
extern "C" int swift_SetMenuItemTextWithCString(unsigned int cmd, const char* text);
extern "C" int swift_ModifyMenu(unsigned int cmd, unsigned int newitem, const char* itemtext);

// delay the next update of the cpu (i.e. Exec6502Instruction) by this accumulation of
// this time
extern "C" void swift_sleepCPU(unsigned long microseconds);



#endif /* winints_h */
