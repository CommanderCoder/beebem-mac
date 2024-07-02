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

func GetContentDictionary(filterString f : String?) -> ([String: String],[String])
{
	/*
	 The first string in each pair is a display string that describes the filter (for example, "Text Files"), and the second string specifies the filter pattern (for example, ".TXT"). To specify multiple filter patterns for a single display string, use a semicolon to separate the patterns (for example, ".TXT;.DOC;.BAK"). A pattern string can be a combination of valid file name characters and the asterisk (*) wildcard character. Do not include spaces in the pattern string.
	 */
	
	// using a 'names' array so that the order of the options doesn't keep changing as it would if the Array(dictionary.keys) were used.
	var names: [String] = []
	
	var dictionary = [String: String]()
	if let s = f
	{
		if let l = s.range(of: "\0\0")?.lowerBound
		{
			let t = String(s[..<l])
			let pairs = t.components(separatedBy: "\0")
			for i in stride(from: 0, to: pairs.count, by:2) {
				names.append(pairs[i])
				dictionary[pairs[i]] = pairs[i+1]
			}
		}
	}
	
	return (dictionary,names)
}


@available(macOS 11.0, *)
func GetContentType(_ dictionary : [String:String]) -> [UTType]?
{
	var ct : [UTType]? = [] // .ANYFILE
	
	if dictionary.values.contains("*.ssd") // DISC
	{
		ct = [.ssd] // ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"]
	}
	else if dictionary.values.contains("*.uefstate") //UEFSTATEFILE
	{
		Swift.print("found uefstate.")
		ct = [.uefstate] // ["uefstate", "csw"]
	}
	else if dictionary.values.contains("*.uef") //UEFFILE
	{
		Swift.print("found uef.")
		ct = [.tape] // ["uef", "csw"]
	}
	else if dictionary.values.contains("*.uef") //IFD
	{
		Swift.print("found uefstate.")
		ct = [.tape] // ["uef", "csw"]
	}
	else if dictionary.values.contains("*.kmap") //KEYBOARD
	{
		Swift.print("found Key Map File.")
		ct = [.keymap] // ["kmap"]
	}
	else if dictionary.values.contains("*.dat") //HARDDRIVE
	{
		Swift.print("found hdd.")
		ct = [.hdd] //nil  // ["dat"]
	}
	else if dictionary.values.contains("*.inf") //DISCFILE
	{
		Swift.print("found inf.")
		ct = [.data] //nil  // ["inf"]
	}
	else if dictionary.values.contains("*.cfg") //ROMCFG
	{
		Swift.print("found rom config file.")
		ct = [.romcfg ] // ["cfg"]
	}
	else if dictionary.values.contains("*.*") //PRINTFILE
	{
		Swift.print("found *.*")
		ct = [.plainText]
	}

	return ct
}

@available(macOS, introduced: 10.0, deprecated: 11.0, renamed: "GetContentType")
func GetFileType(_ dictionary : [String:String]) -> [String]
{
	var ct : [String] = []
	
	if dictionary.values.contains("*.ssd") // DISC
	{
		ct = ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"]
	}
	else if dictionary.values.contains("*.uefstate") //UEFSTATEFILE
	{
		Swift.print("found uefstate.")
		ct = ["uefstate"]
	}
	else if dictionary.values.contains("*.uef") //UEFFILE
	{
		Swift.print("found uef.")
		ct = ["uef", "csw"]
	}
	else if dictionary.values.contains("*.ifd") //IFD
	{
		Swift.print("found ifd.")
		ct = ["ifd"]
	}
	else if dictionary.values.contains("*.kmap") //KEYBOARD
	{
		Swift.print("found Key Map File.")
		ct = ["kmap"]
	}
	else if dictionary.values.contains("*.dat") //HARDDRIVE
	{
		Swift.print("found hdd.")
		ct = ["dat"]
	}
	else if dictionary.values.contains("*.inf") //DISCFILE
	{
		Swift.print("found inf.")
		ct = ["inf"]
	}
	else if dictionary.values.contains("*.cfg") //ROMCFG
	{
		Swift.print("found rom config file.")
		ct = ["cfg"]
	}
	else if dictionary.values.contains("*.*") //PRINTFILE
	{
		Swift.print("found *.*")
		ct = []
	}
	
	return ct
}

	

