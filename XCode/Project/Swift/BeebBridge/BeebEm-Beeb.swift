//
//  BeebEm-Beeb.swift
//  BeebEm
//
//  Created by Commander Coder on 19/04/2024.
//

import Foundation


@objc public enum MachineModel : UInt16 {
	
	case B = 0     // 0: BBC B
	case IntegraB  // 1: BBC B with Integra B
	case BPlus     // 2: BBC B+
	case Master128 // 3: BBC Master 128

}

//option set (bit flags)
struct MachineTypeFlags: OptionSet
{
	let rawValue: UInt16

	static let B            = MachineTypeFlags(rawValue: 1 << MachineModel.B.rawValue)
	static let IntegraB     = MachineTypeFlags(rawValue: 1 << MachineModel.IntegraB.rawValue)
	static let BPlus        = MachineTypeFlags(rawValue: 1 << MachineModel.BPlus.rawValue)
	static let Master128    = MachineTypeFlags(rawValue: 1 << MachineModel.Master128.rawValue)
}


@objc public enum KB_LEDs : UInt16 {
	case CASS = 0
	case CAPS
	case SHIFT
	case HD0
	case HD1
	case HD2
	case HD3
	case FD0
	case FD1
}


//option set (bit flags)
struct LEDFlags: OptionSet
{
	let rawValue: UInt16

	static let CassLED  = LEDFlags(rawValue: 1 << KB_LEDs.CASS.rawValue)
	static let CapsLED  = LEDFlags(rawValue: 1 << KB_LEDs.CAPS.rawValue)
	static let ShiftLED = LEDFlags(rawValue: 1 << KB_LEDs.SHIFT.rawValue)
	static let HD0LED  = LEDFlags(rawValue: 1 << KB_LEDs.HD0.rawValue)
	static let HD1LED  = LEDFlags(rawValue: 1 << KB_LEDs.HD1.rawValue)
	static let HD2LED  = LEDFlags(rawValue: 1 << KB_LEDs.HD2.rawValue)
	static let HD3LED  = LEDFlags(rawValue: 1 << KB_LEDs.HD3.rawValue)
	static let FD0LED  = LEDFlags(rawValue: 1 << KB_LEDs.FD0.rawValue)
	static let FD1LED  = LEDFlags(rawValue: 1 << KB_LEDs.FD1.rawValue)
}

enum CBridge {
	static var windowTitle = "-"
	static var nextCPU = 0
	static var leds: LEDFlags = []
	static var machineType: MachineTypeFlags = []
}
@_cdecl("swift_SetWindowTitleWithCString")
public func swift_SetWindowTitleWithCString( title: UnsafePointer<CChar> )
{
	CBridge.windowTitle = String(cString: title)
}

@_cdecl("swift_sleepCPU")
public func swift_sleepCPU( microseconds: Int)
{
	// increment the nextCPU time
	CBridge.nextCPU += microseconds
}


@_cdecl("swift_SetLED")
public func swift_SetLED(_ led: KB_LEDs, _ value: Bool)
{
	// CASS,CAPS,SHIFT from KB_LEDs
	let ledf: LEDFlags = LEDFlags(rawValue: 1 << led.rawValue)

	if (value) {
		CBridge.leds.insert(ledf)
	} else {
		CBridge.leds.remove(ledf)
	}
}


@_cdecl("swift_SetMachineType")
public func swift_SetMachineType(_ type: MachineModel)
{
	let mtf: MachineTypeFlags = MachineTypeFlags(rawValue: 1 << type.rawValue)

	CBridge.machineType = mtf
}
