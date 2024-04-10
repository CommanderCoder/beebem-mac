//
//  winints.cpp
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
