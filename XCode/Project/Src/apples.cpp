//
//  apples.cpp
//  BeebEm
//
//  Created by Commander Coder on 09/04/2024.
//

#include <stdio.h>
#include <windows.h>
#include "beebemrcids.h"
#include <string>
#include <dispatch/dispatch.h>
#include <dispatch/source.h>

#if ((defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 101500))
#include <filesystem> // C++17 (or Microsoft-specific implementation in C++14)
#define HasC17

#endif

#include "BeebWin.h"
#include "Main.h"
#include "Thread.h"
#include "Messages.h"

int __argc;
char** __argv;


void GetLocalTime(SYSTEMTIME* _t)
{
	std::time_t result = std::time(nullptr);
	_t = std::localtime(&result);
}



DWORD GetTickCount() // milliseconds
{
	auto since_epoch = std::chrono::steady_clock::now().time_since_epoch();
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
	auto millis = milli.count();
	return (DWORD) millis;
}


void beebwin_ModifyMenu(
						UINT position,
						UINT newitem,
						LPCSTR newtext)
{
	auto id = ConvRC2ID(position);
	if (id>0)
		// check the selected item
		swift_ModifyMenu(id, ConvRC2ID(newitem), newtext);
}


void ModifyMenu(HMENU m_hMenu, UINT pf, UINT flags, UINT_PTR newID, LPCSTR str)
{
	beebwin_ModifyMenu(pf,
					   newID,
					   str);
}

// set the tick on the menu with a 4 character identifier
void beebwin_SetMenuCheck(UINT cmd, bool check)
{
	auto id = ConvRC2ID(cmd);
	if (id>0)
	{
		// check the selected item
		swift_SetMenuCheck(id, check);
	}
	
}

// set the tick on the menu with a 4 character identifier
void beebwin_CheckMenuRadioItem(UINT first, UINT last, UINT cmd)
{
	for (UINT v = first; v <= last; v++)
	{
		// uncheck all the items in the 'radio'
		beebwin_SetMenuCheck(v, false);
	}
	// check the selected item
	beebwin_SetMenuCheck(cmd, true);
}


void CheckMenuRadioItem(HMENU m_hMenu,
						UINT first,
						UINT last,
						UINT cmdSelectedMenuItem,
						UINT cmd)
{
	// switch between the items - assumes first and last are consecutive and
	// that 'SelectedMenuItem' is within that range
	beebwin_CheckMenuRadioItem(
							   first,
							   last,
							   cmdSelectedMenuItem);
}


// set the tick on the menu with a 4 character identifier
void beebwin_SetDlgCheck(HWND hwnd, UINT cmd, bool check)
{
	auto id = ConvRC2ID(cmd);
	if (id>0)
	{
		// check the selected item
		swift_SetDlgCheck((Dialogs)*hwnd, id, check);
	}
	
}

void beebwin_CheckDlgRadioItem(HWND hwnd, UINT first, UINT last, UINT cmd)
{
	
	for (UINT v = first; v <= last; v++)
	{
		// uncheck all the items in the 'radio'
		beebwin_SetDlgCheck(hwnd, v, false);
	}
	// check the selected item
	beebwin_SetDlgCheck(hwnd, cmd, true);
}


void CheckRadioButton(HWND hwndDlg,
					  int first,
					  int last,
					  int checked)
{
	beebwin_CheckDlgRadioItem( hwndDlg,
							   first,
							   last,
							   checked);
}


int _vscprintf (const char * format, va_list pargs) {
	int retval;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
 }
int vsprintf_s(char* f, int l, const char * format, va_list pargs)
{
	return vsnprintf(f,l,format,pargs);
}

char* _strerror(const char *strErrMsg)
{
	static char errorstring[256];
	if (strErrMsg != NULL)
		sprintf(errorstring, "%s : %s", strErrMsg, strerror(errno));
	else
		sprintf(errorstring, "%s\n", strerror(errno));
	return errorstring;
}

