//
//  RomConfigDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 29/04/2024.
//

#ifndef RomConfigDialog_mac_hpp
#define RomConfigDialog_mac_hpp

#include <stdio.h>


#include "apples.h"

#include "Dialog.h"
#include "Model.h"
#include <string>

#include "RomConfigFile.h"

class RomConfigDialog : public Dialog
{
	public:
		RomConfigDialog(
			HINSTANCE hInstance,
			HWND hwndParent,
			const RomConfigFile& Config
		);

	public:
		virtual bool WM_INITDIALOG();
		virtual bool WM_COMMAND(WPARAM wParam);
		virtual void WM_NOTIFY(){}
		bool DoModal();
		const RomConfigFile& GetRomConfig() const;

	private:
//		virtual INT_PTR DlgProc(
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);

		void UpdateROMField(int Row);
		void FillModelList();
		void FillROMList();
		bool LoadROMConfigFile();
		bool SaveROMConfigFile();
		bool GetROMFile(char *pszFileName);

	private:
		HWND m_hWndROMList;
		HWND m_hWndModel;
		RomConfigFile m_RomConfig;
		Model m_Model;
};


extern const char* GetRCEntry(UINT,UINT);

#endif /* RomConfigDialog_mac_hpp */
