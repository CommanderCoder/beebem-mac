//
//  apples.h
//  BeebEm
//
//  Created by Commander Coder on 01/04/2024.
//

#ifndef apples_h
#define apples_h


#include <stdio.h>
#include <stdint.h>

#include <string>

#include <sys/syslimits.h>
#include <sys/types.h>
#include <sys/stat.h>


typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value

typedef uint32_t LONG;   // LONG = unsigned 32 bit value
typedef uint32_t UINT;   // UINT = unsigned 32 bit value
typedef uint32_t UINT32; // UINT32 = unsigned 32 bit value
typedef uint8_t UINT8;   // UINT8 = unsigned 8 bit value
typedef int32_t INT;     // INT = signed 32 bit value
typedef int16_t INT16;   // INT16 = signed 16 bit value

typedef uint32_t* HWND; // 
typedef uint32_t* HINSTANCE; //

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

#define _MAX_PATH PATH_MAX
#define MAX_PATH PATH_MAX
#define _MAX_DRIVE 32
#define _MAX_DIR 512
#define _MAX_FNAME 96
#define _MAX_EXT 32
#define u_short uint16_t

#define CHAR char



// ECONET
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <unistd.h>  // NOTE this has redefinitions of 'read' and 'write'

#define SOCKET int
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKADDR sockaddr
long WSAGetLastError();

#define LPCSTR std::string
#define BYTE __uint8_t
#define WORD __uint16_t
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))


// used for Files
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define stricmp strcasecmp
#define strnicmp strncasecmp
char* _strerror(const char *strErrMsg);


// Serial
#define EVENPARITY 1
#define ODDPARITY 0
#define NOPARITY 2


extern int __argc;
extern char** __argv;


// used in BeebMem.h
extern DWORD GetTickCount();

extern int _vscprintf (const char * format, va_list pargs);

extern void _splitpath(const char *path,
				char *drive,
				char *dir,
				char *fname,
				char *ext);
extern void _makepath(char *path,
					  const char *drive,
	   const char *dir,
	   const char *fname,
	   const char *ext);

#define MOVEFILE_REPLACE_EXISTING 0
extern void MoveFileEx(const char* dest, const char* orig, DWORD flags);


bool SHGetFolderPath(const char* path);


template < typename T, int N >
int _countof( T ( & arr )[ N ] )
{
	return std::extent< T[ N ] >::value;
}


extern "C" void swift_SoundStream(BYTE* buffer, int outputType);// outputtype 1 = 8 bit, 1 channel, 2 = 16 bit, 2 channel
extern "C" void swift_SoundInit();



extern "C" void swift_GetBundleDirectory(const char* bundlePath, int length);
extern "C" void swift_GetApplicationSupportDirectory(const char* appPath, int length);
extern "C" void swift_GetResourcePath(const char* resourcePath, int length, const char* filename);
extern "C" bool swift_CopyDirectoryRecursively(const char* sourcePath, const char* targetPath);
extern "C" void swift_saveScreen(const char * filename);


extern "C" void swift_SetMenuCheck(unsigned int cmd, char check);
extern "C" void swift_SetMenuEnable(unsigned int cmd, char enable);
extern "C" int swift_SetMenuItemTextWithCString(unsigned int cmd, const char* text);
extern "C" int swift_ModifyMenu(unsigned int cmd, unsigned int newitem, const char* itemtext);

extern "C" int swift_Remove(const char* path);


#include "ExportFileDialog-mac.hpp"
extern "C" int swift_DoModal(ExportFileDialog* dialog);
extern "C" int swift_EndDialog();

// delay the next update of the cpu (i.e. Exec6502Instruction) by this accumulation of
// this time
extern "C" void swift_sleepCPU(unsigned long microseconds);

int beebwin_RC2ID(int rc);
void beebwin_ModifyMenu(
						UINT position,
						UINT newitem,
						CHAR* newtext);
void beebwin_SetMenuCheck(UINT cmd, bool check);
void beebwin_CheckMenuRadioItem(UINT first, UINT last, UINT cmd);

int beebwin_KeyUpDown(long, long,long);






extern "C" void swift_sleepCPU(unsigned long microseconds);
#define Sleep swift_sleepCPU


// also modify swift Enum
extern "C" enum FileFilter { DISC, UEFFILE, UEFSTATEFILE, IFD, KEYBOARD, DISCFILE, HARDDRIVE, PRINTFILE, ROMCFG, ANYFILE };

extern "C" int swift_GetFilesWithPreview (const char *path, int bytes, const char* directory, FileFilter exts, bool multiFiles);
extern "C" int swift_SaveFile (const char *path, int bytes, FileFilter exts);
extern "C" int swift_MoveFile (const char *src, const char *dest );

extern "C" int swift_SelectFolder (const char *path, int bytes);


extern "C" int swift_setPasteboard ( const char* clipboard, int length);
extern "C" int swift_getPasteboard ( char* clipboard, int length);

extern "C" void swift_InitDialog (char* dfsNames[], int max);
extern "C" int swift_SelectedFiles ( int fileSelected[], int max);

extern "C" int swift_Report ( const char* message, const char* title, int buttonType);

extern void beebwin_ModifyMenu(
						UINT position,
						UINT newitem,
						CHAR* newtext);




#define MB_ICONERROR 0
#define MB_ICONWARNING 1
#define MB_ICONINFORMATION 2
#define MB_ICONQUESTION 3
#define MB_NONE 0x000 //none
#define MB_YESNO 0x100
#define MB_OKCANCEL 0x200

#define IDYES 0x10
#define IDNO 0x20
#define IDOK 0x30
#define IDCANCEL 0x40
int MessageBox(HWND m_hWnd, const char* buffer, const char* WindowTitle, int Type);



#endif /* apples_h */
