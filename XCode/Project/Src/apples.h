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

#include <sys/ioctl.h>

#include "appleKeyCodes.h"


typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value

typedef uint32_t LONG;   // LONG = unsigned 32 bit value
typedef uint32_t UINT;   // UINT = unsigned 32 bit value
typedef uint32_t UINT32; // UINT32 = unsigned 32 bit value
typedef uint8_t UINT8;   // UINT8 = unsigned 8 bit value
typedef int32_t INT;     // INT = signed 32 bit value
typedef int16_t INT16;   // INT16 = signed 16 bit value

typedef float FLOAT;

typedef uint32_t ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;


#define BOOL bool

#define LPCSTR std::string
#define LPTSTR char *

typedef uint32_t UINT_PTR; //
typedef uint32_t* HWND; //  typedef uint32_t*
typedef uint32_t* HINSTANCE; //
typedef uint32_t* HDC; //
typedef uint32_t* HACCEL; //
typedef uint32_t HRESULT; //
typedef uint32_t LRESULT; //
typedef uint32_t* WNDPROC; //
typedef uint32_t* TIMERPROC; //
typedef long LPARAM;
typedef long WPARAM; //
typedef uint32_t* HGDIOBJ; //
typedef uint32_t* HMENU; //
typedef uint32_t* HBITMAP; //

typedef uint32_t* JOYCAPS; //
typedef uint32_t* POINT; //

typedef uint32_t* ISpVoice;//
typedef uint32_t ISpObjectToken;//
typedef uint32_t WCHAR;//
typedef uint32_t TCHAR;//


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


#define _MAX_PATH PATH_MAX
#define MAX_PATH PATH_MAX
#define _MAX_DRIVE 32
#define _MAX_DIR 512
#define _MAX_FNAME 96
#define _MAX_EXT 32
#define u_short uint16_t

#define CHAR char
#define WM_APP 0


// ECONET
#include <sys/socket.h>
//#include <arpa/inet.h>
struct in_addr {
  union {
	struct {
	  u_char s_b1;
	  u_char s_b2;
	  u_char s_b3;
	  u_char s_b4;
	} S_un_b;
	struct {
	  u_short s_w1;
	  u_short s_w2;
	} S_un_w;
	u_long S_addr;
  } S_un;
  u_long s_addr;
};
typedef unsigned short USHORT;
typedef struct in_addr IN_ADDR;

typedef struct sockaddr_in {
  short          sin_family;
  USHORT         sin_port;
  IN_ADDR        sin_addr;
  CHAR           sin_zero[8];
} SOCKADDR_IN, *PSOCKADDR_IN;
//#include <netdb.h>
//#include <unistd.h>  // NOTE this has redefinitions of 'read' and 'write'

#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKADDR sockaddr
#define WSAEWOULDBLOCK -1
#define ioctlsocket ioctl
#define IPPROTO_UDP -1
#define INADDR_ANY -1
#define INADDR_NONE -1
#define INADDR_BROADCAST -1
typedef struct hostent {
  char  *h_name;
  char  **h_aliases;
  short h_addrtype;
  short h_length;
  char  **h_addr_list;
} HOSTENT, *PHOSTENT, *LPHOSTENT;


long WSAGetLastError();
int closesocket(SOCKET s);
int recvfrom(int a, char * b, int c, int d, SOCKADDR *e, int*f);
int connect(int a, SOCKADDR *b, int c);
int connect(int a, sockaddr_in *b, int c);
const char* inet_ntoa(IN_ADDR in);
int inet_addr(const char* c);
int gethostname(const char*, int);
hostent* gethostbyname(const char*);
int select(int, fd_set*, int, int, const timeval*);

#define LPSTR std::string
#define LPCSTR std::string //const
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

#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))

extern int __argc;
extern char** __argv;

#define LB_ADDSTRING -1
#define LB_INSERTSTRING -1
#define LB_DELETESTRING -1
#define LB_GETTEXT -1
#define EM_SETSEL -1
#define LB_GETTEXTLEN -1
#define LB_RESETCONTENT -1
#define LB_FINDSTRING -1
#define LB_ERR -1
int SendMessage(HWND w, int a, int b, long c);
void DestroyWindow(HWND w);


// used in BeebMem.h
extern DWORD GetTickCount();

extern int _vscprintf (const char * format, va_list pargs);
int vsprintf_s(char* f, int l, const char * format, va_list pargs);

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

bool PathIsRelative(const char* path);

bool SHGetFolderPath(const char* path);



template < typename T, int N >
int _countof( T ( & arr )[ N ] )
{
	return std::extent< T[ N ] >::value;
}


extern "C" void swift_SoundStream(BYTE* buffer, int outputType);// outputtype 1 = 8 bit, 1 channel, 2 = 16 bit, 2 channel
extern "C" void swift_SoundInit();

extern "C" bool swift_IsMiniaturized();

extern "C" void swift_BreakoutBoxDialog();

extern "C" void swift_RCSetModelText(const char* n);
extern "C" int swift_RCGetSelectionMark();
extern "C" void swift_RCSetFocus();

extern "C" void swift_TCSelectItem(long item);
extern "C" void swift_TCReload();
extern "C" void swift_TCOpenDialog();
extern "C" unsigned int swift_TCGetSelected();

extern "C" void swift_DbgOpenDialog();

extern "C" void swift_GetBundleDirectory(const char* bundlePath, int length);
extern "C" void swift_GetApplicationSupportDirectory(const char* appPath, int length);
extern "C" void swift_GetResourcePath(const char* resourcePath, int length, const char* filename);
extern "C" bool swift_CopyDirectoryRecursively(const char* sourcePath, const char* targetPath);
extern "C" void swift_saveScreen(const char * filename);


