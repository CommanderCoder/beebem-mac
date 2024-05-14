//
//  BeebEm-Bridge-RomController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let romConfigWindow: NSWindowController = GetWindowCtrl(for: Modals.romConfig)

let romConfigView: RomConfigViewController = romConfigWindow.contentViewController as! RomConfigViewController


@_cdecl("swift_RCGetSelectionMark")
public func  swift_RCGetSelectionMark() -> Int
{
	romConfigView.tableView.reloadData()
	return romConfigView.returnRowInTable()
}

@_cdecl("swift_RCSetFocus")
public func  swift_RCSetFocus()
{
	romConfigView.setFocus();
}



@_cdecl("swift_SetCurSelRC")
public func swift_SetCurSelRC(selection : Int)
{
	romConfigView.modelSelection.selectItem(at: selection)
	romConfigView.tableView.reloadData()
}
