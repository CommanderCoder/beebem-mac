//
//  RenameFileDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 26/04/2024.
//

#ifndef RenameFileDialog_mac_hpp
#define RenameFileDialog_mac_hpp

#include <stdio.h>

#include <windows.h>

#include <string>

#include "Dialog.h"

class RenameFileDialog : public Dialog
{
	public:
		RenameFileDialog(
			HINSTANCE hInstance,
			HWND hwndParent,
			const char* pszBeebFileName,
			const char* pszHostFileName
		);

	public:
		const std::string& GetHostFileName() const;

	bool DoModal()	{ WM_INITDIALOG(); return true; }
	
	private:
		void ExportSelectedFiles();

	private:
//		virtual INT_PTR DlgProc(
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);

		virtual bool WM_INITDIALOG();
		virtual bool WM_COMMAND(int wParam){return false;}
		virtual void WM_NOTIFY(){}

	private:
		std::string m_BeebFileName;
		std::string m_HostFileName;
};

#endif /* RenameFileDialog_mac_hpp */
