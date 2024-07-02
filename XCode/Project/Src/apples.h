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

#include <string> // c++ std::string

#include <sys/syslimits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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


typedef uint32_t UINT_PTR; //
typedef uint32_t* HWND; //  typedef uint32_t*
typedef uint32_t* HINSTANCE; //
typedef uint32_t* HMODULE; //
typedef uint32_t* HDC; //
typedef uint32_t* HACCEL; //
typedef uint32_t HRESULT; //
typedef uint32_t LRESULT; //
typedef uint32_t MMRESULT; //
typedef uint32_t HMONITOR; //

typedef uint32_t* WNDPROC; //
typedef uint32_t* TIMERPROC; //
typedef uint32_t* HANDLE; //
typedef HANDLE HKEY;
typedef long LPARAM;
typedef long WPARAM; //
typedef uint32_t* HGDIOBJ; //
typedef uint32_t* HMENU; //
typedef uint32_t* HBITMAP; //

typedef struct tagPOINT {
  LONG x;
  LONG y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;

#define CSIDL_PERSONAL -1
#define SHGFP_TYPE_CURRENT -1

typedef int CRITICAL_SECTION;
extern void InitializeCriticalSection(int*);
extern void EnterCriticalSection(int*);
extern void LeaveCriticalSection(int*);
extern void DeleteCriticalSection(int*);

#define HKEY_CURRENT_USER NULL
#define CFG_REG_KEY NULL
#define NOERROR 0
#define S_OK 0
#define ERROR_SUCCESS 0
#define KEY_ALL_ACCESS -1
#define REG_SZ 4
#define REG_BINARY 3
typedef unsigned char BYTE;
typedef BYTE *PBYTE;
typedef BYTE *LPBYTE;
//typedef CHAR *LPSTR;
typedef char *LPSTR;
//typedef __nullterminated CONST CHAR *LPCSTR;
typedef const char *LPCSTR;
#define LPTSTR char *

typedef char REGSAM;
typedef char LPSECURITY_ATTRIBUTES;
typedef HKEY *PHKEY;
typedef DWORD *LPDWORD;

typedef uint32_t LSTATUS; //
LSTATUS RegSetValueEx(
HKEY       hKey,
LPCSTR     lpValueName,
				 DWORD      Reserved,
          DWORD      dwType,
          const BYTE *lpData,
           DWORD      cbData
);
LSTATUS RegQueryValueEx(
		HKEY    hKey,
		LPCSTR  lpValueName,
		LPDWORD lpReserved,
		LPDWORD lpType,
		LPBYTE  lpData,
		LPDWORD lpcbData
	  );
LSTATUS RegCloseKey( HKEY hKey);
LSTATUS RegCreateKeyEx(
   HKEY                        hKey,
   LPCSTR                      lpSubKey,
   DWORD                       Reserved,
   LPSTR                       lpClass,
   DWORD                       dwOptions,
   REGSAM                      samDesired,
   const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
   PHKEY                       phkResult,
   LPDWORD                     lpdwDisposition);
LSTATUS RegOpenKeyEx(
   HKEY   hKey,
   LPCSTR lpSubKey,
   DWORD  ulOptions,
   REGSAM samDesired,
   PHKEY  phkResult
);

typedef uint32_t* ISpVoice;//
typedef uint32_t ISpObjectToken;//
typedef wchar_t WCHAR;//
typedef uint32_t TCHAR;//

DWORD GetModuleFileName(
   HMODULE hModule,
   LPSTR   lpFilename,
   DWORD   nSize
);

typedef struct joycaps_tag {
  WORD wMid;
  WORD wPid;
//  char szPname[MAXPNAMELEN];
  UINT wXmin;
  UINT wXmax;
  UINT wYmin;
  UINT wYmax;
  UINT wZmin;
  UINT wZmax;
  UINT wNumButtons;
  UINT wPeriodMin;
  UINT wPeriodMax;
  UINT wRmin;
  UINT wRmax;
  UINT wUmin;
  UINT wUmax;
  UINT wVmin;
  UINT wVmax;
  UINT wCaps;
  UINT wMaxAxes;
  UINT wNumAxes;
  UINT wMaxButtons;
//  char szRegKey[MAXPNAMELEN];
//  char szOEMVxD[MAX_JOYSTICKOEMVXDNAME];
} JOYCAPS, *PJOYCAPS, *NPJOYCAPS, *LPJOYCAPS;

#define JOYERR_NOERROR -1
#define JOYERR_UNPLUGGED 2
#define JOYSTICKID1 1
MMRESULT joySetCapture(HWND Wnd, int jid, int v, bool f);
MMRESULT joyGetDevCaps(int jid, JOYCAPS* jc, int l);




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


#define GWL_STYLE -1
#define MONITOR_DEFAULTTONEAREST -1
HMONITOR MonitorFromWindow(HWND wnd, int s);
typedef struct tagMONITORINFO {
  DWORD cbSize;
  RECT  rcMonitor;
  RECT  rcWork;
  DWORD dwFlags;
} MONITORINFO, *LPMONITORINFO;

void GetMonitorInfo(HMONITOR monitor, MONITORINFO* info);

#define _MAX_PATH PATH_MAX
#define MAX_PATH PATH_MAX
#define _MAX_DRIVE 32
#define _MAX_DIR 512
#define _MAX_FNAME 96
#define _MAX_EXT 32
#define u_short uint16_t

#define CHAR char
#define WM_APP 0

#define EXPORT ;

#define ZeroMemory(Destination,Length) RtlZeroMemory((Destination),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))

#define SYSTEMTIME struct tm
void GetLocalTime(SYSTEMTIME* _t);
#define wYear tm_year
#define wMonth tm_mon
#define wDay tm_mday
#define wHour tm_hour
#define wMinute tm_min
#define wSecond tm_sec
#define wMilliseconds tm_sec // no milliseconds
#define wDayOfWeek tm_wday

// ECONET
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
struct in_addr_econet {
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
	in_addr_t S_addr;
  } S_un;
  u_long s_addr;
};
typedef unsigned short USHORT;
typedef struct in_addr IN_ADDR;

