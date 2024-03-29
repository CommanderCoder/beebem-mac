//
//  BeebEm-Bridge-TapeController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation


// Tape Control

@_cdecl("swift_TCReload")
public func swift_TCReload()
{
//    print("\(#function) \(String(cString:text)) \(b)")
	TapeControlViewController.tcViewControllerInstance!.reloadFileList()
}


@_cdecl("swift_TCSelectItem")
public func swift_TCSelectItem( _ b:Int )
{
//    print("\(#function) \(String(cString:text)) \(b) ")
	TapeControlViewController.tcViewControllerInstance!.selectRowInTable(UInt(b))
}

@_cdecl("swift_TCReturnItem")
public func swift_TCReturnItem(_ text: UnsafePointer<CChar>) -> UInt
{
//    print("\(#function) \(String(cString:text)) \(b) ")
	return TapeControlViewController.tcViewControllerInstance!.returnRowInTable()
}

@_cdecl("swift_TCReloadFileList")
public func swift_TCReloadFileList()
{
	TapeControlViewController.tcViewControllerInstance!.reloadFileList()
}