int SendMessage(HWND w, int a, size_t b, long c)
{
	return 0;
}
void DestroyWindow(HWND w)
{}

void GetWindowRect(HWND h, RECT* r)
{
}

bool EnableWindow(HWND h, BOOL e)
{
    return true;
}

#include "FileUtils.h"
#include <dispatch/source.h>

void _splitpath(const char *path,
				char *drive,
				char *dir,
				char *fname,
				char *ext)
{
	char d[300];
	char n[300];
	char e[300];
	sscanf(path, "%s/%s.%s", d,n,e);
	if (dir)
		strcpy(dir, d);
	if (fname)
		strcpy(fname, n);
	if (ext)
		strcpy(ext, e);
}

void _makepath(char *path,
			   const char *drive,
	  const char *dir,
	  const char *fname,
	  const char *ext)
{
	char fpath[_MAX_PATH];
	strcpy(fpath, fname);
	if (fname && ext)
		sprintf(path, "%s/%s.%s", dir,fpath,ext);
	else
		sprintf(path, "%s/", dir);
}

//SHFOLDERAPI
int SHGetFolderPath(HWND   hwnd,
					   int    csidl,
					   HANDLE hToken,
					   DWORD  dwFlags,
					   LPSTR  pszPath)
{
	if (csidl != CSIDL_PERSONAL)
		return 1; //error
	
	char userDataPath[PATH_MAX];
	swift_GetDocumentsDirectory(userDataPath, _MAX_PATH);
	// ../Documents/
	strcat(userDataPath, "BeebEm/");
	// ../Documents/BeebEm/
	
#if 0 //def DEBUG
	// look for path
	bool folderFound = FolderExists(userDataPath);
	if (folderFound)
	{
		// during debugging need to always copy the userdata from the Bundle
		fprintf(stdout,"*** REMOVING %s SO IT WILL GET A CLEAN COPY ***", userDataPath);
		int ret = swift_Remove(userDataPath);
		fprintf(stdout,"*** %d ***", ret);
	}
#endif

	strcpy(pszPath, userDataPath);

	return S_OK;
}

LSTATUS RegSetValueEx(
HKEY       hKey,
LPCSTR     lpValueName,
				 DWORD      Reserved,
		  DWORD      dwType,
		  const BYTE *lpData,
		   DWORD      cbData
)
{
	return ERROR_SUCCESS;
}
LSTATUS RegQueryValueEx(
		HKEY    hKey,
		LPCSTR  lpValueName,
		LPDWORD lpReserved,
		LPDWORD lpType,
		LPBYTE  lpData,
		LPDWORD lpcbData
	  )
{
	std::string val(lpValueName);
	if (val.compare("UserDataFolder"))
	{
		return 1; //error
	}
	return ERROR_SUCCESS;
}
LSTATUS RegCreateKeyEx(
   HKEY                        hKey,
   LPCSTR                      lpSubKey,
   DWORD                       Reserved,
   LPSTR                       lpClass,
   DWORD                       dwOptions,
   REGSAM                      samDesired,
   const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
   PHKEY                       phkResult,
   LPDWORD                     lpdwDisposition
)
{
	return ERROR_SUCCESS;
}

LSTATUS RegOpenKeyEx(
   HKEY   hKey,
   LPCSTR lpSubKey,
   DWORD  ulOptions,
   REGSAM samDesired,
   PHKEY  phkResult
)
{
	// cannot open keys so
	return !ERROR_SUCCESS;
}
LSTATUS RegCloseKey( HKEY hKey)
{
	return ERROR_SUCCESS;
}

DWORD GetModuleFileNameA(
   HMODULE hModule,
   LPSTR   lpFilename,
   DWORD   nSize
)
{
	// get the path of the executable file of the current process.
	// return the length of the string
	return 0;
}


int WSAGetLastError()
{
	return errno;
}


