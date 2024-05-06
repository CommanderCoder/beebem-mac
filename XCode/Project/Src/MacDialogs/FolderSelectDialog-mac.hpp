//
//  FolderSelectDialog.hpp
//  BeebEm
//
//  Created by Commander Coder on 26/04/2024.
//

#ifndef FolderSelectDialog_hpp
#define FolderSelectDialog_hpp

#include <stdio.h>
#include "apples.h"

class FolderSelectDialog
{
	public:
		enum class Result {
			OK,
			Cancel,
			InvalidFolder
		};

	public:
		FolderSelectDialog(
			HWND hwndOwner,
			const char *Title,
			const char *InitialFolder
		);

	public:
		Result DoModal();

		std::string GetFolder() const;

	private:
//		static int CALLBACK BrowseCallbackProc(
//			HWND hWnd,
//			UINT uMsg,
//			LPARAM lParam,
//			LPARAM lpData
//		);

	private:
//		BROWSEINFO m_BrowseInfo;
		char m_Buffer[MAX_PATH];
		std::string m_InitialFolder;
		std::string m_Title;
	
	
	// From DIALOG
		HINSTANCE m_hInstance;
		HWND m_hwndParent;
		int m_DialogID;
		HWND m_hwnd;

};

#endif /* FolderSelectDialog_hpp */
