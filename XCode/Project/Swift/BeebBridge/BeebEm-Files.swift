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
	do
	{
		//  Find Document directory
		let fileManager = FileManager.default
		let documentURL = fileManager.urls(for: .documentDirectory, in: .userDomainMask).first!
		
		let dpath = documentURL.path+"/"
		// set the filepath back in the C code - fill with zeros first
		resourcePath.assign(repeating: 0, count: length)
		resourcePath.assign(from: dpath, count: dpath.count)


	}
	catch
	{
		print("Couldn't find the DocumentsDirectory")
	}
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
