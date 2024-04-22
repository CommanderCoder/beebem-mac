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

#include "DiscEdit.h"
#include "ExportFileDialog.h"

class ExportFileDialog
{
	public:
		ExportFileDialog(
			 HINSTANCE _hinstIgnore,
			 HWND _hwndIgnore,
			const char* szDiscFile,
			int NumSides,
			int Side,
			DFS_DISC_CATALOGUE* dfsCat,
			const char* ExportPath
		);

	public:
		std::string GetPath() const;
	
		bool DoModal();

	private:
		void ExportSelectedFiles();
		bool ExportFile(DFS_FILE_ATTR* DfsAttrs, const char* LocalFileName);

	private:
		const char* m_DiscFile;
		int m_NumSides;
		int m_Side;
		std::vector<FileExportEntry> m_ExportFiles;
		std::string m_ExportPath;
		int m_FilesSelected[DFS_MAX_CAT_SIZE];
		int m_NumSelected;
};

#endif /* ExportFileDialog_mac_hpp */
