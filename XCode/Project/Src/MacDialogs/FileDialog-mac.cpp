//
//  FileDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 16/04/2024.
//

#include "FileDialog-mac.hpp"



// MacOS FileDialog

FileDialog::FileDialog(HWND hwndOwner, LPTSTR result, DWORD resultLength,
					   LPCTSTR initialFolder, LPCTSTR filter)
{
	memset(&m_ofn, 0, sizeof(m_ofn));

	m_ofn.lStructSize = sizeof(OPENFILENAME);
	m_ofn.lpstrFilter = filter;
	m_ofn.nFilterIndex = 1;
	m_ofn.lpstrFile = result;
	m_ofn.nMaxFile = resultLength;
	m_ofn.lpstrInitialDir = initialFolder;
//	m_ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	multiSelect = false;
	
	/* parse the filter for DISC, UEF, etc */
	if (strstr(filter,"ssd")!=0)
	{
		fileFilter = DISC;
	}
	else if (strstr(filter,"UEF State")!=0) //  uefstate
	{
		fileFilter = UEFSTATEFILE;
	}
	else if (strstr(filter,"uef")!=0)
	{
		fileFilter = UEFFILE;
	}
	else if (strstr(filter,"Printer")!=0)
	{
		fileFilter = PRINTFILE;
	}
	else if (strstr(filter,"kmap")!=0)
	{
		fileFilter = KEYBOARD;
	}
	else if (strstr(filter,"Hard Drive")!=0)
	{
		fileFilter = HARDDRIVE;
	}
	else if (strstr(filter,"INF files")!=0)
	{
		fileFilter = DISCFILE;
	}
	else if (strstr(filter,".rom")!=0)
	{
		fileFilter = ANYFILE;
	}
	else if (strstr(filter,".cfg")!=0)
	{
		fileFilter = ROMCFG;
	}
	else if (strstr(filter,"All Files")!=0)
	{
		fileFilter = ANYFILE;
	}
	else
	{
		// not recognised
	}
}

bool FileDialog::ShowDialog(bool open)
{
	if (open)
	{
//		return GetOpenFileName(&m_ofn) != 0;
		return true;
	}
	else
	{
//		m_ofn.Flags |= OFN_OVERWRITEPROMPT;
//
//		return GetSaveFileName(&m_ofn) != 0;
		return true;
	}
}
