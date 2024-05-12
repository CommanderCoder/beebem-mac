//
//  BeebEm-Bridge-RomController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let romConfigWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "ROMConfigSB") as! NSWindowController

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


// allow access to this in C
@_cdecl("swift_DoModalRC")
public func swift_DoModalRC(caller : UnsafeMutableRawPointer)
{
	
	print(caller)
	// run the modal until it is closed or the Export Selected (::exportSelected) button
	// is pressed

	let modalresp = NSApp.runModal(for: romConfigWindow.window!)
	romConfigWindow.close()
	NSApp.stopModal()

	//now export the files
	if (modalresp == NSApplication.ModalResponse.OK)
	{
		// this should call the method within Dialog

	}
}

@_cdecl("swift_SetCurSelRC")
public func swift_SetCurSelRC(selection : Int)
{
	romConfigView.modelSelection.selectItem(at: selection)
	romConfigView.tableView.reloadData()
}
