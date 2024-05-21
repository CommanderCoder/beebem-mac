/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 2023 Chris Needham

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.
****************************************************************/

#include <windows.h>

#include "FileUtils.h"

/****************************************************************************/

bool FileExists(const char* PathName)
{
#ifdef __APPLE__
	struct stat info;

	return stat( PathName, &info ) == 0 && (info.st_mode & S_IFMT);
#else
	DWORD dwAttrib = GetFileAttributes(PathName);

	return dwAttrib != INVALID_FILE_ATTRIBUTES &&
	       !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
#endif
}

/****************************************************************************/

bool FolderExists(const char* PathName)
{
#ifdef __APPLE__
	struct stat info;

	return stat( PathName, &info ) == 0 && (info.st_mode & S_IFDIR);
#else
	DWORD dwAttrib = GetFileAttributes(PathName);

	return dwAttrib != INVALID_FILE_ATTRIBUTES &&
	       (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
#endif
}

/****************************************************************************/

std::string AppendPath(const std::string& BasePath, const std::string& Path)
{
	std::string PathName(BasePath);

	if (!PathName.empty())
	{
		char LastChar = PathName[PathName.size() - 1];

		if (LastChar != '\\' && LastChar != '/')
		{
#ifndef __APPLE__
			PathName.append(1, '\\');
#else
			PathName.append(1, '/');
#endif
		}
	}

	PathName += Path;

	return PathName;
}

/****************************************************************************/

bool HasFileExt(const char* FileName, const char* Ext)
{
	const size_t ExtLen = strlen(Ext);
	const size_t FileNameLen = strlen(FileName);

	return FileNameLen >= ExtLen &&
	       _stricmp(FileName + FileNameLen - ExtLen, Ext) == 0;
}

/****************************************************************************/

std::string ReplaceFileExt(const std::string& FileName, const char* Ext)
{
	size_t index = FileName.find_last_of(".");

	if (index == std::string::npos)
	{
		return FileName;
	}

	std::string NewFileName(FileName, 0, index);

	NewFileName += Ext;

	return NewFileName;
}

/****************************************************************************/
