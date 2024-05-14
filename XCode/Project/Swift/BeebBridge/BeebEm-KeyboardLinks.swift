//
//  BeebEm-KeyboardLinks.swift
//  BeebEm
//
//  Created by Commander Coder on 13/05/2024.
//

import Foundation
import Cocoa


// need to have given the controller an identified (StoryboardID)
let keyboardLinksWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "KeyboardLinksSB") as! NSWindowController

let keyboardLinksView: KeyboardLinksViewController = keyboardLinksWindow.contentViewController as! KeyboardLinksViewController


func recurse2(find id: String, viewcontroller: NSViewController) -> NSButton? {
	for item in viewcontroller.view.subviewsRecursive() {
	  if item.identifier?.rawValue == id //&& item.isEnabled
	  {
		  if let button = item as? NSButton {
			  return button
		  }
	  }
	}
  return nil
}


func dlgItemByIdentifier(id: String) -> NSButton? {

	return recurse2(find: id, viewcontroller: keyboardLinksView)
}

// set the tick on the menu with a 4 character identifier
@_cdecl("swift_SetDlgCheck")
public func swift_SetDlgCheck(_ cmd: UInt32, _ check: Bool) -> Bool
{
	let cmdSTR =  conv(cmd)
	if let n = dlgItemByIdentifier(id:cmdSTR)
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
public func swift_GetDlgCheck(_ cmd: UInt32) -> Bool
{
	let cmdSTR =  conv(cmd)
	if let n = dlgItemByIdentifier(id:cmdSTR)
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


// Keyboard Links
@_cdecl("swift_DoModalKL")
public func swift_DoModalKL(caller : UnsafeMutableRawPointer)
{
	print(caller)

//	keyboardLinksWindow.showWindow(nil);
	
	let modalresp = NSApp.runModal(for: keyboardLinksWindow.window!)
	keyboardLinksWindow.close()
	NSApp.stopModal()

	//now export the files
	if (modalresp == NSApplication.ModalResponse.OK)
	{
		// this should call the method within Dialog

	}

}
