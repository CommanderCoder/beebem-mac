//
//  ExportFileDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 16/04/2024.
//


#include <windows.h>

#include <string>

#include "ExportFileDialog.h"
#include "DebugTrace.h"
#include "DiscEdit.h"
#include "FileDialog.h"
#include "FileUtils.h"
#include "FolderSelectDialog.h"
#include "ListView.h"
#include "Main.h"
#include "RenameFileDialog.h"
#include "Resource.h"
#include "StringUtils.h"




#define LVIS_SELECTED 0
#define LVNI_SELECTED 1

// NOTE: ExportFileDialog::m_FileSelected is not used even though it exists in the windows version
// values are grabbed directly from the ListView ItemData
static int filesSelected[DFS_MAX_CAT_SIZE];

// populate the menu with disc files
char szDiscFile[MAX_PATH];
int heads;
int side;


FileExportEntry* LVGetItemDataEF(HWND a, UINT b)
{
	return NULL;
//	return &(m_ExportFiles[filesSelected[b]]);
}



/****************************************************************************/


ExportFileDialog::ExportFileDialog(HINSTANCE hInstance,
								   HWND hwndParent,
								   const char* szDiscFile,
								   int NumSides,
								   int Side,
								   DFS_DISC_CATALOGUE* dfsCat,
								   const char* ExportPath) :
	Dialog(hInstance, hwndParent, IDD_DISCEXPORT),
	m_DiscFile(szDiscFile),
	m_NumSides(NumSides),
	m_Side(Side),
	m_ExportPath(ExportPath),
	m_NumSelected(0)
{
	for (int i = 0; i < dfsCat->numFiles; ++i)
	{
		FileExportEntry Entry;
		Entry.DfsAttrs = dfsCat->fileAttrs[i];
		Entry.BeebFileName = std::string(1, dfsCat->fileAttrs[i].directory) + "." + dfsCat->fileAttrs[i].filename;
		Entry.HostFileName = BeebToLocalFileName(Entry.BeebFileName);

		m_ExportFiles.push_back(Entry);
	}
}

/****************************************************************************/
//
//static LPCTSTR Columns[] = {
//	"File name",
//	"Load Addr",
//	"Exec Addr",
//	"Length",
//	"Host File name"
//};





// 6 columns
char* dfsNames[DFS_MAX_CAT_SIZE][6];
 
int LVInsertItemEF(HWND hWnd, UINT uRow, UINT uCol, const char* pszText, LPARAM lParam)
{
	for (int c=0;c<6;c++)
		dfsNames[uRow][c] = (char*)malloc(100);
	sprintf(dfsNames[uRow][uCol], "%-7s", pszText);
	return 0;
}


void LVSetItemTextEF(HWND hWnd, UINT uRow, UINT uCol, const LPTSTR pszText)
{
	sprintf(dfsNames[uRow][uCol], "%-7s", pszText);
}

bool ExportFileDialog::DoModal()
{
	WM_INITDIALOG();
	return swift_DoModal(Modals::exportFiles, this);
}


bool ExportFileDialog::WM_INITDIALOG()
{

	// Make a list view that is row select with grid lines and make left formatted columns
	// 'Columns' count ; filename, load addr, ..etc
	// autosize column widths
	
	int Row = 0;

	// add each of m_ExportFiles as rows
	for (const FileExportEntry& Entry : m_ExportFiles)
	{
		// List is sorted so store catalogue index in list's item data
		LVInsertItemEF(m_hwndListView,
					 Row,
					 0,
					 const_cast<LPTSTR>(Entry.BeebFileName.c_str()),
					 reinterpret_cast<LPARAM>(&Entry));

		char str[100];
		sprintf(str, "%06X", Entry.DfsAttrs.loadAddr & 0xffffff);
		LVSetItemTextEF(m_hwndListView, Row, 1, str);

		sprintf(str, "%06X", Entry.DfsAttrs.execAddr & 0xffffff);
		LVSetItemTextEF(m_hwndListView, Row, 2, str);

		sprintf(str, "%06X", Entry.DfsAttrs.length);
		LVSetItemTextEF(m_hwndListView, Row, 3, str);

		LVSetItemTextEF(m_hwndListView, Row, 4, const_cast<LPTSTR>(Entry.HostFileName.c_str()));

		Row++;
	}


  
   // this sets the rows in the ExportDiscViewController.beeblistdata
   // to be displayed in the list
   swift_InitExportDialog(dfsNames, Row, 6);

   return TRUE;
}

#ifndef __APPLE__
	case WM_NOTIFY: {
		LPNMHDR nmhdr = (LPNMHDR)lParam;

		if (nmhdr->hwndFrom == m_hwndListView && nmhdr->code == NM_DBLCLK)
		{
			LPNMITEMACTIVATE pItemActivate = (LPNMITEMACTIVATE)lParam;

			LVHITTESTINFO HitTestInfo = { 0 };
			HitTestInfo.pt = pItemActivate->ptAction;
			ListView_SubItemHitTest(m_hwndListView, &HitTestInfo);

			FileExportEntry* Entry = reinterpret_cast<FileExportEntry*>(
				LVGetItemData(m_hwndListView, HitTestInfo.iItem)
			);

			if (Entry)
			{
				RenameFileDialog Dialog(hInst,
										m_hwnd,
										Entry->BeebFileName.c_str(),
										Entry->HostFileName.c_str());

				if (Dialog.DoModal())
				{
					Entry->HostFileName = Dialog.GetHostFileName();

					LVSetItemText(m_hwndListView, HitTestInfo.iItem, 4, const_cast<LPTSTR>(Entry->HostFileName.c_str()));
				}
			}
		}
		break;
	}
