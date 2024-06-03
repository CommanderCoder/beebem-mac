//
//  BeebEm-Bundle.swift
//  BeebEm
//
//  Created by Commander Coder on 19/04/2024.
//

import Foundation



// Get the path to a resource in the bundle
@_cdecl("swift_GetResourcePath")
public func swift_GetResourcePath( _ resourcePath: UnsafeMutablePointer<CChar>, _ length:Int, _ filename: UnsafePointer<CChar>)
{
	let filenameString = String(cString: filename)
	
	if let fileurl = Bundle.main.url(forResource:filenameString,withExtension: nil)
	{
		let path: String = fileurl.path
		print("\(#function) \(path)")

		// set the filepath back in the C code - fill with zeros first
		resourcePath.assign(repeating: 0, count: length)
		resourcePath.assign(from: path, count: path.count)
	}
	
}


// Get the bundle directory
@_cdecl("swift_GetBundleDirectory")
public func swift_GetBundleDirectory( _ bundlePath: UnsafeMutablePointer<CChar>, _ length:Int)
{
	let dpath : String = Bundle.main.resourcePath!+"/"
	print("\(#function) \(dpath)")
	// set the filepath back in the C code - fill with zeros first
	bundlePath.assign(repeating: 0, count: length)
	bundlePath.assign(from: dpath, count: dpath.count)
}
