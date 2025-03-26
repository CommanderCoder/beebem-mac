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
    HWND hwndOwner;
    DWORD lStructSize;
	DWORD nFilterIndex;
	DWORD nMaxFile;
	LPCTSTR lpstrFilter;
	LPCTSTR lpstrFile;
	LPCTSTR lpstrTitle;
	LPCTSTR lpstrInitialDir;
    DWORD Flags;
};

#define OFN_FILEMUSTEXIST 0x00001000
#define OFN_PATHMUSTEXIST 0x00000800
#define OFN_HIDEREADONLY 0x00000004
#define OFN_OVERWRITEPROMPT 0x00000002
#define OFN_ALLOWMULTISELECT 0x00000200
#define OFN_EXPLORER 0x00080000

class FileDialog
{
	public:
        FileDialog(HWND hwndOwner, LPTSTR Result, DWORD ResultLength,
                   LPCTSTR InitialFolder, LPCTSTR Filter);
        FileDialog(const FileDialog&) = delete;
        FileDialog& operator=(FileDialog&) = delete;

	public:
		// Prepare dialog
		void SetFilterIndex(DWORD Index);
		void AllowMultiSelect();
		void NoOverwritePrompt();
		void SetTitle(LPCTSTR Title);

		// Show dialog
		bool Open();
		bool Save();

		// Get results
		DWORD GetFilterIndex() const;

	private:
		OPENFILENAME m_ofn;

		bool ShowDialog(bool Open);
};



#endif /* FileDialog_mac_hpp */