#define ioctlsocket ioctl
#define closesocket close

#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKADDR sockaddr
#define WSAEWOULDBLOCK -1
#define IPPROTO_UDP             17              /* user datagram protocol */
#define INADDR_NONE             0xffffffff              /* -1 return */
#define INADDR_ANY              (u_int32_t)0x00000000
#define INADDR_BROADCAST        (u_int32_t)0xffffffff   /* must be masked */


int WSAGetLastError();


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
#define MAKELPARAM(wLow, wHigh) (((DWORD)wHigh << 16) | ((DWORD)wLow & 0xffff))

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
int SendMessage(HWND w, int a, size_t b, long c);
void DestroyWindow(HWND w);
void ModifyMenu(HMENU m_hMenu, UINT pf, UINT flags, UINT_PTR newID, LPCSTR str);

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

int SHGetFolderPath(HWND   hwnd,
					 int    csidl,
					 HANDLE hToken,
					 DWORD  dwFlags,
					 LPSTR  pszPath);

template < typename T, int N >
int _countof( T ( & arr )[ N ] )
{
	return std::extent< T[ N ] >::value;
}

enum Dialogs {
   breakoutBox = 256
   , tapeControl
   , debugWindow
   , teletextSelect
};

enum Modals {
   keyboardLinks = 0
   , selectKey
   , romConfig
   , exportFiles
   , keyboardMapping
   , serialPort
};


extern "C" bool swift_IsMiniaturized();
extern "C" bool swift_JoystickCapture();

extern "C" void swift_SetModelText(Modals m, const char* n);
extern "C" int swift_GetSelectionMark(Modals m);
extern "C" void swift_SetFocus(Modals m);

extern "C" void swift_TCSelectItem(long item);
extern "C" void swift_TCReload();
extern "C" unsigned int swift_TCGetSelected();

extern "C" void swift_GetBundleDirectory(const char* bundlePath, int length);
extern "C" void swift_GetApplicationSupportDirectory(const char* appPath, int length);
extern "C" void swift_GetResourcePath(const char* resourcePath, int length, const char* filename);
extern "C" void swift_GetDocumentsDirectory(const char* appPath, int length);

extern "C" bool swift_CopyDirectoryRecursively(const char* sourcePath, const char* targetPath);
extern "C" void swift_saveScreen(const char * filename);


extern "C" bool swift_SetMenuCheck(unsigned int cmd, char check);
extern "C" bool swift_SetMenuEnable(unsigned int cmd, char enable);
extern "C" int swift_SetMenuItemTextWithCString(unsigned int cmd, const char* text);
extern "C" int swift_ModifyMenu(unsigned int cmd, unsigned int newitem, const char* itemtext);