extern "C" bool swift_SetMenuCheck(unsigned int cmd, char check);
extern "C" bool swift_SetMenuEnable(unsigned int cmd, char enable);
extern "C" int swift_SetMenuItemTextWithCString(unsigned int cmd, const char* text);
extern "C" int swift_ModifyMenu(unsigned int cmd, unsigned int newitem, const char* itemtext);

extern "C" int swift_Remove(const char* path);

// used in BeebWinDx.h
extern "C" void swift_SetWindowTitleWithCString(const char* title);

#include "Model.h"
extern "C" enum KB_LEDS { CASS, CAPS, SHIFT, HD0, HD1, HD2, HD3, FD0, FD1 };
extern "C" int swift_SetLED(KB_LEDS led, bool on);
extern "C" int swift_SetMachineType(Model machinetype);

void GetWindowRect(HWND h, RECT* r);

class ExportFileDialog;

extern "C" int swift_DoModalEF(ExportFileDialog* dialog); // export files
extern "C" int swift_EndDialog(bool ok);

class RomConfigDialog;
extern "C" int swift_DoModalRC(RomConfigDialog* dialog); //
extern "C" int swift_UserKeyboardDialog();
extern "C" int swift_SetCurSelRC(int m);


// delay the next update of the cpu (i.e. Exec6502Instruction) by this accumulation of
// this time
extern "C" void swift_sleepCPU(unsigned long microseconds);

extern "C" void swift_UKSetAssignedTo(const char* title);
extern "C" void swift_buttonSetControlValue(unsigned int cmd, int state);

int beebwin_RC2ID(int rc);
void beebwin_ModifyMenu(
						UINT position,
						UINT newitem,
						CHAR* newtext);
void beebwin_SetMenuCheck(UINT cmd, bool check);
void beebwin_CheckMenuRadioItem(UINT first, UINT last, UINT cmd);

int beebwin_KeyUpDown(long, long,long);



void SetWindowText(HWND m_hWnd, const char* m_szTitle);
typedef  enum
{
  DIB_RGB_COLORS = 0x00,
  DIB_PAL_COLORS = 0x01,
  DIB_PAL_INDICES = 0x02
} DIBColors;

void EndDialog(HWND m_hWnd, bool wParam);

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

extern "C" void swift_InitExportDialog (char* dfsNames[][6], int max, int columns);
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

#ifndef IDYES
#define IDYES 0x10
#define IDNO 0x20
#define IDOK 0x30
#define IDCANCEL 0x40
#endif

int MessageBox(HWND m_hWnd, const char* buffer, const char* WindowTitle, int Type);


struct bmiData;
#define BITMAPINFO bmiData
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ ppvBits);
HGDIOBJ CreateDIBSection(HDC hdc, const BITMAPINFO *pbmi,
							   UINT usage, void **ppvBits,
					 int hSection, DWORD offset);


void SetMenu(HWND w, bool s);

#define MF_CHECKED 1
#define MF_UNCHECKED 0
#define MF_GRAYED 0
#define MF_ENABLED 1


DWORD CheckMenuItem(  HMENU hMenu,  UINT  uIDCheckItem,UINT  uCheck);
DWORD EnableMenuItem(  HMENU hMenu,  UINT  uIDCheckItem,UINT  uEnable);


int GetSystemMetrics(int);
#define SM_CXSCREEN 0
#define SM_CYSCREEN 0
#define SM_CXSIZEFRAME 0
#define SM_CYSIZEFRAME 0
#define SM_CYMENUSIZE 0
#define SM_CYCAPTION 0

#define WS_OVERLAPPED 0
#define WS_CAPTION 0
#define WS_SYSMENU 0
#define WS_MINIMIZEBOX 0
#define WS_MAXIMIZEBOX 0
#define WS_SIZEBOX 0

void TextToSpeechResetState();
HMENU GetMenu(HWND);
HDC GetDC(HWND);

void ReleaseDC(HWND m_hWnd, HDC m_hDC);
void DeleteObject(HBITMAP x);
void DeleteDC(HDC m_hDCBitmap);


UINT_PTR SetTimer(HWND hWnd, UINT_PTR  nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
BOOL KillTimer(HWND hWnd, UINT_PTR  nIDEvent);


#define HWND_TOP 0
#define SWP_NOSIZE 0
#define SWP_NOMOVE 0
#define SWP_SHOWWINDOW 0
#define SWP_NOACTIVATE 0

BOOL SetWindowPos( HWND hWnd, HWND hWndInsertAfter,
        int  X,int  Y,
		int  cx,int  cy,
		UINT uFlags
);
BOOL SetWindowText(HWND    hWnd, LPCSTR lpString);

HWND SetFocus(HWND focus);


void ModifyMenu(HMENU m_hMenu, UINT pf, UINT flags, UINT_PTR newID, LPCSTR str);


#define BM_GETCHECK 0
#define BST_CHECKED 1
#define BM_SETCHECK 2
#define BST_UNCHECKED 3

LRESULT SendDlgItemMessage(
   HWND   hDlg,
   int    nIDDlgItem,
   UINT   Msg,
   WPARAM wParam,
   LPARAM lParam
);


#define BM_GETCHECK 0
#define BST_CHECKED 1
#define BM_SETCHECK 2
#define BST_UNCHECKED 3

BOOL SetDlgItemText(
  HWND   hDlg,
  int    nIDDlgItem,
  LPCSTR lpString
);

UINT GetDlgItemText(
   HWND  hDlg,
   int   nIDDlgItem,
   LPSTR lpString,
    int   cchMax
);


HWND GetDlgItem(
  HWND hDlg,
  int  nIDDlgItem
);

HWND GetFocus();


int WSACleanup();

#endif /* apples_h */
