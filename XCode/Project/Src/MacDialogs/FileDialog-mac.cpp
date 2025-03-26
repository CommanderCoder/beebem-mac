//
//  FileDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 16/04/2024.
//

#include "FileDialog-mac.hpp"


/*
 filter:
 const char* filter = "UEF Tape File (*.uef)\0*.uef\0";
 const char* filter =
	 "Auto (*.ssd;*.dsd;*.ad*;*.img;*.dos;*.fsd)\0*.ssd;*.dsd;*.adl;*.adf;*.img;*.dos;*.fsd\0"
	 "ADFS Disc (*.adl;*.adf)\0*.adl;*.adf\0"
	 "Single Sided Disc (*.ssd)\0*.ssd\0"
	 "Double Sided Disc (*.dsd)\0*.dsd\0"
	 "Single Sided Disc (*.*)\0*.*\0"
	 "Double Sided Disc (*.*)\0*.*\0";
 const char* filter =
	 "Auto (*.uef;*.csw)\0*.uef;*.csw\0"
	 "UEF Tape File (*.uef)\0*.uef\0"
	 "CSW Tape File (*.csw)\0*.csw\0";
 const char* filter = "FDC Extension Board Plugin DLL (*.dll)\0*.dll\0";
 const char* filter =
	 "Single Sided Disc (*.ssd)\0*.ssd\0"
	 "Double Sided Disc (*.dsd)\0*.dsd\0"
	 "Single Sided Disc (*.*)\0*.*\0"
	 "Double Sided Disc (*.*)\0*.*\0"
	 "ADFS M (80 Track) Disc (*.adf)\0*.adf\0"
	 "ADFS L (160 Track) Disc (*.adl)\0*.adl\0";
 const char* filter = "UEF State File (*.uefstate)\0*.uefstate\0";
 const char* filter = "UEF State File (*.uefstate; *.uef)\0*.uefstate;*.uef\0";
 const char* filter = "Printer Output (*.*)\0*.*\0";
 const char* filter = "AVI File (*.avi)\0*.avi\0";
 const char* filter = "Key Map File (*.kmap)\0*.kmap\0";
 const char* filter = "Key Map File (*.kmap)\0*.kmap\0";
 const char* filter = "INF files (*.inf)\0*.inf\0" "All files (*.*)\0*.*\0";
 const char* filter =
	 "Hard Drive Images (*.dat)\0*.dat\0"
	 "All files (*.*)\0*.*\0";
 const char* filter = "Memory Dump Files (*.dat)\0*.dat\0" "All Files (*.*)\0*.*\0";
 const char* filter = "Text Files (*.txt)\0*.txt\0";
 const char* filter = "Debugger Script Files (*.txt)\0*.txt\0" "All Files (*.*)\0*.*\0";
 const char* filter = "Label Files (*.txt)\0*.txt\0" "All Files (*.*)\0*.*\0";
 const char* Filter = "All Files (*.*)\0*.*\0";
 const char* filter = "ROM Config File (*.cfg)\0*.cfg\0";
 const char* filter = "ROM File (*.rom)\0*.rom\0";
 const char* Filter = "Teletext Capture File (*.dat)\0*.dat\0";
 const char* filter = "Label Files (*.txt)\0*.txt\0" "All Files (*.*)\0*.*\0";
 const char* filter = "Debugger Script Files (*.txt)\0*.txt\0" "All Files (*.*)\0*.*\0";
 

 */

// __APPLE__
BOOL GetOpenFileName(OPENFILENAME* ofn)
{
	// show folder select
	//		if (strlen(m_ofn.lpstrFile) == 0)
	//		{
	//			bool err = swift_SelectFolder(m_ofn.lpstrFile, _MAX_PATH, "Choose a Folder");
	//			return !err;
	//		}
	//		else
	{
		bool multiSelect = (ofn->Flags & OFN_ALLOWMULTISELECT) != 0;
		bool err = swift_GetFilesWithPreview(ofn->lpstrFile, ofn->nMaxFile, ofn->lpstrInitialDir, multiSelect, ofn->lpstrFilter, ofn->lpstrTitle);
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

BOOL GetSaveFileName(OPENFILENAME* ofn)
{
	bool Result = swift_SaveFile(ofn->lpstrFile, 256, ofn->lpstrFilter, ofn->lpstrInitialDir);

	// lpstrFile will have changed value
	ofn->nFilterIndex = SelectFilterFromExt(ofn->lpstrFile);

	return Result;
}

// MacOS FileDialog

FileDialog::FileDialog(HWND hwndOwner, LPTSTR Result, DWORD ResultLength,
                       LPCTSTR InitialFolder, LPCTSTR Filter)
{
    ZeroMemory(&m_ofn, sizeof(m_ofn));

    m_ofn.lStructSize = sizeof(OPENFILENAME);
    m_ofn.hwndOwner = hwndOwner;
    m_ofn.lpstrFilter = Filter;
    m_ofn.nFilterIndex = 1;
    m_ofn.lpstrFile = Result;
    m_ofn.nMaxFile = ResultLength;
    m_ofn.lpstrInitialDir = InitialFolder;
    m_ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
                  OFN_OVERWRITEPROMPT;
}

/****************************************************************************/

void FileDialog::SetFilterIndex(DWORD Index)
{
	m_ofn.nFilterIndex = Index;
}

void FileDialog::AllowMultiSelect()
{
	m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER;
}

void FileDialog::NoOverwritePrompt()
{
	m_ofn.Flags &= ~OFN_OVERWRITEPROMPT;
}

void FileDialog::SetTitle(LPCTSTR Title)
{
	m_ofn.lpstrTitle = Title;
}

/****************************************************************************/

bool FileDialog::Open()
{
	return ShowDialog(true);
}

bool FileDialog::Save()
{
	return ShowDialog(false);
}

/****************************************************************************/

DWORD FileDialog::GetFilterIndex() const
{
	return m_ofn.nFilterIndex;
}

/****************************************************************************/

bool FileDialog::ShowDialog(bool Open)
{
	if (Open)
	{
		return GetOpenFileName(&m_ofn) != 0;
	}
	else
	{
		return GetSaveFileName(&m_ofn) != 0;
	}
}
