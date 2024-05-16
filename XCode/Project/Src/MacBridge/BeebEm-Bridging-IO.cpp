//
//  BeebEm-Bridging-IO.cpp
//  BeebEm
//
//  Created by Commander Coder on 16/04/2024.
//

#include <windows.h>

/*
 IO Responses to state changes in WinIO
 - Setting name of 'eject disc 0 : filename' if DiscLoaded[Drive] == true
 - Grey out 'eject disc' if DiscLoaded[Drive] == false

 
 */


void MoveFileEx(const char* dest, const char* orig, DWORD flags)
{
	// flags ignored
	swift_MoveFile(dest, orig);
}
