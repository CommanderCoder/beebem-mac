//
//  winints.cpp
//  BeebEm
//
//  Created by Commander Coder on 09/04/2024.
//

#include <stdio.h>
#include <windows.h>
#include "beebemrcids.h"
#include "FileUtils.h"

int __argc;
char** __argv;

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
						CHAR* newtext)
{
	auto cmdID = RC2ID.find(position);
	auto newID = RC2ID.find(newitem);
	if (cmdID != RC2ID.end())
	{
		// check the selected item
		swift_ModifyMenu(cmdID->second, newID->second, newtext);
	}

}

int _vscprintf (const char * format, va_list pargs) {
	int retval;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
 }


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
	if (fname && ext)
		sprintf(path, "%s/%s.%s", dir,fname,ext);
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
