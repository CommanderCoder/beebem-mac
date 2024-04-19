//
//  apples.cpp
//  BeebEm
//
//  Created by Commander Coder on 09/04/2024.
//

#include <stdio.h>
#include <windows.h>
#include "beebemrcids.h"

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

long WSAGetLastError()
{
	return errno;
}
