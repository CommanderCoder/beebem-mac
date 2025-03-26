//
//  BeebEm-Bridging-Code.swift
//
//  Created by Commander Coder on 06/07/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

import Foundation
import Cocoa
import AVFoundation
import AppKit




//https://developer.apple.com/documentation/uniformtypeidentifiers/uttype
//https://useyourloaf.com/blog/swiftui-importing-and-exporting-files/

// probably use imported type rather than exported as we aren't the 'canonical' source of
// these types.  That's someone else
@available(macOS 11.0, *)
extension UTType {
	public static let romcfg = UTType(filenameExtension: "cfg")!
	public static let ssd = UTType(importedAs: "com.commandercoder.BeebEm.ssd")
	public static let hdd = UTType(importedAs: "com.commandercoder.BeebEm.dat")
	public static let tape = UTType(importedAs: "com.commandercoder.BeebEm.uef")
	public static let rom = UTType(importedAs: "com.commandercoder.BeebEm.rom")

	//do these really need to be exportedAs?
		
	public static let keymap = UTType(exportedAs: "com.commandercoder.BeebEm.kmap")
	public static let uefstate = UTType(exportedAs: "com.commandercoder.BeebEm.uefstate")
//	public static let ssdout = UTType(exportedAs: "com.commandercoder.BeebEm.ssdout")
}

func GetFilters(filterString s : String) -> ([String: String],[String])
{
	/*
	 The first string in each pair is a display string that describes the filter (for example, "Text Files"), and the second string specifies the filter pattern (for example, ".TXT"). To specify multiple filter patterns for a single display string, use a semicolon to separate the patterns (for example, ".TXT;.DOC;.BAK"). A pattern string can be a combination of valid file name characters and the asterisk (*) wildcard character. Do not include spaces in the pattern string.
	 */
	
	// including a 'descriptor' array so that the order of the options doesn't change
    // as it would if an array of dictionary.keys were used.
	var descriptor: [String] = []
	var desc_pattern = [String: String]()
    
    // l = the end of the long string indicated by \0\0
    if let l = s.range(of: "\0\0")?.lowerBound
    {
        let t = String(s[..<l]) // the sliced string
        let element = t.components(separatedBy: "\0")
        // each even element (starting at 0) is a descriptor, each odd element is the filter pattern
        for i in stride(from: 0, to: element.count, by:2) {
            descriptor.append(element[i])
            desc_pattern[element[i]] = element[i+1]
        }
    }
	
	return (desc_pattern, descriptor)
}

func GetContentDictionary(filterString filters : String) -> [String:String]
{
    var desc_pattern = [String: String]()
    (desc_pattern, _) = GetFilters(filterString: filters)
    return desc_pattern
}



// get an array of content types based on the values in the dictionary (the filter patterns)
@available(macOS 11.0, *)
func GetContentType(_ dictionary : [String:String]) -> [UTType]?
{
	var ct : [UTType] = [] // .ANYFILE
	
	if dictionary.values.contains("*.ssd") // DISC
	{
        ct.append(.ssd) // ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"]
	}
	if dictionary.values.contains("*.uefstate") //UEFSTATEFILE
	{
		Swift.print("found uefstate.")
        ct.append(.uefstate) // ["uefstate", "csw"]
	}
    if dictionary.values.contains("*.uef") //UEFFILE
	{
		Swift.print("found uef.")
        ct.append(.tape) // ["uef", "csw"]
	}
	if dictionary.values.contains("*.uef") //IFD
	{
		Swift.print("found uefstate.")
        ct.append(.tape) // ["uef", "csw"]
	}
	if dictionary.values.contains("*.kmap") //KEYBOARD
	{
		Swift.print("found Key Map File.")
		ct.append(.keymap) // ["kmap"]
	}
	if dictionary.values.contains("*.dat") //HARDDRIVE
	{
		Swift.print("found hdd.")
		ct.append(.hdd) //nil  // ["dat"]
	}
	if dictionary.values.contains("*.inf") //DISCFILE
	{
		Swift.print("found inf.")
		ct.append(.data) //nil  // ["inf"]
	}
	if dictionary.values.contains("*.cfg") //ROMCFG
	{
		Swift.print("found rom config file.")
		ct.append(.romcfg ) // ["cfg"]
	}
	if dictionary.values.contains("*.*") //PRINTFILE
	{
		Swift.print("found *.*")
		ct.append(.plainText)
	}

	return ct
}

