//
//  FolderSelectDialog.cpp
//  BeebEm
//
//  Created by Commander Coder on 26/04/2024.
//


#include <windows.h>

#include "FolderSelectDialog-mac.hpp"

//int CALLBACK FolderSelectDialog::BrowseCallbackProc(
//	HWND hWnd,
//	UINT uMsg,
//	LPARAM /* lParam */,
//	LPARAM lpData)
//{
//	FolderSelectDialog* pDialog = reinterpret_cast<FolderSelectDialog *>(lpData);
//
//	if (pDialog != nullptr)	{
//		switch (uMsg) {
//			case BFFM_INITIALIZED:
//				if (!pDialog->m_InitialFolder.empty()) {
//					SendMessage(
//						hWnd,
//						BFFM_SETEXPANDED,
//						TRUE, // lParam contains a path name, not a pidl.
//						(LPARAM)pDialog->m_InitialFolder.c_str()
//					);
//
//					SendMessage(
//						hWnd,
//						BFFM_SETSELECTION,
//						TRUE, // lParam contains a path name, not a pidl.
//						(LPARAM)pDialog->m_InitialFolder.c_str()
//					);
//				}
//				break;
//		}
//	}
//
//	return 0;
//}

FolderSelectDialog::FolderSelectDialog(
	HWND hwndOwner,
	const char *Title,
	const char *InitialFolder) :
	m_Title(Title),
	m_InitialFolder(InitialFolder)
{
	m_Buffer[0] = 0;
}

FolderSelectDialog::Result FolderSelectDialog::DoModal()
{
	Result result = Result::Cancel;

	char exportPath[MAX_PATH];
	bool error = swift_SelectFolder(exportPath, MAX_PATH, m_Title.c_str());
	
	result = Result::InvalidFolder;
	if (error)
	{
	   m_Buffer[0] = 0;
	   result = Result::Cancel;
	}
	else
	{
		strcpy(m_Buffer, exportPath);
		result = Result::OK;
	}
	
	return result;
}

std::string FolderSelectDialog::GetFolder() const
{
	return m_Buffer;
}