int MessageBox(HWND m_hWnd, const char* buffer, const char* WindowTitle, int Type) {

	int type = Type &0xf;
	int button = (Type & 0xf00);
	int res = 0;

	if (type == MB_ICONERROR ||
		type == MB_ICONWARNING ||
		type == MB_ICONINFORMATION ||
		type == MB_ICONQUESTION )
	{
		Type = (int)type;
		const char* msgtitle[5] {"Error:","Warning:","Info:","Question:"};
		const char* buttontitles[3] {"","YesNo:","OKCancel:"};
		fprintf(stderr,"%s: %s (%s)",msgtitle[Type],buffer,buttontitles[button]);
		
		int ret = swift_Report(buffer,msgtitle[Type],button);
		if (button == MB_YESNO)
		{
			res = ret==1?IDYES:IDNO;
		}
		else if (button == MB_OKCANCEL)
		{
			res = ret==1?IDOK:IDCANCEL;
		}
	}

	return res;
}


BOOL SetWindowText(HWND    hWnd, LPTSTR lpString)
{
	return SetWindowText(hWnd, (LPCSTR) lpString);
}

BOOL SetWindowText(HWND    hWnd, LPCSTR lpString)
{
	// set the window title via swift
	swift_SetWindowTitleWithCString(lpString);
	return true;
}

bool PathIsRelative(const char * winPathString)
{
#ifdef HasC17
	std::filesystem::path path(winPathString); // Construct the path from a string.

	if (path.is_absolute()) {
		// Arriving here if winPathString = "C:/tmp".
		return false;
	}
	if (path.is_relative()) {
		// Arriving here if winPathString = "".
		// Arriving here if winPathString = "tmp".
		// Arriving here in windows if winPathString = "/tmp". (see quote below)
		return true;
	}
#else
	if (winPathString[0] != '/')
		// this is relative address as first character is NOT '/'
		return true;
#endif

	return false;
}


void SetMenu(HWND w, bool s)
{
	
}


DWORD CheckMenuItem( HMENU hMenu,  UINT  uIDCheckItem,UINT  uCheck)
{
	swift_SetMenuCheck(ConvRC2ID(uIDCheckItem), uCheck);
	return uCheck;   // should be the previous value of this item
}


DWORD EnableMenuItem(  HMENU hMenu,  UINT  uIDCheckItem,UINT  uEnable)
{
	swift_SetMenuEnable(ConvRC2ID(uIDCheckItem), uEnable);
	return uEnable;  // should be the previous value of this item
}

void _itoa(int i, CHAR* c, int l)
{
	snprintf(c, l, "%X ",i);
}

int GetSystemMetrics(int  x)
{
	// value should never be used
	return 0;
}

void TextToSpeechResetState()
{
	
}

HMENU GetMenu(HWND m)
{
	return 0;
}

HDC GetDC(HWND m)
{
	return 0;
}

void ReleaseDC(HWND m_hWnd, HDC m_hDC)
{
	
}

void DeleteObject(HBITMAP x)
{
	
}

void DeleteDC(HDC m_hDCBitmap)
{

}

dispatch_source_t kbdtimer = NULL;
dispatch_source_t boottimer = NULL;

bool createTimer(dispatch_source_t& timer, dispatch_block_t handler) {
    // Get the global dispatch queue [DEFAULT]
	dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	if (!timer)
	{
		timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
		if (!timer) return false;
		dispatch_source_set_event_handler(timer, handler);
		dispatch_resume(timer);
	}
	return true;
};

bool CreateTimers()
{
	return createTimer(kbdtimer,
                       ^{ mainWin->HandleKeyboardTimer(); }) &&
		   createTimer(boottimer,
                       ^{
                           mainWin->KillBootDiscTimer();
                           mainWin->DoShiftBreak();
                       });
}