@available(macOS, introduced: 10.0, deprecated: 11.0, renamed: "GetContentType")
func GetFileType(_ dictionary : [String:String]) -> [String]
{
	var ct : [String] = []
	
    // quick exit if *.*
    if dictionary.values.contains("*.*") //PRINTFILE
    {
        Swift.print("found *.*")
        return ct
    }
    
	if dictionary.values.contains("*.ssd") // DISC
	{
        ct.append( contentsOf: ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"] )
	}
	if dictionary.values.contains("*.uefstate") //UEFSTATEFILE
	{
		Swift.print("found uefstate.")
		ct.append( contentsOf: ["uefstate"])
	}
	if dictionary.values.contains("*.uef") //UEFFILE
	{
		Swift.print("found uef.")
		ct.append( contentsOf:["uef", "csw"])
	}
	if dictionary.values.contains("*.ifd") //IFD
	{
		Swift.print("found ifd.")
		ct.append( contentsOf:["ifd"])
	}
    if dictionary.values.contains("*.kmap") //KEYBOARD
	{
		Swift.print("found Key Map File.")
		ct.append( contentsOf:["kmap"])
	}
    if dictionary.values.contains("*.dat") //HARDDRIVE
	{
		Swift.print("found hdd.")
		ct.append( contentsOf:["dat"])
	}
    if dictionary.values.contains("*.inf") //DISCFILE
	{
		Swift.print("found inf.")
		ct.append( contentsOf:["inf"])
	}
    if dictionary.values.contains("*.cfg") //ROMCFG
	{
		Swift.print("found rom config file.")
		ct.append( contentsOf:["cfg"])
	}
    
	return ct
}

	

// allow access to this in C
@_cdecl("swift_GetFilesWithPreview")
func swift_GetFilesWithPreview(filepath : UnsafeMutablePointer<CChar>, bytes: Int, directory : UnsafeMutablePointer<CChar>, multiFiles : Bool = false, filefilter : UnsafePointer<CUnsignedChar>, title : UnsafeMutablePointer<CChar>?) -> Int
{
    let dialog = NSOpenPanel()
	let fileFilterBytes = UnsafeBufferPointer(start: filefilter, count: 1000) // 1000 should be large enough!s
    let filters = String(bytes:fileFilterBytes, encoding: .ascii)!
    let filter_patterns = GetContentDictionary(filterString: filters)
    // FUTURE - need to expand the path if it includes ~
    //    let launcherLogPath = NSString("~/Documents").expandingTildeInPath
    let launcherLogPath = String( cString: directory)

    if let pointer = title
    {
        dialog.message =  String(cString: pointer)
    }
    else
    {
        dialog.message = "Select File"
    }
    
    dialog.showsResizeIndicator    = true
    dialog.showsHiddenFiles        = false
    dialog.allowsMultipleSelection = multiFiles
    dialog.canChooseDirectories    = false
	dialog.canChooseFiles		   = true
	dialog.allowsOtherFileTypes	   = true
    dialog.showsTagField = false
    dialog.isExtensionHidden = false
    // dialog.nameFieldStringValue = "somefile.txt"

	// Defaults in FileDialog-mac.cpp are:
	//    OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	//    OFN_OVERWRITEPROMPT can be false
	
    dialog.directoryURL = NSURL.fileURL(withPath: launcherLogPath, isDirectory: true)
    dialog.canCreateDirectories = true
	
	if #available(macOS 11.0, *) {
		dialog.allowedContentTypes = GetContentType(filter_patterns) ?? []
	} else {
		dialog.allowedFileTypes = GetFileType(filter_patterns)
	}

    // open the LOAD dialog
    if (dialog.runModal() == NSApplication.ModalResponse.OK) {
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
    let dialog = NSOpenPanel()
    dialog.title                   = "Select Folder"
	dialog.message                 = String(cString: title)
	dialog.showsResizeIndicator    = true
    dialog.canChooseDirectories    = true
    dialog.canChooseFiles          = false
    dialog.canCreateDirectories    = true
    
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
func swift_SaveFile(filepath : UnsafeMutablePointer<CChar>, bytes: Int, filefilter : UnsafePointer<CUnsignedChar>, initialDir : UnsafePointer<CUnsignedChar>) -> Bool
{
    // MUST be a buffer of characters because /0 is used as a separator and we'd only
    // get the first entry if we converted from a CString to a Swift String
    let fileFilterBytes = UnsafeBufferPointer(start: filefilter, count: 1000) // 1000 should be large enough!!
    let filters = String(bytes:fileFilterBytes, encoding: .ascii)!
    let filter_patterns = GetContentDictionary(filterString: filters)

    let initDir = String(cString: initialDir)
    
    let dialog = NSSavePanel()
    
    dialog.message = "Select File"

    dialog.showsResizeIndicator    = true
    dialog.showsHiddenFiles        = false
    dialog.allowsOtherFileTypes    = true
    dialog.showsTagField           = false
    dialog.isExtensionHidden       = false
    dialog.directoryURL            = NSURL.fileURL(withPath: initDir, isDirectory: true)
    dialog.canCreateDirectories    = true
    
    if #available(macOS 11.0, *) {
        dialog.allowedContentTypes = GetContentType(filter_patterns) ?? []
    } else {
        dialog.allowedFileTypes = GetFileType(filter_patterns)
    }

    // open the SAVE dialog
    if (dialog.runModal() == NSApplication.ModalResponse.OK) {
        guard let result = dialog.url else { return false } // Pathname of the file
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
public func swift_getPasteboard(_ text: UnsafeMutablePointer<CChar>, _ length: Int) -> Bool
{
    let pasteboard = NSPasteboard.general

    if let string = pasteboard.string(forType: .string) {
        // text was found and placed in the "string" constant
        
        // set the clipboard back in the C code - fill with zeros first
        text.assign(repeating: 0, count: length)
        text.assign(from: string, count: string.count)
        return true
    }
    return false
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