extern "C" bool swift_SetDlgCheck(unsigned short dlg, unsigned int cmd, char check);
extern "C" bool swift_GetDlgCheck(unsigned short dlg, unsigned int cmd);

extern "C" bool swift_SetDlgItem(unsigned short dlg, unsigned int cmd, bool enabled);



extern "C" bool swift_SetDlgItemText(unsigned short dlg, unsigned int cmd, const char* text);
extern "C" bool swift_GetDlgItemText(unsigned short dlg, unsigned int cmd, const char* text, int len);

extern "C" int swift_Remove(const char* path);

// used in BeebWinDx.h
extern "C" void swift_SetWindowTitleWithCString(const char* title);

#include "Model.h"
extern "C" enum KB_LEDS { CASS, CAPS, SHIFT, HD0, HD1, HD2, HD3, FD0, FD1 };
extern "C" int swift_SetLED(KB_LEDS led, bool on);
extern "C" int swift_SetMachineType(Model machinetype);

#define SIZE_RESTORED -1
#define SIZE_MINIMIZED -1
void GetWindowRect(HWND h, RECT* r);
void GetClientRect(HWND h, RECT* r);
void InvalidateRect(HWND h, RECT* r, bool b);

void CheckRadioButton(HWND hwndDlg,
					  int first,
					  int last,
					  int checked);


//#define IDD_SELECT_KEY			Modals::selectKey
//#define IDD_USERKYBRD			Modals::keyboardMapping

//#define IDD_BREAKOUT			Dialogs::breakoutBox
//#define IDD_TAPECONTROL			Dialogs::tapeControl
//#define IDD_DEBUG				Dialogs::debugWindow


extern "C" int swift_SetCurSel(Modals mod, int m);


extern "C" int swift_OpenDialog(Dialogs dlg, void* dialogClass);
extern "C" int swift_CloseDialog(Dialogs dlg);
extern "C" int swift_DoModal(Modals mod, void* dialogClass);
extern "C" int swift_EndModal(bool ok);


extern "C" void swift_UKSetAssignedTo(const char* title);
extern "C" void swift_buttonSetControlValue(unsigned int cmd, int state);

void beebwin_ModifyMenu(
						UINT position,
						UINT newitem,
						LPCSTR newtext);
void beebwin_SetMenuCheck(UINT cmd, bool check);
void beebwin_CheckMenuRadioItem(UINT first, UINT last, UINT cmd);

int beebwin_KeyUpDown(long, long,long);

#define WS_POPUP -1
#define WS_OVERLAPPEDWINDOW -1
#define WIN_STYLE -1
#define WM_CLOSE 0x0010
#define SW_MAXIMIZE -1
#define SW_RESTORE -1
#define SW_SHOWNORMAL -1
DWORD GetWindowLong(HWND wnd, long s);
void SetWindowLong(HWND m_hWnd, int f, DWORD s);
BOOL SetWindowText(HWND    hWnd, LPCSTR lpString);
BOOL SetWindowText(HWND    hWnd, LPTSTR lpString);
typedef  enum
{
  DIB_RGB_COLORS = 0x00,
  DIB_PAL_COLORS = 0x01,
  DIB_PAL_INDICES = 0x02
} DIBColors;
#define BI_RGB 255
void MoveWindow(HWND m_hWnd, int x, int y, int w, int h, bool b);
void ShowWindow(HWND m_hWnd, int x);


void PostMessage(HWND wnd, int s, int t, long l);
void ShellExecute(HWND m_hWnd, void* a, char* p, void* b, void* c, int f);


int SHCreateDirectoryEx(void* a, const char* f, void *b);


// delay the next update of the cpu (i.e. Exec6502Instruction) by this accumulation of
// this time
extern "C" void swift_sleepCPU(unsigned long microseconds);
#define Sleep swift_sleepCPU
extern "C" void swift_sleepThread(unsigned long microseconds);


extern "C" int swift_GetFilesWithPreview (const char *path, int bytes, const char* directory, bool multiFiles, const char *filter);
extern "C" int swift_SaveFile (const char *path, int bytes, const char *filter, const char *directory);
extern "C" int swift_MoveFile (const char *src, const char *dest );

extern "C" int swift_SelectFolder (const char *path, int bytes, const char* title);


extern "C" int swift_setPasteboard ( const char* clipboard, long length);
extern "C" bool swift_getPasteboard ( char* clipboard, long length);

