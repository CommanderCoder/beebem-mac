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
	case keyboardLinks = 0
	case selectKey
	case romConfig
	case exportFiles
	case keyboardMapping
	case breakoutBox = 256
	case tapeControl
	case debugWindow
	case serialPort
	case teletextSelect
}

var allViews = [
	Dialogs.breakoutBox : breakoutBoxView,
	Dialogs.tapeControl : tapeControlView,
	Dialogs.debugWindow : dbgControlView,
	Dialogs.serialPort : serialPortView,
	Dialogs.teletextSelect : teletextView,
	Dialogs.keyboardLinks : keyboardLinksView,
	Dialogs.selectKey : selectKeyView,
	Dialogs.romConfig : romConfigView,
	Dialogs.exportFiles : exportFilesView,
	Dialogs.keyboardMapping : keyMapView
]

var allStoryboardNames = [
	Dialogs.breakoutBox : "BreakoutBoxSB",
	Dialogs.tapeControl : "TapeControlSB",
	Dialogs.debugWindow : "DebuggerSB",
	Dialogs.serialPort : "SerialPortSB",
	Dialogs.teletextSelect : "TeletextSB",
	Dialogs.keyboardLinks : "KeyboardLinksSB",
	Dialogs.selectKey : "SelectKeySB",
	Dialogs.romConfig : "ROMConfigSB",
	Dialogs.exportFiles : "ExportFilesSB",
	Dialogs.keyboardMapping : "KeyMapSB"
]


func GetWindowCtrl(for f: Dialogs) -> NSWindowController
{
	return NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: allStoryboardNames[f]!) as! NSWindowController
}


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
	guard let dlgView : NSViewController = allViews[dlg] else {
		return false
	}
	
	let cmdSTR =  conv(cmd)
	if let n = dlgItemByIdentifier(cmdSTR, dlgView)
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
	guard let dlgView : NSViewController = allViews[dlg] else {
		return false
	}

	let cmdSTR =  conv(cmd)
	if let n = dlgItemByIdentifier(cmdSTR, dlgView)
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
let keyboardLinksWindow: NSWindowController = GetWindowCtrl(for: Dialogs.keyboardLinks)


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
public func swift_DoModal(_ mod: Dialogs, caller : UnsafeMutableRawPointer) -> Bool
{
	print(caller)
	guard let modView = allViews[mod] else {return false}
	guard let modWindow = modView.view.window else {return false}
	let modalresp = NSApp.runModal(for: modWindow)
	modWindow.close()
	NSApp.stopModal()

	//now export the files
	if (modalresp == NSApplication.ModalResponse.OK)
	{
		// this should call the method within Dialog
		beeb_ModalOK(caller);
		return true
	}
	return false
}

// allow access to this in C
@_cdecl("swift_EndModal")
public func swift_EndModal(_ ok: Bool)
{
	if ok
	{
		NSApp.stopModal(withCode: NSApplication.ModalResponse.OK)
	}
	else
	{
		NSApp.stopModal(withCode: NSApplication.ModalResponse.cancel)
	}
}