//
//  BeebEm5-Bridging-Code.swift
//  BeebEm5
//
//  Created by Commander Coder on 06/07/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

import Foundation
import Cocoa
import AVFoundation





//https://developer.apple.com/documentation/uniformtypeidentifiers/uttype
//https://useyourloaf.com/blog/swiftui-importing-and-exporting-files/
extension UTType {
	public static let ssd = UTType(importedAs: "com.commandercoder.BeebEm.ssd")
	public static let hdd = UTType(importedAs: "com.commandercoder.BeebEm.dat")
	public static let tape = UTType(importedAs: "com.commandercoder.BeebEm.uef")
	public static let rom = UTType(importedAs: "com.commandercoder.BeebEm.rom")

	public static let keymap = UTType(exportedAs: "com.commandercoder.BeebEm.kmap")
	public static let state = UTType(exportedAs: "com.commandercoder.BeebEm.uefstate")
}


// allow access to this in C
@_cdecl("swift_GetFilesWithPreview")
func swift_GetFilesWithPreview(filepath : UnsafeMutablePointer<CChar>, bytes: Int, directory : UnsafeMutablePointer<CChar>, fileexts : FileFilter, multiFiles : Bool = false) -> Int
{
    let dialog = NSOpenPanel()
    
    dialog.title                   = "Choose a file | BeebEm5"
    dialog.showsResizeIndicator    = true
    dialog.showsHiddenFiles        = false
    dialog.allowsMultipleSelection = multiFiles
    dialog.canChooseDirectories    = false
    dialog.canChooseFiles    = true
    dialog.allowsOtherFileTypes = true


    // FUTURE
//    let launcherLogPath = NSString("~/Documents").expandingTildeInPath  // need to expand the path if it includes ~
    let launcherLogPath = String( cString: directory)
    dialog.directoryURL = NSURL.fileURL(withPath: launcherLogPath, isDirectory: true)
    dialog.canCreateDirectories = true
    
    switch fileexts {
    case .DISC:
		dialog.allowedContentTypes        = [.ssd] // ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"]
		break
	case .UEFSTATEFILE:
		dialog.allowedContentTypes        = [.state]
		break
	case .UEFFILE:
        dialog.allowedContentTypes        = [.tape] // ["uef", "csw"]
		break
    case .IFD:
        dialog.allowedContentTypes        = [] // ["ssd", "dsd", "inf"]
		break
    case .KEYBOARD:
        dialog.allowedContentTypes        = [.keymap] // ["kmap"]
		break
	case .HARDDRIVE:
		dialog.allowedContentTypes        = [.hdd] //nil  // ["inf"]
		break
    case .DISCFILE:
		dialog.allowedContentTypes        = [.data] //nil  // ["inf"]
		break
    case .ROMCFG:
        dialog.allowedContentTypes        = [.rom] // ["rom"]
		break
    case .PRINTFILE:
        break
	case .ANYFILE:
		dialog.allowedContentTypes        = []
		break
	}
    
    if (dialog.runModal() ==  NSApplication.ModalResponse.OK) {
        let result = dialog.urls // Pathnames of the files

        if (result.count != 0) {
            
			var path: String = ""

			if (result.count == 1)
			{
				path = result[0].path + "\0"
			}
			else
			{
			for f in result
			{
				if path.isEmpty {
					// need base path
					path = f.deletingLastPathComponent().path + "\0"
				}
				path = path + f.lastPathComponent + "\0"
			}
			}
            // path contains the file path e.g
            // /Users/ourcodeworld/Desktop/file.txt
            
            // set the filepath back in the C code - fill with zeros first
            filepath.assign(repeating: 0, count: bytes)
            filepath.assign(from: path, count: path.count)

            return 0 // no issues
        }
    }
//      User clicked on "Cancel"
// or   the result URL was nil
    return 1 // error
}

@_cdecl("swift_SelectFolder")
func swift_SelectFolder(filepath : UnsafeMutablePointer<CChar>, bytes: Int, title : UnsafePointer<CChar> ) -> Int
{
    let dialog = NSOpenPanel();
    dialog.title                   = "Select Folder | BeebEm5"
	dialog.message                   = String(cString: title)
	dialog.showsResizeIndicator    = true
    dialog.canChooseDirectories    = true
    dialog.canChooseFiles          = false
    
    if (dialog.runModal() ==  NSApplication.ModalResponse.OK) {
        guard let result = dialog.url else { return 0} // Pathname of the file
        let path: String = result.path
        
        // path contains the folder e.g
        // /Users/ourcodeworld/Desktop/
        
        // set the filepath back in the C code.. fill with zeros first
        filepath.assign(repeating: 0, count: bytes)
        filepath.assign(from: path, count: path.count)
            
        print("Picked \(String(cString:filepath))")
        return 0
    }
    return 1 //err
}



