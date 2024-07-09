//
//  BeebEm-Bridge-RomController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let romConfigWindow: NSWindowController = GetWindowCtrl(for: Dialogs.romConfig)

let romConfigView: RomConfigViewController = romConfigWindow.contentViewController as! RomConfigViewController


@_cdecl("swift_GetSelectionMark")
public func  swift_GetSelectionMark(_ m : Dialogs) -> Int
{
	guard let viewCtrl = allViews[m] as? RomConfigViewController else {return 0}
	viewCtrl.tableView.reloadData()
	return viewCtrl.returnRowInTable()
}

@_cdecl("swift_SetSelectionMark")
public func  swift_SetSelectionMark(_ m : Dialogs, _ s: Int)
{
	guard let viewCtrl = allViews[m] as? RomConfigViewController else {return}
	viewCtrl.setRowInTable(s)
}

@_cdecl("swift_SetFocus")
public func  swift_SetFocus(_ m : Dialogs)
{
	guard let viewCtrl = allViews[m] as? RomConfigViewController else {return}
	viewCtrl.setFocus();
}



@_cdecl("swift_SetCurSel")
public func swift_SetCurSel(_ m : Dialogs, selection : Int)
{
	switch allViews[m] {
	case is RomConfigViewController:
		guard let rcCtrl = allViews[m] as? RomConfigViewController else {break}
		rcCtrl.modelSelection.selectItem(at: selection)
		rcCtrl.tableView.reloadData()
		break
		
	case .none:
		break

	case .some(_):
		break

	}
	
}
