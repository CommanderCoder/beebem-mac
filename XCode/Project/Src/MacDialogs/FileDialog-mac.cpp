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
