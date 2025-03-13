//
//  BeebEm-Files.swift
//  BeebEm
//
//  Created by Commander Coder on 19/04/2024.
//

import Foundation


// allow access to this in C
@_cdecl("swift_GetApplicationSupportDirectory")
public func swift_GetApplicationSupportDirectory( _ resourcePath: UnsafeMutablePointer<CChar>, _ length:Int)
{
	
	/* discover Application Support and Resources folder
	 Opening files will always start at Documents or folder
	 https://developer.apple.com/library/archive/documentation/FileManagement/Conceptual/FileSystemProgrammingGuide/MacOSXDirectories/MacOSXDirectories.html#//apple_ref/doc/uid/TP40010672-CH10-SW1
	 
	 https://developer.apple.com/library/archive/documentation/FileManagement/Conceptual/FileSystemProgrammingGuide/AccessingFilesandDirectories/AccessingFilesandDirectories.html#//apple_ref/doc/uid/TP40010672-CH3-SW7
	 */
	
	
//        let patharray = FileManager.default.urls(for: .applicationSupportDirectory,
//                                                 in: .userDomainMask)
//        let userDataDirectory = patharray[0].appendingPathComponent("UserData")
//
//        print (userDataDirectory.path)
//
//        let teletextpath = Bundle.main.url(forResource: "teletext", withExtension: "fnt")!
//        let z80path = Bundle.main.url(forResource: "Z80", withExtension: "ROM",subdirectory: "UserData/BeebFile")!
//        let rompath = Bundle.main.url(forResource: "Roms", withExtension: "cfg", subdirectory: "UserData")!
//        let allpath = Bundle.main.urls( forResourcesWithExtension: nil, subdirectory: "UserData")!
//
//        print (teletextpath)
//        print (z80path)
//        print (rompath)
//        print (allpath)

//        if let fileContents = try? String(contentsOf: rompath) {
//            // we loaded the file into a string!
//            print(fileContents)
//        }
//
//        if let fileContents = try? Data(contentsOf: z80path) {
//            // we loaded the file into a binary datablock!
//            print(fileContents.description)
//            print(fileContents.base64EncodedString(options: .lineLength64Characters))
//            print(String(decoding: fileContents, as: UTF8.self))
//        }
	
	
	
	
	do
	{
		//  Find Application Support directory
		let fileManager = FileManager.default
		let appSupportURL = fileManager.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!
		//  Create subdirectory
		let directoryURL = appSupportURL.appendingPathComponent("BeebEm-mac")
		try fileManager.createDirectory (at: directoryURL, withIntermediateDirectories: true, attributes: nil)
		
		let dpath = directoryURL.path+"/"
		// set the filepath back in the C code - fill with zeros first
		resourcePath.assign(repeating: 0, count: length)
		resourcePath.assign(from: dpath, count: dpath.count)

	}
	catch
	{
		print("Couldn't find the ApplicationSupportDirectory")
	}
}

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


/** GEMINI AI GENERATED CODE **/


// File name for the plist in Application Support
let plistFileName = "prefs.plist"


// Function to set a value in the plist
@_cdecl("swift_PListSetValue")
func swift_PListSetValue(_ CCkey: UnsafePointer<CChar>, _ CCvalue: UnsafePointer<CChar>) {
    
    let key = String(cString: CCkey)
    let value = String(cString: CCvalue)

    let fileManager = FileManager.default
    do {
        let appSupportURL = try fileManager.url(for: .applicationSupportDirectory, in: .userDomainMask, appropriateFor: nil, create: true)
        if let bundleIdentifier = Bundle.main.bundleIdentifier {
            let appDirectoryURL = appSupportURL.appendingPathComponent(bundleIdentifier, isDirectory: true)
            try fileManager.createDirectory(at: appDirectoryURL, withIntermediateDirectories: true, attributes: nil)
            let plistURL = appDirectoryURL.appendingPathComponent(plistFileName)

            // Read existing plist or create a new one
            var plistData = readPlist(from: plistURL) ?? [:]

            // Set the value
            plistData[key] = value

            // Write the updated plist
            try writePlist(dictionary: plistData, to: plistURL)
        } else {
            print("Error: Could not get bundle identifier.")
        }
    } catch {
        print("Error in swift_PListSetValue: \(error)")
    }
}

// Function to get a value from the plist
@_cdecl("swift_PListGetValue")
func swift_PListGetValue(_ CCkey: UnsafePointer<CChar>) -> UnsafeMutablePointer<CChar>? {
    
    let key = String(cString: CCkey)

    let fileManager = FileManager.default
    do {
        let appSupportURL = try fileManager.url(for: .applicationSupportDirectory, in: .userDomainMask, appropriateFor: nil, create: false)
        if let bundleIdentifier = Bundle.main.bundleIdentifier {
            let appDirectoryURL = appSupportURL.appendingPathComponent(bundleIdentifier, isDirectory: true)
            let plistURL = appDirectoryURL.appendingPathComponent(plistFileName)

            if let plistData = readPlist(from: plistURL) {
                
                guard let cString = plistData[key]?.cString(using: .utf8) else {
                    return nil
                }

                let copiedCString = strdup(cString) // Copy to manage lifetime
                return UnsafeMutablePointer<CChar>(mutating: copiedCString)
                
            } else {
                return nil // Plist file doesn't exist or is invalid
            }
        } else {
            print("Error: Could not get bundle identifier.")
            return nil
        }
    } catch {
        print("Error in swift_PListGetValue: \(error)")
        return nil
    }
}

// Helper functions for read and write
func readPlist(from url: URL) -> [String: String]? {
    do {
        let data = try Data(contentsOf: url)
        if let plist = try PropertyListSerialization.propertyList(from: data, options: [], format: nil) as? [String: String] {
            return plist
        } else {
            return nil
        }
    } catch {
        return nil
    }
}

func writePlist(dictionary: [String: Any], to url: URL) throws {
    let data = try PropertyListSerialization.data(fromPropertyList: dictionary, format: .xml, options: 0)
    try data.write(to: url)
}

/** END GEMINI AI GENERATED CODE **/
