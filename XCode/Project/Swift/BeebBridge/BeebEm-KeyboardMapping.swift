//
//  BeebEm-Bridge-KbdController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation


@_cdecl("swift_uksetasstitle")
public func swift_uksetasstitle( _ text: UnsafePointer<CChar>)
{
	print("\(#function) \(text)")
	kbViewControllerInstance?.setasstitle(String(cString: text));
}



@_cdecl("swift_buttonSetControlValue")
public func swift_buttonSetControlValue(_ cmd: UInt32, _ state: Int)
{
	let cmdSTR = conv(cmd)
	
	kbViewControllerInstance?.buttonSetControlValue(cmdSTR, state);
}
