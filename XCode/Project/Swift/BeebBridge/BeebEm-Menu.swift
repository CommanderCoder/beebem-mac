//
//  BeebEm-Menu.swift
//  BeebEm
//
//  Created by Commander Coder on 19/04/2024.
//

import Foundation
import Cocoa



// convert a string of 4 characters to a UInt32
// if the string has more than 4 characters it will be clamped
// to the first 4
func conv(_ str: String) -> UInt32
{
	var  v: UInt32 = 0
    let  l = min(str.count,4)
	let  i = l-1
	for (index, element) in str.unicodeScalars.enumerated()
	{
		if index == 4
		{
			break
		}
		v += element.value << ((i-index)*8)
	}
	return v
}

// convert a UInt32 to a string of 4 characters
func conv(_ value: UInt32) -> String
{
	var  s: String = ""
	let  i = 4
	for index in 1...i
	{
		// if unicode not recognised - use '@'
		s.append( Character(UnicodeScalar(value >> ((i-index)*8)&0xff) ?? UnicodeScalar(64)))
	}
	return s
}


extension NSView {

	func subviewsRecursive() -> [NSView] {
		return subviews + subviews.flatMap { $0.subviewsRecursive() }
	}
}



func recurse(find id: String, menu: NSMenu) -> NSMenuItem? {
  for item in menu.items {
	  if item.identifier?.rawValue == id //&& item.isEnabled
	  {
		return item
	  } else if let submenu = item.submenu {
		if let item = recurse(find: id, menu: submenu) {
			return item
	   }
	}
  }
  return nil
}

func textFieldByIdentifier(id: String) -> NSTextField? {
	return nil
}


func menuItemByIdentifier(id: String) -> NSMenuItem? {
	guard let mainmenu = NSApplication.shared.mainMenu else { return nil }

	return recurse(find: id, menu: mainmenu)
}



// set the tick on the menu with a 4 character identifier
@_cdecl("swift_SetMenuCheck")
public func swift_SetMenuCheck(_ cmd: UInt32, _ check: Bool) -> Bool
{
	let cmdSTR =  conv(cmd)
	if let n = menuItemByIdentifier(id:cmdSTR)
	{
//        print("\(#function)",cmdSTR,check)
		let oldstate = n.state
		n.state = check ? .on : .off
		return (oldstate == .on)
	}
	else
	{
//		print("\(#function): \(cmd) not found: \(cmdSTR)")
	}
	
	return false
}


// set the text on the menu with a 4 character identifier
@_cdecl("swift_ModifyMenu")
public func swift_ModifyMenu(_ cmd: UInt32, _ newitem: UInt32,  _ itemtext: UnsafePointer<CChar>)
{
	assert(cmd == newitem)

	let cmdSTR =  conv(cmd)
	var text = String(cString: itemtext)
	if let n = menuItemByIdentifier(id:cmdSTR)
	{
		if (text.first == "&" )
		{
			text.removeFirst()
			n.keyEquivalent = String(text.first!)
		}
		n.title = text
	}
}

// grey the menu with a 4 character identifier
@_cdecl("swift_SetMenuEnable")
public func swift_SetMenuEnable(_ cmd: UInt32, _ enable: Bool) -> Bool
{
	// There is a checkbox in the Menu's Inspector called "Auto Enables Items" that was overriding my code.
	let cmdSTR =  conv(cmd)
	if let n = menuItemByIdentifier(id:cmdSTR)
	{
		let oldEnable = n.isEnabled
//        print("\(#function)",cmdSTR,enable)
		n.isEnabled = enable
		return oldEnable
	}
	else
	{
//        print("\(#function) not found: ",cmdSTR)
	}
	return false
}


@_cdecl("swift_SetMenuItemTextWithCString")
public func swift_SetMenuItemTextWithCString(_ cmd: UInt32, _ text: UnsafePointer<CChar>) -> Int
{
	let cmdSTR =  conv(cmd)
	if let n = menuItemByIdentifier(id:cmdSTR)
	{
		n.title = String(cString: text)
		return 0
	}
	return 1
}

// Rom Config

//@_cdecl("swift_RCSelectItem")
//public func swift_TCSelectItem(_ text: UnsafePointer<CChar>, _ b:Int, _ c: UnsafePointer<UInt>)
//{
////    print("\(#function) \(String(cString:text)) \(b) ")
//    tcViewControllerInstance?.selectRowInTable(UInt(b))
//}
//
//@_cdecl("swift_RCSelectItem")
//@_cdecl("swift_RCSelectItem")





@_cdecl("swift_menuSetControlValue")
public func swift_menuSetControlValue(_ cmd: UInt32, _ state: Int)
{
	let cmdSTR = conv(cmd)
	if let n = menuItemByIdentifier(id:cmdSTR)
	{
		print("scv",cmdSTR)
		n.state = (state==1) ? .on : .off
	}
}

@_cdecl("swift_menuGetControlValue")
public func swift_menuGetControlValue(_ cmd: UInt32) -> Int
{
	var val: Int = 0
	let cmdSTR =  conv(cmd)
	if let n = menuItemByIdentifier(id:cmdSTR)
	{
		print("gcv",cmdSTR)
		val = (n.state == .on) ? 1 : 0
	}
	return val
}

@_cdecl("swift_menuSetControlEditText")
public func swift_menuSetControlEditText(_ cmd: UInt32, _ text: NSString)
{
	let cmdSTR =  conv(cmd)
	if let n = textFieldByIdentifier(id:cmdSTR)
	{
		n.stringValue = text as String
	}
}

@_cdecl("swift_menuGetControlEditText")
public func swift_menuGetControlEditText(_ cmd: UInt32, _ text: NSMutableString, _ length:Int)
{
	let cmdSTR =  conv(cmd)
	if let n = textFieldByIdentifier(id:cmdSTR)
	{
		// set the text back in the C code - fill with zeros first
//        text.assign(repeating: 0, count: length)
//        text.assign(from: n.stringValue, count: length)
		text.setString(n.stringValue)
	}
	
}
