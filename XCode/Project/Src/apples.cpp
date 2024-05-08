//
//  apples.cpp
//  BeebEm
//
//  Created by Commander Coder on 09/04/2024.
//

#include <stdio.h>
#include <windows.h>
#include "beebemrcids.h"

#include <filesystem> // C++17 (or Microsoft-specific implementation in C++14)

int __argc;
char** __argv;

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
	return 0;
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

int SendMessage(HWND w, int a, int b, long c)
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

bool SHGetFolderPath(const char* path)
{
	// look for path
	bool folderFound = FolderExists(path);
	
	if (!folderFound)
	{
		// replace the path with the path to the users DOCUMENTS
		// folder and return 'true'
	}

	return false;
}

int connect(int a, SOCKADDR *b, int c)
{
	return 0;
}
int connect(int a, sockaddr_in *b, int c)
{
	return 0;
}
int recvfrom(int a, char * b, int c, int d, SOCKADDR *e, int*f)
{
	return 0;
}
const char* inet_ntoa(IN_ADDR in){
	return "";
}
int inet_addr(const char* c)
{
	return 0;
}
int gethostname(const char*, int){
	return 0;
}
hostent* gethostbyname(const char*)
{
	return 0;
}
int select(int, fd_set*, int, int, const timeval*)
{
	return 0;
}

int closesocket(SOCKET s)
{
//	close(s);
	return 0;
}

long WSAGetLastError()
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



void SetWindowText(HWND m_hWnd, const char* m_szTitle)
{
	// set the window title via swift
	swift_SetWindowTitleWithCString(m_szTitle);
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


void EndDialog(HWND m_hWnd, bool wParam)
{
	swift_EndDialog(wParam);
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
BOOL SetWindowText(HWND    hWnd, LPCSTR lpString) {return false;}

HWND SetFocus(HWND focus) {return 0;}


void ModifyMenu(HMENU m_hMenu, UINT pf, UINT flags, UINT_PTR newID, LPCSTR str)
{

}

LRESULT SendDlgItemMessage(
   HWND   hDlg,
   int    nIDDlgItem,
   UINT   Msg,
   WPARAM wParam,
   LPARAM lParam
)
{
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