extern "C" void swift_InitExportDialog (char* dfsNames[][6], int max, int columns);
extern "C" int swift_SelectedFiles ( int fileSelected[], int max);

extern "C" int swift_Report ( const char* message, const char* title, int buttonType);


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

#define ID_FDC_ACORN                      39901
#define ID_FDC_OPUS                       39902
#define ID_FDC_WATFORD                    39903


int MessageBox(HWND m_hWnd, const char* buffer, const char* WindowTitle, int Type);


struct bmiData;
#define BITMAPINFO bmiData
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ ppvBits);
HGDIOBJ CreateDIBSection(HDC hdc, const BITMAPINFO *pbmi,
							   UINT usage, void **ppvBits,
					 int hSection, DWORD offset);


void SetMenu(HWND w, bool s);
void RemoveMenu(HWND w, int a, int b);
#define MF_CHECKED 1
#define MF_UNCHECKED 0
#define MF_GRAYED 0
#define MF_ENABLED 1

#define MF_BYCOMMAND -1

DWORD CheckMenuItem(  HMENU hMenu,  UINT  uIDCheckItem,UINT  uCheck);
DWORD EnableMenuItem(  HMENU hMenu,  UINT  uIDCheckItem,UINT  uEnable);

void _itoa(int i, CHAR* c, int l);

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

HWND SetFocus(HWND focus);


void CheckMenuRadioItem(HMENU m_hMenu,
						UINT first,
						UINT last,
						UINT cmdSelectedMenuItem,
						UINT cmd);


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


DWORD EnableDlgItem(HWND hwnd, UINT nIDDlgItem, bool Enable);

HWND GetFocus();

int WSACleanup();

void SetEvent(HANDLE);

void FreeLibrary(HMODULE);


bool GetFullPathName(const char* c, int l, const char* p, char** f);
void PathCanonicalize(const char* c, const char* p);

typedef char* PCZZSTR;//
typedef uint32_t LPVOID;//
typedef uint32_t PCSTR;//
typedef uint32_t FILEOP_FLAGS;//
#define FO_COPY -1

typedef struct _SHFILEOPSTRUCTA {
  HWND         hwnd;
  UINT         wFunc;
  PCZZSTR      pFrom;
  PCZZSTR      pTo;
  FILEOP_FLAGS fFlags;
  BOOL         fAnyOperationsAborted;
  LPVOID       hNameMappings;
  PCSTR        lpszProgressTitle;
} SHFILEOPSTRUCTA, *LPSHFILEOPSTRUCTA, SHFILEOPSTRUCT ;

bool SHFileOperation(SHFILEOPSTRUCT* a);


extern "C" int swift_ReleaseSoundBuffer(int);
extern "C" int swift_CreateSoundBuffer(WORD, DWORD, DWORD, WORD, WORD);
extern "C" void swift_StopStream(int);
extern "C" void swift_PlayStream(int);
extern "C" int  swift_BufferedStreams(int);

extern "C" void swift_SoundInit();
extern "C" void swift_SubmitStream(int outputType, BYTE* buffer, int size);
// index: outputtype 1 = 8 bit, 1 channel, 2 = 16 bit, 2 channel


#define WM_COMMAND_def 273

#define WM_INPUT 255
#define WM_MOUSEMOVE 512
#define WM_LBUTTONDOWN 513
#define WM_LBUTTONUP 514
#define WM_LBUTTONDBLCLK 515
#define WM_RBUTTONDOWN 516
#define WM_RBUTTONUP 517
#define WM_RBUTTONDBLCLK 518
#define WM_MBUTTONDOWN 519
#define WM_MBUTTONUP 520
#define WM_MBUTTONDBLCLK 521


#define MM_JOY1MOVE 0x3A0
#define MM_JOY2MOVE 0x3A1
#define MM_JOY1ZMOVE 0x3A2
#define MM_JOY2ZMOVE 0x3A3
#define MM_JOY1BUTTONDOWN 0x3B5
#define MM_JOY2BUTTONDOWN 0x3B6
#define MM_JOY1BUTTONUP 0x3B7
#define MM_JOY2BUTTONUP 0x3B8

#define JOY_BUTTON1         0x0001
#define JOY_BUTTON2         0x0002
#define JOY_BUTTON3         0x0004
#define JOY_BUTTON4         0x0008


#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

#define MK_LBUTTON 0x001
#define MK_RBUTTON 0x002
#define MK_MBUTTON 0x010

void* std_ThreadFunc(void* parameter);

#endif /* apples_h */