UINT_PTR SetTimer(HWND hWnd, UINT_PTR  nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) {
	
	if (mainWin == NULL)
		return 0;
    
    if (!CreateTimers())
        return 0;
	
	if (mainWin->TIMER_PRINTER == nIDEvent)
	{
		mainWin->CopyPrinterBufferToClipboard();
		return 1;
	}
    
    dispatch_source_t timer = NULL;
    
    if (mainWin->TIMER_KEYBOARD == nIDEvent)
    {
        timer = kbdtimer;
    }
    
    if (mainWin->TIMER_AUTOBOOT_DELAY == nIDEvent)
    {
        timer = boottimer;
    }

    if (!timer)
    {
        fprintf(stderr, "Trying to set unknown timer %d for %d microsec", nIDEvent, uElapse);
        return 0;
    }

    // Set timer start time and interval
    dispatch_source_set_timer(timer,
                              dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_MSEC*uElapse),
                              DISPATCH_TIME_FOREVER /*one shot*/,
                              0);
    return 1;
}

BOOL KillTimer(HWND hWnd, UINT_PTR  nIDEvent) {
    return 0;
}

BOOL SetWindowPos( HWND hWnd, HWND hWndInsertAfter,
		int  X,int  Y,
		int  cx,int  cy,
		UINT uFlags
				  ) {return false;}

HWND SetFocus(HWND focus) {return 0;}



LRESULT SendDlgItemMessage(
   HWND   hDlg,
   int    nIDDlgItem,
   UINT   Msg,
   WPARAM wParam,
   LPARAM lParam
)
{
	
	if (Msg == BM_SETCHECK){
		swift_SetDlgCheck((Dialogs)*hDlg, nIDDlgItem, wParam);
//	e.g. 	hwndTapeControl, IDC_TAPE_CONTROL_UNLOCK
//	e.g. 	Modals::tapeControl, 'tcul'
	}

	return 0;
}

void SetDlgItemText(int nID, const std::string& str)
{
	swift_SetDlgItemText(Modals::serialPort, ConvRC2ID(nID), str.c_str());
}



BOOL SetDlgItemText(
  HWND   hDlg,
  int    nIDDlgItem,
  LPCSTR lpString
)
{
	return swift_SetDlgItemText((Dialogs)*hDlg, ConvRC2ID(nIDDlgItem), lpString);
}

UINT GetDlgItemText(
   HWND  hDlg,
   int   nIDDlgItem,
  LPSTR lpString,
	int   cchMax
)
{
	bool ok = swift_GetDlgItemText((Dialogs)*hDlg, ConvRC2ID(nIDDlgItem), lpString, cchMax);
	return ok;
}

HWND GetDlgItem(
  HWND hDlg,
  int  nIDDlgItem
)
{
	return hDlg;
}


HWND GetFocus()
{
	return 0;
}


int WSACleanup()
{
	return 0;
}

void SetEvent(HANDLE)
{
	
}


void FreeLibrary(HMODULE)
{
	
}

bool GetFullPathName(const char* c, int l, const char* p, char** f)
{
	return 0;
}

void PathCanonicalize(const char* c, const char* p)
{
	
}

void GetClientRect(HWND h, RECT* r){}
void InvalidateRect(HWND h, RECT* r, bool b){}
DWORD GetWindowLong(HWND wnd, long s){return 0;}
void SetWindowLong(HWND m_hWnd, int f, DWORD s){}



void MoveWindow(HWND m_hWnd, int x, int y, int w, int h, bool b){}
void ShowWindow(HWND m_hWnd, int x){}
void RemoveMenu(HWND w, int a, int b){}


#include <queue>
#include <mutex>


extern std::queue< std::tuple<UINT,int,long> > buffer;
extern std::mutex mtx;

void PostMessage(HWND wnd, int s, int t, long l)
{
	switch (s)
	{
		case WM_IP232_ERROR:
		{
			auto i = std::tuple<UINT,int,long>{s,t,l};
			std::lock_guard<std::mutex> lock(mtx);
			buffer.push(i);
		}
			break;
		default:
			break;
	}
}
void ShellExecute(HWND m_hWnd, void* a, char* p, void* b, void* c, int f){}