@_cdecl("swift_SaveFile")
func swift_SaveFile(filepath : UnsafeMutablePointer<CChar>, bytes: Int, fileexts: FileFilter) -> Bool
{
	// test that PRINTDIALOG with no filename works
	let dialog = NSSavePanel()
    
    dialog.title                   = "Choose a file | BeebEm5"
    dialog.showsResizeIndicator    = true
    dialog.showsHiddenFiles        = false
    dialog.allowsOtherFileTypes    = true
	dialog.isExtensionHidden	= false
	
    switch fileexts {
    case .DISC:
        dialog.allowedContentTypes        = [.ssd] // ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"]
		break
	case .UEFSTATEFILE:
		dialog.allowedContentTypes        = [.state]
		break
    case .UEFFILE:
        dialog.allowedContentTypes        = [.tape] // ["uef", "csw"]
		break
	case .ANYFILE:
		dialog.allowedContentTypes        = []
		break
    case .IFD:
        dialog.allowedContentTypes        = [] // ["ssd", "dsd", "inf"]
		break
    case .KEYBOARD:
        dialog.allowedContentTypes        = [.keymap] // ["kmap"]
		break
	case .HARDDRIVE:
		dialog.allowedContentTypes        = [.data] //nil  // ["inf"]
		break
    case .DISCFILE:
		dialog.allowedContentTypes        = [.data] //nil // ["inf"]
		break
    case .ROMCFG:
        dialog.allowedContentTypes        = [.rom] // ["rom"]
		break
    case .PRINTFILE:
		dialog.allowedContentTypes        = [.plainText] //nil  // ["inf"]
		break
    }

    if (dialog.runModal() ==  NSApplication.ModalResponse.OK) {
        guard let result = dialog.url else { return false} // Pathname of the file

        let path: String = result.path
        
        // path contains the file path e.g
        // /Users/ourcodeworld/Desktop/file.txt
        
        // set the filepath back in the C code.. fill with zeros first
        filepath.assign(repeating: 0, count: bytes)
        filepath.assign(from: path, count: path.count)
            
        print("Picked \(String(cString:filepath))")
        return true
    
    }
    return false
}

@_cdecl("swift_Alert")
public func swift_Alert(_ text: UnsafePointer<CChar>, _ text2: UnsafePointer<CChar>,_ hasCancel:Bool) -> Int
{
    
    let a = NSAlert()
    a.messageText = String(cString: text)
   a.informativeText = String(cString: text2)
   //   .alertFirstButtonReturn
   a.addButton(withTitle: "OK")


   //   .alertSecondButtonReturn
    if hasCancel
    {
        a.addButton(withTitle: "Cancel")
    }
    
    a.alertStyle = .warning
    let res = a.runModal()
    let val = res == .alertFirstButtonReturn ? 1:2
    print(String(cString:text)+" "+String(cString:text2)+" "+String(hasCancel))
	// should reload file list BUT not in this alert.. do it before alert
//	swift_TCReloadFileList()
	return val
}


@_cdecl("swift_saveScreen")
public func swift_saveScreen(_ text: UnsafePointer<CChar>)
{
    print("\(#function) \(text)")
    beebViewControllerInstance?.screenFilename = String(cString: text);
}


@_cdecl("swift_getPasteboard")
public func swift_getPasteboard(_ text: UnsafeMutablePointer<CChar>, _ length: Int) -> Int
{
    let pasteboard = NSPasteboard.general

    if let string = pasteboard.string(forType: .string) {
        // text was found and placed in the "string" constant
        
        // set the filepath back in the C code - fill with zeros first
        text.assign(repeating: 0, count: length)
        text.assign(from: string, count: string.count)
        return 1
    }
    return 0
}


@_cdecl("swift_setPasteboard")
public func swift_setPasteboard(_ text: UnsafePointer<CChar>, _ length: UInt)
{
    let pasteboard = NSPasteboard.general

    pasteboard.clearContents()
    pasteboard.setString(String(cString:text), forType: .string)
}



@_cdecl("swift_Report")
public func swift_Report(_ text: UnsafePointer<CChar>, _ title: UnsafePointer<CChar>,_ buttonType:Int) -> Int
{
	
	let a = NSAlert()
	a.messageText = String(cString: text)
   a.window.title = String(cString: title)
	
	if buttonType == 0x100 //yesno
	{
		a.addButton(withTitle: "Yes")
		a.addButton(withTitle: "No")
	}
	else 	if buttonType == 0x200 //okcancel
	{
		a.addButton(withTitle: "OK")
		a.addButton(withTitle: "Cancel")

	}
	
	a.alertStyle = .warning
	let res = a.runModal()
	let val = res == .alertFirstButtonReturn ? 1:2
//	let val2 = res == .alertSecondButtonReturn ? 1:2
//	print(String(cString:text)+" "+String(cString:title)+" "+String(buttonType))
		print("result "+String(val))
	return val
}


// allow access to this in C
@_cdecl("swift_IsMiniaturized")
public func swift_IsMiniaturized() -> Bool
{
	return NSApp.mainWindow!.isMiniaturized
}
