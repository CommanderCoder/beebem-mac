//
//  BeebEm-Files.swift
//  BeebEm
//
//  Created by Commander Coder on 19/04/2024.
//

import Foundation



@_cdecl("swift_GetDocumentsDirectory")
public func swift_GetDocumentsDirectory( _ resourcePath: UnsafeMutablePointer<CChar>, _ length:Int)
{
	//  Find Document directory
	let fileManager = FileManager.default
	let documentURL = fileManager.urls(for: .documentDirectory, in: .userDomainMask).first!
	
	let dpath = documentURL.path+"/"
	// set the filepath back in the C code - fill with zeros first
	resourcePath.assign(repeating: 0, count: length)
	resourcePath.assign(from: dpath, count: dpath.count)
}

// allow access to this in C
@_cdecl("swift_MoveFile")
public func swift_MoveFile(srcpath : UnsafeMutablePointer<CChar>, destpath : UnsafeMutablePointer<CChar>) -> Bool
{
	let src = String(cString: srcpath)
	let dest = String(cString: destpath)
	do
	{
		try FileManager.default.moveItem(atPath: src, toPath: dest)
	}
	catch  {
		print("Unexpected error: \(error).")
		return false
	}
	return true
}


@_cdecl("swift_Remove")
public func swift_Remove( _ path: UnsafePointer<CChar>) -> Int
{
	let pathStr = String(cString: path)
	do
	{
		let folderURL = NSURL.fileURL(withPath: pathStr, isDirectory: true)
		try FileManager.default.removeItem(at: folderURL)
	}
	catch  {
		print("Unexpected error: \(error).")
		return -1
	}
	return 0
}


// allow access to this in C
@_cdecl("swift_CopyDirectoryRecursively")
public func swift_CopyDirectoryRecursively( _ sourcePath: UnsafePointer<CChar>, _ targetPath:UnsafePointer<CChar>) -> Bool
{
	let sourcePathStr = String(cString: sourcePath)
	let targetPathStr = String(cString: targetPath)

	print( "copy \(sourcePathStr) to \(targetPathStr)");
	
	do {
		if FileManager.default.fileExists(atPath: targetPathStr) {
			try FileManager.default.removeItem(atPath: targetPathStr)
		}
		try FileManager.default.copyItem(atPath: sourcePathStr, toPath:targetPathStr)
	} catch (let error) {
		print("Cannot copy item at \(sourcePathStr) to \(targetPathStr): \(error)")
		return false
	}
	return true
			
}

// File name for the plist in Application Support
let plistFileName = "prefs.plist"
let appSupportBundleID = "BeebEm-mac" // Bundle.main.bundleIdentifier
let fileManager = FileManager.default
let appSupportURL = fileManager.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!
let appSupportBundleURL = appSupportURL.appendingPathComponent(appSupportBundleID)

let plistURL = appSupportBundleURL.appendingPathComponent(plistFileName)

// Function to set a value in the plist
@_cdecl("swift_PListSetValue")
func swift_PListSetValue(_ CCkey: UnsafePointer<CChar>, _ CCvalue: UnsafePointer<CChar>) {
	let key = String(cString: CCkey)
	let value = String(cString: CCvalue)
    
	do {
        // create the bundle folder
		try fileManager.createDirectory(at: appSupportBundleURL, withIntermediateDirectories: true, attributes: nil)

        do {
            var plistData = readPlist(from: plistURL) ?? [String: String]()
            plistData[key] = value
            try writePlist(dictionary: plistData, to: plistURL)
        } catch {
            print("Error updating plist: \(error)")
        }
	} catch {
		print("Error creating directory: \(error)")
	}

}

// Function to get a value from the plist
@_cdecl("swift_PListGetValue")
func swift_PListGetValue(_ CCkey: UnsafePointer<CChar>) -> UnsafeMutablePointer<CChar>? {
	let key = String(cString: CCkey)
	do {
		if let plistData = readPlist(from: plistURL), let value = plistData[key] {
			let cString = strdup(value)
			return UnsafeMutablePointer<CChar>(cString)
		}
	} catch {
		print("Error reading plist: \(error)")
	}
	return nil
}

// Helper functions for read and write
func readPlist(from url: URL) -> [String: String]? {
	guard let data = try? Data(contentsOf: url),
		  let plist = try? PropertyListSerialization.propertyList(from: data, options: [], format: nil) as? [String: String] else {
		return nil
	}
	return plist
}

func writePlist(dictionary: [String: String], to url: URL) throws {
	let data = try PropertyListSerialization.data(fromPropertyList: dictionary, format: .xml, options: 0)
	try data.write(to: url)
}