#else
void ExportFileDialog::WM_NOTIFY()
{
	// double click a filename will allow it to be changed before exporting
	int hitItem = 0; //HitTestInfo.iItem
	
	// if Notified of DOUBLECLICK on a valid SELECTED ITEM
	FileExportEntry* Entry = reinterpret_cast<FileExportEntry*>(
		LVGetItemDataEF(m_hwndListView, hitItem)
	);

	if (Entry)
	{
		RenameFileDialog Dialog(hInst,
								m_hwnd,
								Entry->BeebFileName.c_str(),
								Entry->HostFileName.c_str());

		if (Dialog.DoModal())
		{
			Entry->HostFileName = Dialog.GetHostFileName();

			LVSetItemTextEF(m_hwndListView, hitItem, 4, const_cast<LPTSTR>(Entry->HostFileName.c_str()));
		}
	}
	
}
#endif
			
#ifndef __APPLE__

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			ExportSelectedFiles();
			return TRUE;

		case IDCANCEL:
			EndDialog(m_hwnd, wParam);
			return TRUE;
		}
	}

	return FALSE;
}
#else
bool ExportFileDialog::WM_COMMAND(WPARAM param)
{
	switch (param)
	{
	case IDOK:
		ExportSelectedFiles();
		return true;

	case IDCANCEL:
//		swift_EndModal(false);
		return true;
	}
	return false;
}
#endif















/****************************************************************************/

std::string ExportFileDialog::GetPath() const
{
	return m_ExportPath;
}

/****************************************************************************/




int itemCount = 0;
int ListView_GetSelectedCount(HWND a)
{
	itemCount = swift_SelectedFiles(filesSelected, sizeof(filesSelected));
	return itemCount;
}

int itemIndex = 0;
int ListView_GetNextItem(HWND a, int b, int c)
{
	itemIndex++;
	if (itemIndex>=itemCount)
	{
		return -1;
	}
	
	if (b<0)
	{
		itemIndex = 0;
		return filesSelected[itemIndex];
	}

	return filesSelected[itemIndex];
}

int ListView_GetItemCount(HWND a)
{
	return 0; // number of items in the listview
}

void ListView_SetItemState(HWND a, int c, int x, int y)
{
	// set state of item in the listview
}


void ExportFileDialog::ExportSelectedFiles()
{
	m_NumSelected = ListView_GetSelectedCount(m_hwndListView);

	if (m_NumSelected == 0)
	{
		int Count = ListView_GetItemCount(m_hwndListView);

		for (int i = 0; i < Count; i++)
		{
			ListView_SetItemState(m_hwndListView, i, LVIS_SELECTED, LVIS_SELECTED);
		}

		m_NumSelected = Count;
	}

	// Get folder to export to
	FolderSelectDialog Dialog(m_hwnd,
							  "Select folder for exported files:",
							  m_ExportPath.c_str());

	FolderSelectDialog::Result Result = Dialog.DoModal();

	switch (Result)
	{
		case FolderSelectDialog::Result::OK:
			m_ExportPath = Dialog.GetFolder();
			break;

		case FolderSelectDialog::Result::InvalidFolder:
			mainWin->Report(MessageType::Warning, "Invalid folder selected");
			return;

		case FolderSelectDialog::Result::Cancel:
		default:
			return;
	}

	int Item = ListView_GetNextItem(m_hwndListView, -1, LVNI_SELECTED);
	
	int Count = 0;

	while (Item != -1)
	{
//		FileExportEntry* Entry = reinterpret_cast<FileExportEntry*>(
//			LVGetItemDataEF(m_hwndListView, Item)
//		);
		FileExportEntry* Entry =
					&(m_ExportFiles[Item]);


		std::string LocalFileName = AppendPath(m_ExportPath, Entry->HostFileName);

		if (FileExists(LocalFileName.c_str()))
		{
			char FileName[_MAX_PATH];
			const char* Filter = "All Files (*.*)\0*.*\0";

			strcpy(FileName, Entry->HostFileName.c_str());

			FileDialog fileDialog(m_hwnd, FileName, sizeof(FileName), m_ExportPath.c_str(), Filter);

			if (fileDialog.Save())
			{
				if (ExportFile(&Entry->DfsAttrs, FileName))
				{
					Count++;
				}
			}
		}
		else
		{
			if (ExportFile(&Entry->DfsAttrs, LocalFileName.c_str()))
			{
				Count++;
			}
		}

		Item = ListView_GetNextItem(m_hwndListView, Item, LVNI_SELECTED);
	}

	
	mainWin->Report(MessageType::Info, "Files successfully exported: %d", Count);
	
}

/****************************************************************************/

bool ExportFileDialog::ExportFile(DFS_FILE_ATTR* DfsAttrs, const char* LocalFileName)
{
	char szErrStr[512];

	if (dfs_export_file(m_DiscFile, m_NumSides, m_Side, DfsAttrs, LocalFileName, szErrStr))
	{
		return true;
	}
	else
	{
		mainWin->Report(MessageType::Error, szErrStr);
		return false;
	}
}
