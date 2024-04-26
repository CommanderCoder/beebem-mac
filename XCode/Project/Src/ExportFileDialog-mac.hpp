//
//  ExportFileDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 16/04/2024.
//

#ifndef ExportFileDialog_mac_hpp
#define ExportFileDialog_mac_hpp

#include <stdio.h>

#include <string>
#include <vector>

#include "Dialog.h"
#include "DiscEdit.h"


struct FileExportEntry
{
	DFS_FILE_ATTR DfsAttrs;
	std::string BeebFileName;
	std::string HostFileName;
};

class ExportFileDialog : public Dialog
{
	public:
		ExportFileDialog(
			 HINSTANCE hInstance,
			 HWND hwndParent,
			 const char* szDiscFile,
			 int NumSides,
			 int Side,
			 DFS_DISC_CATALOGUE* dfsCat,
			 const char* ExportPath
		);

	public:
		std::string GetPath() const;
	
		bool DoModal();

		bool WM_COMMAND(int param);
	private:
		void ExportSelectedFiles();
		bool ExportFile(DFS_FILE_ATTR* DfsAttrs, const char* LocalFileName);
	
		void WM_INITDIALOG();
		void WM_NOTIFY();

	private:
		const char* m_DiscFile;
		int m_NumSides;
		int m_Side;
		std::vector<FileExportEntry> m_ExportFiles;
		std::string m_ExportPath;
		HWND m_hwndListView;
		int m_FilesSelected[DFS_MAX_CAT_SIZE];  //unused on windows
		int m_NumSelected;
};


#endif /* ExportFileDialog_mac_hpp */
