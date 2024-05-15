//
//  Dialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 26/04/2024.
//

#ifndef Dialog_mac_hpp
#define Dialog_mac_hpp

#include "apples.h"
#include <stdio.h>
#include <string>

class Dialog
{
	public:
		Dialog(
			HINSTANCE hInstance,
			HWND hwndParent,
			int DialogID
		);

	private:
//		static INT_PTR CALLBACK sDlgProc(
//			HWND   hwnd,
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);
//
//		virtual INT_PTR DlgProc(
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		) = 0;
public:

	virtual bool WM_INITDIALOG() = 0;
	virtual bool WM_COMMAND(WPARAM wParam) = 0;
	virtual void WM_NOTIFY() = 0;

	protected:
//		std::string GetDlgItemText(int nID);
//		void SetDlgItemText(int nID, const std::string& str);
		bool IsDlgItemChecked(int nID);
		void SetDlgItemChecked(int nID, bool bChecked);
//		void SetDlgItemFocus(int nID);
//		void EnableDlgItem(int nID, bool bEnable);

	private:
//		void CenterDialog();

	protected:
		HINSTANCE m_hInstance;
		HWND m_hwndParent;
		int m_DialogID;
		HWND m_hwnd;
};
#endif /* Dialog_mac_hpp */
