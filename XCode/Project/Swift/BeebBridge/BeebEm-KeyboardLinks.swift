//
//  BeebEm-KeyboardLinks.swift
//  BeebEm
//
//  Created by Commander Coder on 13/05/2024.
//

import Foundation
import Cocoa

// these must match the dialogs and modals in the CPP files
@objc public enum Dialogs : UInt16 {
	case breakoutBox = 256
	case tapeControl
	case debugWindow
	case serialPort
	case teletextSelect
}

@objc public enum Modals : UInt16 {
	case keyboardLinks = 0
	case selectKey
	case romConfig
	case exportFiles
	case keyboardMapping
}

var allViews = [
	Dialogs.breakoutBox : breakoutBoxView,
	Dialogs.tapeControl : tapeControlView,
	Dialogs.debugWindow : dbgControlView,
	Dialogs.serialPort : serialPortView,
	Dialogs.teletextSelect : teletextView,
	Modals.keyboardLinks : keyboardLinksView,
	Modals.selectKey : selectKeyView,
	Modals.romConfig : romConfigView,
	Modals.exportFiles : exportFilesView,
	Modals.keyboardMapping : keyMapView
] as [AnyHashable : NSViewController]


func recurse2(find id: String, viewcontroller vc: NSViewController) -> NSButton? {
	for item in vc.view.subviewsRecursive() {
	  if item.identifier?.rawValue == id //&& item.isEnabled
	  {
		  return (item as? NSButton)  // nil otherwise
	  }
	}
  return nil
}


func dlgItemByIdentifier(_ id: String, _ vc: NSViewController) -> NSButton? {

	return recurse2(find: id, viewcontroller: vc)
}

// set the tick on the menu with a 4 character identifier
@_cdecl("swift_SetDlgCheck")
public func swift_SetDlgCheck(_ dlg: Dialogs, _ cmd: UInt32, _ check: Bool) -> Bool
{
	guard let v : NSViewController = allViews[dlg] else {
		return false
	}
	
	let cmdSTR =  conv(cmd)
	if let n = dlgItemByIdentifier(cmdSTR, v)
	{
//        print("\(#function)",cmdSTR,check)
		let oldstate = n.state
		n.state = check ? .on : .off
		return (oldstate == .on)
	}
	else
	{
		print("\(#function): \(cmd) not found: \(cmdSTR)")
	}
	
	return false
}

@_cdecl("swift_GetDlgCheck")
public func swift_GetDlgCheck(_ dlg: Dialogs, _ cmd: UInt32) -> Bool
{
	guard let v : NSViewController = allViews[dlg] else {
		return false
	}

	let cmdSTR =  conv(cmd)
	if let n = dlgItemByIdentifier(cmdSTR, v)
	{
//        print("\(#function)",cmdSTR,check)
		return n.state == .on
	}
	else
	{
		print("\(#function): \(cmd) not found: \(cmdSTR)")
	}
	
	return false
}





// need to have given the controller an identified (StoryboardID)
let keyboardLinksWindow: NSWindowController = NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "KeyboardLinksSB") as! NSWindowController

let keyboardLinksView: KeyboardLinksViewController = keyboardLinksWindow.contentViewController as! KeyboardLinksViewController



@_cdecl("swift_OpenDialog")
public func swift_OpenDialog(_ dlg: Dialogs, caller : UnsafeMutableRawPointer)
{
	print(caller)
	guard let dlgView = allViews[dlg] else {return}
	guard let dlgWindowCtrl = dlgView.view.window?.windowController else {return}
	dlgWindowCtrl.showWindow(nil)
}


// allow access to this in C
@_cdecl("swift_DoModal")
public func swift_DoModal(_ mod: Modals, caller : UnsafeMutableRawPointer)
{
	print(caller)
	guard let modView = allViews[mod] else {return}
	guard let modWindow = modView.view.window else {return}
	let modalresp = NSApp.runModal(for: modWindow)
	modWindow.close()
	NSApp.stopModal()

	//now export the files
	if (modalresp == NSApplication.ModalResponse.OK)
	{
		// this should call the method within Dialog

	}
}
