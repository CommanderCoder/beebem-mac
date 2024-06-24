//
//  KeyboardLinksDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 13/05/2024.
//

#ifndef KeyboardLinksDialog_mac_hpp
#define KeyboardLinksDialog_mac_hpp

#include <stdio.h>

#include "Dialog.h"

class KeyboardLinksDialog : public Dialog
{
	public:
		KeyboardLinksDialog(
			HINSTANCE hInstance,
			HWND hwndParent,
			unsigned char Value
		);

	public:
		unsigned char GetValue() const;

	bool DoModal();

	virtual bool WM_COMMAND(WPARAM wParam);
	private:
	virtual bool WM_INITDIALOG();
	virtual void WM_NOTIFY() {}
//
//		virtual INT_PTR DlgProc(
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);

	private:
		unsigned char m_Value;
};

#endif /* KeyboardLinksDialog_mac_hpp */
