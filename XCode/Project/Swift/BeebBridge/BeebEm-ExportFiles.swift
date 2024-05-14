//
//  BeebEm-Bridge-FilesController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation
import Cocoa


// construct the list view

// need to have given the controller an identified (StoryboardID)
let exportFilesWindow: NSWindowController = GetWindowCtrl(for: Modals.exportFiles)

let exportFilesView: ExportDiscViewController = exportFilesWindow.contentViewController as! ExportDiscViewController



@_cdecl("swift_SelectedFiles")
public func swift_SelectedFiles ( selectedFiles : UnsafeMutablePointer<Int32> , length : Int)  -> Int
{
	let beeblist = exportFilesView.beeblistdata
	let numSelected = beeblist.selectedFiles.count

	selectedFiles.assign(repeating: 0, count: length)
	selectedFiles.assign(from: beeblist.selectedFiles, count: numSelected)
	return numSelected
}


// UnsafePointer<UnsafePointer<UnsafePointer<CChar>>>
// const char**  or/ const char * [columns]

// allow access to this in C
@_cdecl("swift_InitExportDialog")
public func swift_InitExportDialog(dfsNames : UnsafePointer<UnsafePointer<CChar>>, files : UInt, columns: UInt)
{
	// fill up the beeblistdata within the view controller
	let beeblist = exportFilesView.beeblistdata
	beeblist.clear();

	let dn = UnsafeBufferPointer(start: dfsNames, count:Int(columns*files))
	var col = 0
	var row = ""
	for c in dn.enumerated()
	{
		let i:UnsafePointer<CChar> = c.element
		// get the rows
		let s = String(cString: i)
		print(s)
		col+=1
		if (col == columns)
		{
			beeblist.setrow(row)
			col = 0
			row = ""
		}
		else
		{
			row+=s+" | "
		}
		
	}

}
