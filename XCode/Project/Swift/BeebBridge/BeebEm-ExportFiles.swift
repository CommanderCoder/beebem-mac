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
let exportFilesWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(
		withIdentifier: "ExportFilesSB") as! NSWindowController
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

// allow access to this in C
@_cdecl("swift_InitDialog")
public func swift_InitDialog(dfsNames : UnsafePointer<UnsafePointer<CChar>>, files : UInt)
{
	let dn = UnsafeBufferPointer(start: dfsNames, count:Int(files))
	
	// fill up the beeblistdata within the view controller
	let beeblist = exportFilesView.beeblistdata
	
	beeblist.clear();
	for i in dn.enumerated()
	{
		let s = String(cString: i.element)
		beeblist.setrow(s)
	}

}


// allow access to this in C
@_cdecl("swift_DoModal")
public func swift_DoModal(d : UnsafeMutableRawPointer)
{
	ExportDiscViewController.caller = d


	// run the modal until it is closed or the Export Selected (::exportSelected) button
	// is pressed
	let modalresp = NSApp.runModal(for: exportFilesWindow.window!)
	
	//now export the files
	exportFilesWindow.close()
	NSApp.stopModal()

	if (modalresp == NSApplication.ModalResponse.OK)
	{
	// this should call the method within ExportFileDialog
		beeb_exportSelected(ExportDiscViewController.caller);
	}
	
}

// allow access to this in C
@_cdecl("swift_EndDialog")
public func swift_EndDialog()
{
//	NSApp.abortModal()
}