// allow access to this in C
@_cdecl("swift_GetFilesWithPreview")
func swift_GetFilesWithPreview(filepath : UnsafeMutablePointer<CChar>, bytes: Int, directory : UnsafeMutablePointer<CChar>, multiFiles : Bool = false, filefilter : UnsafePointer<CUnsignedChar>) -> Int
{
    let dialog = NSOpenPanel()
	let s = UnsafeBufferPointer(start: filefilter, count: 1000) // 1000 should be large enough!s

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
	
	
	let (dictionary, _) = GetContentDictionary(filterString: String(bytes:s, encoding: .ascii))
	
	if #available(macOS 11.0, *) {
		dialog.allowedContentTypes = GetContentType(dictionary) ?? []
	} else {
		dialog.allowedFileTypes = GetFileType(dictionary)
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






//https://christiantietze.de/posts/2020/05/create-nssavepanel-accessoryview-in-swift/
class SaveDelegate: NSObject, NSOpenSavePanelDelegate {
	
	func panelSelectionDidChange(_ sender: Any?)
	{
		Swift.print("panelSelectionDidChange \(String(describing: sender))")
	}

	func panel(_ sender: Any, userEnteredFilename filename: String, confirmed okFlag: Bool) -> String? {
		Swift.print("panel \(String(describing: sender))")
		Swift.print("panel \(filename)")
		return filename
	}
	
	func panel(_ sender: Any, shouldEnable url: URL) -> Bool
	{
		return true;
	}
	
//	func panel(_ sender: Any, validate url: URL) throws
//	{
//
//	}
	func panel(_ sender: Any, didChangeToDirectoryURL url: URL?)
	{
		
	}
}


@available(macOS 11.0, *)
var contentTypes : [UTType] = []

@available(macOS, introduced: 10.0, deprecated: 11.0, renamed: "GetContentType")
var fileTypes : [String] = []

class SaveDocument: NSDocument {
	
	internal init(filterString f : String?, initialDirectory d : String?)
	{
		(dictionary,names) = GetContentDictionary(filterString:f)
		if #available(macOS 11.0, *) {
			contentTypes = GetContentType(dictionary) ?? []
		} else {
			fileTypes = GetFileType(dictionary)
		}
		
		initialDir = d
	}
	
	internal var fileurl : URL?
	override func save(
		to url: URL,
		ofType typeName: String,
		for saveOperation: NSDocument.SaveOperationType,
		completionHandler: @escaping ((any Error)?) -> Void
	)
	{
		Swift.print(url, typeName)
		fileurl = url
	}

	override func fileNameExtension(forType typeName: String, saveOperation: NSDocument.SaveOperationType) -> String? {
		Swift.print("file name extension \(typeName)")
		return String((dictionary[typeName]?.dropFirst() )!)
		
	}

	override func writableTypes(for saveOperation: NSDocument.SaveOperationType) -> [String]
	{
		return names
	}
	
	var names = [String]()
	var dictionary = [ String: String]()
	var initialDir : String?
	
	override func prepareSavePanel(_ savePanel: NSSavePanel) -> Bool {

		let dialog=savePanel
		let delegate = SaveDelegate()

		dialog.delegate = delegate
		dialog.title                   = "Choose a file | BeebEm5"
		dialog.allowsOtherFileTypes    = true
		dialog.isExtensionHidden	= false
		dialog.showsTagField = false
		dialog.nameFieldStringValue = "somefile.txt"
		//	dialog.showsResizeIndicator    = true
		//	dialog.showsHiddenFiles        = false
		//	dialog.canSelectHiddenExtension	= true
		if !(initialDir?.isEmpty ?? true) {
			dialog.directoryURL = NSURL.fileURL(withPath: initialDir!, isDirectory: true)
		}

		if #available(macOS 11.0, *) {
			dialog.allowedContentTypes = contentTypes
		} else {
			dialog.allowedFileTypes = fileTypes
		}
		
		return true
	}
}


@_cdecl("swift_SaveFile")
func swift_SaveFile(filepath : UnsafeMutablePointer<CChar>, bytes: Int, filefilter : UnsafePointer<CUnsignedChar>, initialDir : UnsafePointer<CUnsignedChar>) -> Bool
{
	// MUST be a buffer of characters because /0 is used as a separator
	let s = UnsafeBufferPointer(start: filefilter, count: 1000) // 1000 should be large enough!!
	let d = String(cString: initialDir)
	let sav = SaveDocument(filterString: String(bytes:s, encoding: .ascii), initialDirectory: d)
	
	sav.runModalSavePanel(for:.saveOperation, delegate: nil , didSave:
								nil, contextInfo: nil)
	// fileurl in SaveDocument will contain the file IF
	// it SAVE was pressed
	guard let result = sav.fileurl else { return false} // Pathname of the file

	let path: String = result.path
	
	// path contains the file path e.g
	// /Users/ourcodeworld/Desktop/file.txt
	
	// set the filepath back in the C code.. fill with zeros first
	filepath.assign(repeating: 0, count: bytes)
	filepath.assign(from: path, count: path.count)
		
	print("Picked \(String(cString:filepath))")
	return true
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
