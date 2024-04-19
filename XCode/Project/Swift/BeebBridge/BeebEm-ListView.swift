//
//  BeebEm-Bridge-FilesController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation



@_cdecl("swift_SelectedFiles")
public func swift_SelectedFiles ( selectedFiles : UnsafeMutablePointer<Int32> , length : Int)  -> Int
{
	let beeblist = ExportDiscViewController.beeblistdata
	let numSelected = beeblist.selectedFiles.count

	selectedFiles.assign(repeating: 0, count: length)
	selectedFiles.assign(from: beeblist.selectedFiles, count: numSelected)
	return numSelected
}


// allow access to this in C
@_cdecl("swift_SelectFiles")
public func swift_SelectFiles(dfsNames : UnsafePointer<UnsafePointer<CChar>>, files : UInt)
{
	let dn = UnsafeBufferPointer(start: dfsNames, count:Int(files))
	
	// fill up the beeblistdata within the view controller
	let beeblist = ExportDiscViewController.beeblistdata
	
	beeblist.clear();
	for i in dn.enumerated()
	{
		let s = String(cString: i.element)
		beeblist.setrow(s)
	}
}
