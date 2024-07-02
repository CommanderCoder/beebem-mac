//
//  FileDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 16/04/2024.
//

#ifndef FileDialog_mac_hpp
#define FileDialog_mac_hpp

#include <stdio.h>

#include <windows.h>

#define LPTSTR char *
#define LPCTSTR const char *


struct OPENFILENAME
{
	/*
	Type OPENFILENAME
		lStructSize As Long
		hwndOwner As Long
		hInstance As Long
		lpstrFilter As String
		lpstrCustomFilter As String
		nMaxCustomFilter As Long
		nFilterIndex As Long
		lpstrFile As String
		nMaxFile As Long
		lpstrFileTitle As String
		nMaxFileTitle As Long
		lpstrInitialDir As String
		lpstrTitle As String
		flags As Long
		nFileOffset As Integer
		nFileExtension As Integer
		lpstrDefExt As String
		lCustData As Long
		lpfnHook As Long
		lpTemplateName As String
	End Type
*/
	DWORD lStructSize;
	DWORD nFilterIndex;
	DWORD nMaxFile;
	LPCTSTR lpstrFilter;
	LPCTSTR lpstrFile;
	LPCTSTR lpstrTitle;
	LPCTSTR lpstrInitialDir;
};

class FileDialog
{
public:
	FileDialog(HWND hwndOwner, LPTSTR result, DWORD resultLength,
			   LPCTSTR initialFolder, LPCTSTR filter);
	FileDialog(const FileDialog&) = delete;
	FileDialog& operator=(FileDialog&) = delete;

	// Prepare dialog
	void SetFilterIndex(DWORD index)
	{
		m_ofn.nFilterIndex = index;
	}

	void AllowMultiSelect()
	{
		multiSelect = true;
//		m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	}

	void SetTitle(LPCTSTR title)
	{
		m_ofn.lpstrTitle = title;
	}

	// Show dialog
	bool Open()
	{
		// show folder select
//		if (strlen(m_ofn.lpstrFile) == 0)
//		{
//			bool err = swift_SelectFolder(m_ofn.lpstrFile, _MAX_PATH, "Choose a Folder");
//			return !err;
//		}
//		else
		{
			bool err = swift_GetFilesWithPreview(m_ofn.lpstrFile, m_ofn.nMaxFile, m_ofn.lpstrInitialDir, multiSelect, m_ofn.lpstrFilter);
			return !err;
		}
	}
	
	int SelectFilterFromExt(const char* filename)
	{
		const char *Ext = strrchr(filename, '.');
		if (Ext != nullptr)
		{
			if (_stricmp(Ext + 1, "ssd") == 0)
			{
				return 1;
			}
			else if (_stricmp(Ext + 1, "dsd") == 0)
			{
				return 2;
			}
			else if (_stricmp(Ext + 1, "adf") == 0)
			{
				return 5;
			}
			else if (_stricmp(Ext + 1, "adl") == 0)
			{
				return 6;
			}
		}
		return 0;
	}

	bool Save()
	{
		bool Result = swift_SaveFile(m_ofn.lpstrFile, 256, m_ofn.lpstrFilter, m_ofn.lpstrInitialDir);

		// lpstrFile will have changed value
		m_ofn.nFilterIndex = SelectFilterFromExt(m_ofn.lpstrFile);

		return Result;
	}

	// Get results
	DWORD GetFilterIndex() const
	{
		return m_ofn.nFilterIndex;
	}

private:
	OPENFILENAME m_ofn;
	
	bool ShowDialog(bool open);

	bool multiSelect;
};



#endif /* FileDialog_mac_hpp */
