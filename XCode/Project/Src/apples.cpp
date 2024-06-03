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
#include <filesystem> // C++17 (or Microsoft-specific implementation in C++14)

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

int beebwin_RC2ID(int rc)
{
	auto cmdID = RC2ID.find(rc);
	if (cmdID != RC2ID.end())
		return cmdID->second;
	return -1;
}

void beebwin_ModifyMenu(
						UINT position,
						UINT newitem,
						CHAR* newtext)
{
	auto id = beebwin_RC2ID(position);
	if (id>0)
		// check the selected item
		swift_ModifyMenu(id, beebwin_RC2ID(newitem), newtext);
}

void ModifyMenu(HMENU m_hMenu, UINT pf, UINT flags, UINT_PTR newID, CHAR* str)
{
	beebwin_ModifyMenu(pf,
					   newID,
					   str);
}

// set the tick on the menu with a 4 character identifier
void beebwin_SetMenuCheck(UINT cmd, bool check)
{
	auto id = beebwin_RC2ID(cmd);
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

#include "FileUtils.h"

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
#if 0
	// grab the application support folder
	swift_GetApplicationSupportDirectory(userDataPath, _MAX_PATH);
	// ../library/application support/beebem-mac
	strcat(userDataPath, "UserData/");
	// ../library/application support/beebem-mac/UserData
#else
	swift_GetDocumentsDirectory(userDataPath, _MAX_PATH);
	// ../Documents/
	strcat(userDataPath, "BeebEm/");
	// ../Documents/BeebEm/
#endif
	
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
int connect(int a, SOCKADDR *b, int c)
{
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
	return false;
}


void SetMenu(HWND w, bool s)
{
	
}



DWORD CheckMenuItem( HMENU hMenu,  UINT  uIDCheckItem,UINT  uCheck)
{
	if (uIDCheckItem == ID_FDC_DLL)
	{
		swift_SetMenuCheck(beebwin_RC2ID(ID_FDC_ACORN), uCheck);
		swift_SetMenuCheck(beebwin_RC2ID(ID_FDC_OPUS), uCheck);
		swift_SetMenuCheck(beebwin_RC2ID(ID_FDC_WATFORD), uCheck);
	}

	swift_SetMenuCheck(beebwin_RC2ID(uIDCheckItem), uCheck);
	return uCheck;   // should be the previous value of this item
}


DWORD EnableMenuItem(  HMENU hMenu,  UINT  uIDCheckItem,UINT  uEnable)
{
	
	if (uIDCheckItem == ID_FDC_DLL)
	{
		swift_SetMenuEnable(beebwin_RC2ID(ID_FDC_ACORN), uEnable);
		swift_SetMenuEnable(beebwin_RC2ID(ID_FDC_OPUS), uEnable);
		swift_SetMenuEnable(beebwin_RC2ID(ID_FDC_WATFORD), uEnable);
	}

	swift_SetMenuEnable(beebwin_RC2ID(uIDCheckItem), uEnable);
	
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

UINT_PTR SetTimer(HWND hWnd, UINT_PTR  nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) {return 0;}

BOOL KillTimer(HWND hWnd, UINT_PTR  nIDEvent) {return 0;}

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

BOOL SetDlgItemText(
  HWND   hDlg,
  int    nIDDlgItem,
  LPCSTR lpString
)
{
	return 0;
}

UINT GetDlgItemText(
   HWND  hDlg,
   int   nIDDlgItem,
  LPSTR lpString,
	int   cchMax
)
{
	return 0;
}

HWND GetDlgItem(
  HWND hDlg,
  int  nIDDlgItem
)
{
	return 0;
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
long GetWindowLong(HWND wnd, long s){return 0L;}
void SetWindowLong(HWND m_hWnd, int f, long s){}



void MoveWindow(HWND m_hWnd, int x, int y, int w, int h, bool b){}
void ShowWindow(HWND m_hWnd, int x){}
void RemoveMenu(HWND w, int a, int b){}


void PostMessage(HWND wnd, int s, int t, long l){}
void ShellExecute(HWND m_hWnd, void* a, char* p, void* b, void* c, int f){}


int SHCreateDirectoryEx(void* a, const char* f, void *b)
{
	// checked if Application Support/BeebEm/UserData exists
	// it doesn't so create it and copy the files
	mode_t mode = 0755;
	mkdir(f, mode);
	return ERROR_SUCCESS;
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
MMRESULT joySetCapture(HWND Wnd, int jid, int v, bool f){return 0;}
MMRESULT joyGetDevCaps(int jid, JOYCAPS* jc, int l){return 0;}