int SHCreateDirectoryEx(void* a, const char* f, void *b)
{
	// checked if Application Support/BeebEm/UserData exists
	// it doesn't so create it and copy the files
	mode_t mode = 0755;
	mkdir(f, mode);
	return ERROR_SUCCESS;
}

extern int_fast32_t m_RGB32[256];
extern int_fast16_t m_RGB16[256];


HGDIOBJ SelectObject(HDC hdc, HGDIOBJ ppvBits)
{
	return (HGDIOBJ)1;
}


HGDIOBJ CreateDIBSection(HDC hdc, const BITMAPINFO *pbmi,
							   UINT usage, void **ppvBits,
							   int hSection, DWORD offset)
{

	*ppvBits = (char *)calloc(pbmi->bmiHeader.biSizeImage,1);

	fprintf(stderr, "Base Address = %08lx\n", (unsigned long) ppvBits);
	
	// just in case usage ever changes
	int COLS = 64;
	if (usage==DIB_RGB_COLORS)
		COLS = 64;
	
	for (int i = 0; i < COLS + 4; ++i)
	{
	  m_RGB32[i] = ((( ((pbmi->bmiColors[i].rgbRed) << 8)  + (pbmi->bmiColors[i].rgbGreen )) << 8) + (pbmi->bmiColors[i].rgbBlue));
	  m_RGB32[i] |= 0xff000000;

	  m_RGB16[i] = ((( ((pbmi->bmiColors[i].rgbRed >> 3) << 5)  + (pbmi->bmiColors[i].rgbGreen >> 3)) << 5) + (pbmi->bmiColors[i].rgbBlue >> 3));

	//      printf("RGB32[%d] = %08x, RGB16[%d] = %04x\n", i, m_RGB32[i], i, m_RGB16[i]);

	}
	
	return (HGDIOBJ)1;
}

bool SHFileOperation(SHFILEOPSTRUCT* a)
{
	if (a->wFunc == FO_COPY)
	{
		// using SWIFT Foundation to copy all the files in pFrom
		a->fAnyOperationsAborted = false;
		bool res = swift_CopyDirectoryRecursively(a->pFrom, a->pTo);
		if (!res)
		{
			return 1; //error
		}
	}
	return ERROR_SUCCESS;
}
void GetMonitorInfo(HMONITOR monitor, MONITORINFO* info){}
HMONITOR MonitorFromWindow(HWND wnd, int s){return 0L;}

MMRESULT joySetCapture(HWND Wnd, int jid, int v, bool f)
{
	swift_JoystickCapture();
	return JOYERR_NOERROR;
}
MMRESULT joyGetDevCaps(int jid, JOYCAPS* jc, int l)
{
	jc->wXmin = 0;
	jc->wYmin = 0;
	jc->wXmax = 65536;
	jc->wYmax = 65536;
	return JOYERR_NOERROR;
}

void* std_ThreadFunc(void* parameter)
{
	Thread* pThread = reinterpret_cast<Thread*>(parameter);
	long res = (long)pThread->ThreadFunc();
	return (void *)res;
}


void InitializeCriticalSection(int*){}
void EnterCriticalSection(int*){}
void LeaveCriticalSection(int*){}
void DeleteCriticalSection(int*){}

int GetCurrentThreadId() {return 0;}

HHOOK SetWindowsHookEx(
  int       idHook,
  HOOKPROC  lpfn,
  HINSTANCE hmod,
  DWORD     dwThreadId
)
{
	return 0;
}
BOOL UnhookWindowsHookEx(
  HHOOK hhk
)
{
	return 0;
}

LRESULT CallNextHookEx(
   HHOOK  hhk,
   int    nCode,
   WPARAM wParam,
   LPARAM lParam
)
{
	return 0;
}
