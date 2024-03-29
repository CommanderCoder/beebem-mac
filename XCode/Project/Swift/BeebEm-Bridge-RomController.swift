//
//  BeebEm-Bridge-RomController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation




@_cdecl("swift_RCSetModelText")
public func swift_RCSetModelText(_ n: UnsafePointer<CChar>)
{
	print(n)
	let z = String( cString: n )
	
	// set model text
	RomConfigViewController.rcViewControllerInstance!.setModelText(z)
	RomConfigViewController.rcViewControllerInstance!.tableView.reloadData()

}
@_cdecl("swift_RCGetSelectionMark")
public func  swift_RCGetSelectionMark() -> Int
{
	RomConfigViewController.rcViewControllerInstance!.tableView.reloadData()
	return RomConfigViewController.rcViewControllerInstance!.returnRowInTable()
}

@_cdecl("swift_RCSetFocus")
public func  swift_RCSetFocus()
{
	RomConfigViewController.rcViewControllerInstance!.setFocus();
}
