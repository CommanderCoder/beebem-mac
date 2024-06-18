//
//  BeebEm-Bridge-KbdController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation
import Cocoa

import Carbon

import IOKit
import IOKit.hid


// need to have given the controller an identified (StoryboardID)
let keyMapWindow: NSWindowController = GetWindowCtrl(for:Dialogs.keyboardMapping)

let keyMapView: KeyboardMappingViewController = keyMapWindow.contentViewController as! KeyboardMappingViewController



@_cdecl("swift_UKSetAssignedTo")
public func swift_UKSetAssignedTo( _ text: UnsafePointer<CChar>)
{
	print("\(#function) \(text)")
	keyMapView.setAssignedToText(String(cString: text));
}


@_cdecl("swift_buttonSetControlValue")
public func swift_buttonSetControlValue(_ cmd: UInt32, _ state: Int)
{
	let cmdSTR = conv(cmd)
	
	keyMapView.buttonSetControlValue(cmdSTR, state);
}


@_cdecl("swift_UserKeyboardDialog")
public func swift_UserKeyboardDialog() -> Bool
{
	let modalresp = NSApp.runModal(for: keyMapWindow.window!)
	keyMapWindow.close()
	NSApp.stopModal()

	if (modalresp == NSApplication.ModalResponse.OK)
	{
		return true
	}
	return false
}





// need to have given the controller an identified (StoryboardID)
let selectKeyWindow: NSWindowController = GetWindowCtrl(for:Dialogs.selectKey)

let selectKeyView: SelectKeyViewController = selectKeyWindow.contentViewController as! SelectKeyViewController


func CONVERT_MAGNITUDE(_ x:Int) -> Int { return  (((x)*10000) / 0x7FFF) }

// adventures in swift
// https://gist.github.com/yamaya/e64cc5d0fdf8c31f0dd1f2a7f775130e

let hidManager = IOHIDManagerCreate(kCFAllocatorDefault, IOOptionBits(kIOHIDOptionsTypeNone))

let deviceCriteria:NSArray = [
	[
		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
		kIOHIDDeviceUsageKey: kHIDUsage_GD_Joystick
	],
	[
		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
		kIOHIDDeviceUsageKey: kHIDUsage_GD_GamePad
	]
	,
//	[
//		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
//		kIOHIDDeviceUsageKey: kHIDUsage_GD_Hatswitch
//	]
//	,
	[
		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
		kIOHIDDeviceUsageKey: kHIDUsage_GD_MultiAxisController
	],
]

let versionKey = kIOHIDVersionNumberKey as CFString
let vendorKey = kIOHIDVendorIDKey as CFString
let productKey = kIOHIDProductKey as CFString
let productIDKey = kIOHIDProductIDKey as CFString
let usagePageKey = kIOHIDPrimaryUsagePageKey as CFString
let usageKey = kIOHIDPrimaryUsageKey as CFString
let manufacturerKey = kIOHIDManufacturerKey as CFString

@_cdecl("swift_JoystickCapture")
func swift_JoystickCapture() -> Bool
{
	guard let runloop = CFRunLoopGetCurrent() else { return false };

	if IOHIDManagerOpen(hidManager, IOOptionBits(kIOHIDOptionsTypeNone)) != kIOReturnSuccess
	{
		return false
	}

	IOHIDManagerSetDeviceMatchingMultiple(hidManager, deviceCriteria)
	IOHIDManagerRegisterDeviceMatchingCallback(hidManager,JoystickDeviceWasAddedCallback,nil)

	IOHIDManagerScheduleWithRunLoop(hidManager, runloop, CFRunLoopMode.defaultMode.rawValue)
	
	while CFRunLoopRunInMode(CFRunLoopMode.defaultMode, 0, true) == .handledSource
	{
		/* no-op. Callback fires once per existing device. */
	}

	return true
}


// class so that this is a reference type
class recElement
{
	var elementRef : IOHIDElement? = nil
	var cookie : IOHIDElementCookie? = nil
	var usagePage : Int = 0
	var usage : Int = 0/* HID usage */
	var min : Int = 0                /* reported min value possible */
	var max : Int = 0               /* reported max value possible */

	/* runtime variables used for auto-calibration */
	var minReport : Int = 0/* min returned value */
	var maxReport : Int = 0 /* max returned value */
	
	init(_ element: IOHIDElement, usagePage:Int, usage: Int)
	{
		let minval = IOHIDElementGetLogicalMin(element)
		let maxval = IOHIDElementGetLogicalMax(element)
		
		self.elementRef = element
		self.cookie = IOHIDElementGetCookie(element)
		self.usagePage = usagePage
		self.usage = usage
		self.min = minval
		self.minReport = minval
		self.max = maxval
		self.maxReport = maxval
	}

}

class recDevice
{
	var deviceHID : IOHIDDevice?  /* HIDManager device handle */
//	io_service_t ffservice;   /* Interface for force feedback, 0 = no ff */
//	FFDeviceObjectReference ffdevice;
//	FFEFFECT *ffeffect;
//	FFEffectObjectReference ffeffect_ref;
//	SDL_bool ff_initialized;

	var product: String = " " /* name of product */
	var usage: Int = 0    /* usage page from IOUSBHID Parser.h which defines general usage */
	var usagePage: Int = 0/* usage within above page from IOUSBHID Parser.h which defines specific usage */

	var axes: Int = 0    /* number of axis (calculated, not reported by device) */
	var buttons: Int = 0/* number of buttons (calculated, not reported by device) */
	var hats: Int = 0    /* number of hat switches (calculated, not reported by device) */
	var elements: Int = 0/* number of total elements (should be total of above) (calculated, not reported by device) */

	var axisList:[recElement] = [] // firstAxis
	var buttonList:[recElement] = []//firstButton
	var hatList:[recElement] = [] //firstHat

	var removed: Bool = false
//	SDL_Joystick *joystick;
	var runLoopAttached : Bool = false /* is 'deviceRef' attached to a CFRunLoop? */

	var instance_id: Int = 0
//	SDL_JoystickGUID guid;
//	int steam_virtual_gamepad_slot;
	
}
var deviceList : [recDevice] = []



func JoystickAlreadyKnown(_ d: IOHIDDevice) -> Bool
{
	return deviceList.contains {$0.deviceHID == d}
}

func AddElement(to list: inout [recElement], element: IOHIDElement, cookie : IOHIDElementCookie, usagePage: Int, usage: Int) -> Bool
{
	if list.isEmpty || !list.contains(where: { $0.cookie ?? 0 == cookie })
	{
		// find element with usage >= element.usage
		let index = list.firstIndex(where: {usage >= $0.usage} ) ?? 0
		// insert element here
		list.insert(
			recElement(element, usagePage: usagePage, usage: usage),
			at: index)
		return true
	}
	return false
}

func AddHIDElement(_ element : IOHIDElement, _ device : inout recDevice)
{
	let elementTypeID = CFGetTypeID(element)

	if (elementTypeID == IOHIDElementGetTypeID())
	{
		let cookie = IOHIDElementGetCookie(element)
		let usagePage = Int(IOHIDElementGetUsagePage(element))
		let usage = Int(IOHIDElementGetUsage(element))
		/* look at types of interest */
		switch (IOHIDElementGetType(element))
		{
		case kIOHIDElementTypeInput_Misc: fallthrough
		case kIOHIDElementTypeInput_Button: fallthrough
		case kIOHIDElementTypeInput_Axis:
		
			switch (usagePage) { /* only interested in kHIDPage_GenericDesktop and kHIDPage_Button */
			case kHIDPage_GenericDesktop:
				switch (usage) {
				case kHIDUsage_GD_X: fallthrough
				case kHIDUsage_GD_Y:fallthrough
				case kHIDUsage_GD_Z:fallthrough
				case kHIDUsage_GD_Rx:fallthrough
				case kHIDUsage_GD_Ry:fallthrough
				case kHIDUsage_GD_Rz:fallthrough
				case kHIDUsage_GD_Slider:fallthrough
				case kHIDUsage_GD_Dial:fallthrough
				case kHIDUsage_GD_Wheel:
					print("G")
					print(cookie, usagePage, usage)
					
					if AddElement(to:&device.axisList, element: element, cookie:cookie, usagePage: usagePage, usage: usage)
					{
						/* add to list */
						device.axes += 1
						device.elements += 1
					}

					break
				case kHIDUsage_GD_Hatswitch:
					print("H")
					print(cookie, usagePage, usage)
					if !device.hatList.contains(where: { $0.cookie == cookie })
					{
						/* add to list */
						device.hats += 1

						// find element with usage >= element.usage
						let index = device.hatList.firstIndex(where: {usage >= $0.usage} ) ?? 0
						// insert element here
						device.hatList.insert(
							recElement(element, usagePage: usagePage, usage: usage),
							at: index)

						device.elements += 1
					}
					break
				case kHIDUsage_GD_DPadUp: fallthrough
				case kHIDUsage_GD_DPadDown: fallthrough
				case kHIDUsage_GD_DPadRight: fallthrough
				case kHIDUsage_GD_DPadLeft: fallthrough
				case kHIDUsage_GD_Start: fallthrough
				case kHIDUsage_GD_Select: fallthrough
				case kHIDUsage_GD_SystemMainMenu:
					print("B")
					print(cookie, usagePage, usage)
					if !device.buttonList.contains(where: { $0.cookie == cookie })
					{
						/* add to list */
						device.buttons += 1

						// find element with usage >= element.usage
						let index = device.buttonList.firstIndex(where: {usage >= $0.usage} ) ?? 0
						// insert element here
						device.buttonList.insert(
							recElement(element, usagePage: usagePage, usage: usage),
							at: index)

						device.elements += 1
					}
					break

				default:
					break
				}
			case kHIDPage_Button: fallthrough
			case kHIDPage_Consumer: /* e.g. 'pause' button on Steelseries MFi gamepads. */
				print("CB", cookie, usagePage, usage)
				if !device.buttonList.contains(where: { $0.cookie == cookie })
				{
					print("adding")
					/* add to list */
					device.buttons += 1

					// find element with usage >= element.usage
					let index = device.buttonList.firstIndex(where: {usage >= $0.usage} ) ?? 0
					// insert element here
					device.buttonList.insert(
						recElement(element, usagePage: usagePage, usage: usage),
						at: index)

					device.elements += 1
				}

			default:
				break
			}
		
		case kIOHIDElementTypeCollection:
			print("T")
			print(cookie, usagePage, usage)
			
			if let array = IOHIDElementGetChildren(element) as? [IOHIDElement]
			{
				print("-->")
				AddHIDElements(array, &device)
			}

			break

		default:
			break
		}
		
	}

}

func AddHIDElements(_ array: [IOHIDElement], _ device : inout recDevice)
{
	_ = array.map { AddHIDElement( $0 , &device) }
}


func GetDeviceInfo(_ hidDevice: IOHIDDevice ) -> recDevice?
{
	var vendor = 0
	var product = 0
//	var version = 0
	var manufacturer_string = ""
	var product_string = ""
	
	var device = recDevice()
	
	device.usagePage = IOHIDDeviceGetProperty(hidDevice, usagePageKey) as! CFNumber as! Int

	if device.usagePage != kHIDPage_GenericDesktop
	{
		return nil /* Filter device list to non-keyboard/mouse stuff */
	}

	device.usage = IOHIDDeviceGetProperty(hidDevice, usageKey) as! CFNumber as! Int

	if (device.usage != kHIDUsage_GD_Joystick &&
		device.usage != kHIDUsage_GD_GamePad &&
		device.usage != kHIDUsage_GD_MultiAxisController) {
		return nil /* Filter device list to non-keyboard/mouse stuff */
	}
	
	device.deviceHID = hidDevice
	
	vendor = IOHIDDeviceGetProperty(hidDevice, vendorKey) as! CFNumber as! Int
	product = IOHIDDeviceGetProperty(hidDevice, productIDKey) as! CFNumber as! Int
//	version = IOHIDDeviceGetProperty(hidDevice, versionKey) as! CFNumber as! Int

	/* get device name */
	manufacturer_string = IOHIDDeviceGetProperty(hidDevice, manufacturerKey) as! String
	product_string = IOHIDDeviceGetProperty(hidDevice, productKey) as! String

	device.product = "\(vendor) \(product) \(manufacturer_string) \(product_string)"

	// if SDL_JoystickHandledByAnotherDriver { return false; }
	// CreateJoystickGUID
	
	let array = IOHIDDeviceCopyMatchingElements(hidDevice, nil, IOOptionBits(kIOHIDOptionsTypeNone)) as! [IOHIDElement]
	AddHIDElements(array, &device)

	return device
}

fileprivate var deviceid = 0

var JoystickDeviceWasAddedCallback : IOHIDDeviceCallback = {
	(context, result, sender, hiddevice) in
	
	var device : recDevice?
	if result != kIOReturnSuccess { return }
	if JoystickAlreadyKnown(hiddevice) { return }
	guard var device = GetDeviceInfo(hiddevice) else {
		return /* not a device we care about, probably. */
	}
	
//	if (SDL_ShouldIgnoreJoystick(device.product, device.guid)) { return	}

	/* Get notified when this device is disconnected. */
	IOHIDDeviceRegisterRemovalCallback(hiddevice, JoystickDeviceWasRemovedCallback, nil)
	
	IOHIDDeviceScheduleWithRunLoop(hiddevice, CFRunLoopGetCurrent(), CFRunLoopMode.defaultMode.rawValue)
	device.runLoopAttached = true

	/* Allocate an instance ID for this device */
	device.instance_id = deviceid
	deviceid+=1 // get next Id
	
	deviceList.append(device)
	
	// seize the device for myself - stop system recieving events
//	IOHIDDeviceOpen(hiddevice, IOOptionBits(kIOHIDOptionsTypeSeizeDevice))
	
	// set value callback
	IOHIDDeviceRegisterInputValueCallback(hiddevice, valueCallback, nil)

	print("attached: \(device.product)")
}

let JOY_BUTTON1 =        0x0001
let JOY_BUTTON2 =        0x0002

var valueCallback : IOHIDValueCallback = {
	(context, result, sender, value) in

	// context will be 'nil'
	// result will be success if we got a value
	// sender will be the device
	// value will contain the element (& cookie) and the value

	// what is the element for the value
	let element = IOHIDValueGetElement(value)
			
	// find the device element (i.e. a button or axis) with this cookie
	let cookie = IOHIDElementGetCookie(element)
			
	// get the integer value for this HIDValue
	var code = IOHIDValueGetIntegerValue(value)

	// unsafeBitCast is not recommended
	let dev = Unmanaged<IOHIDDevice>.fromOpaque(sender!).takeUnretainedValue()
	let foundDev = deviceList.filter({$0.deviceHID == dev}).first

	if result == kIOReturnSuccess && foundDev != nil
	{
		let thisDev = foundDev!
		
		// that's the device
				
		var theButtons = thisDev.buttonList.filter {$0.elementRef == element}
		var theAxes = thisDev.axisList.filter {$0.elementRef == element}
		
		for thisButton in theButtons
		{
			print("device", thisDev.usagePage, thisDev.usage, terminator:" - ")
			print("button", cookie, thisButton.usagePage, thisButton.usage, code)
			if code > 1
			{
				/* handle pressure-sensitive buttons */
				code = 1
			}
			
			var buttonBits = 0 // ought to record the current state of all buttons to send
			// buttons are bit flags
			if code == 1
			{
				buttonBits = thisButton.usage == 1 ? JOY_BUTTON1 : JOY_BUTTON2
				beeb_handlejoystick(kEventMouseDown, buttonBits, 0)
			}
			else
			{
				beeb_handlejoystick(kEventMouseUp, buttonBits, 0)
			}

		}

		for thisAxis in theAxes
		{
			print("device", thisDev.usagePage, thisDev.usage, terminator:" - ")

			switch thisAxis.usage
			{
			case kHIDUsage_GD_X:
				print("x axis", cookie, thisAxis.usagePage, thisAxis.usage, code)
				let lparam = toLParam(x:CGFloat(Double(code)/256.0),y:CGFloat(0))
				beeb_handlejoystick(kEventMouseMoved, 0, lparam)
				break
			case kHIDUsage_GD_Y:
				print("y axis", cookie, thisAxis.usagePage, thisAxis.usage, code)
				let lparam = toLParam(x:CGFloat(0),y:CGFloat(Double(code)/256.0))
				beeb_handlejoystick(kEventMouseMoved, 0, lparam)
				break
			default:
				break
			}


		}
		
	}
	else
	{
		print("not success")
	}
	
	
}

var valueCallback1 : IOHIDValueCallback = {
	(context, result, sender, value) in

	let element = IOHIDValueGetElement(value)
	let cookie = IOHIDElementGetCookie(element)
	let code = IOHIDValueGetIntegerValue(value)

	let JOY_BUTTON1  =  0x0001
	let JOY_BUTTON2  =  0x0002

	print (element)
//	print(cookie,code)


	switch cookie {
	case 6:     /* Button 1 */
//		print("Button 0: ", code)
		beeb_handlejoystick((code != 0) ? kEventMouseDown : kEventMouseUp, JOY_BUTTON1, 0)
	case 7:     /* Button 1 */
//		print("Button 1: ", code)
		beeb_handlejoystick((code != 0) ? kEventMouseDown : kEventMouseUp, JOY_BUTTON2, 0)
	case 15:     /* Horizontal */
		print("X Axis: ", code)
		let lparam = toLParam(x:CGFloat(Double(code)/256.0),y:CGFloat(0))
		beeb_handlejoystick(kEventMouseMoved, 0, lparam)
	case 16:     /* Vertical */
		print("Y Axis: ", code)
		let lparam = toLParam(x:CGFloat(0),y:CGFloat(Double(code)/256.0))
		beeb_handlejoystick(kEventMouseMoved, 0, lparam)
	default:
		print("Unknown element")
		// kHIDPage_GenericDesktop = 0x01
		// kHIDUsage_GD_Joystick = 0x04
		// kHIDUsage_GD_GamePad = 0x05
		// kHIDUsage_GD_MultiAxisController = 0x08
		// kHIDUsage_GD_X = 0x30
		// kHIDUsage_GD_Y = 0x31
		// kHIDUsage_GD_Hatswitch = 0x39

//		print (element)
		print(cookie,code)

	}

}



var JoystickDeviceWasRemovedCallback : IOHIDCallback = {
	(context, result, sender) in

	let dev = Unmanaged<IOHIDDevice>.fromOpaque(sender!).takeUnretainedValue()
	
	let device = deviceList.filter( {$0.deviceHID == dev}).first
	print("detached ", device?.product ?? "unknown")

	deviceList.removeAll {$0.deviceHID == dev}
}

func GetHIDScaledCalibratedState(_ device : recDevice, _ element : inout recElement, _ min: Int, _ max: Int, _ value: inout Int) -> Bool
{
	let deviceScale = max - min
	let readScale = element.maxReport - element.minReport

	if (GetHIDElementState(device, &element, &value)) {
		if readScale == 0
		{
			return true /* no scaling at all */
		}
		else
		{
			value = (((value - element.minReport) * deviceScale / readScale) + min)
			return true
		}
	}

	return false
}
func GetHIDElementState(_ device : recDevice, _ element : inout recElement, _ value: inout Int) -> Bool
{
	if element.cookie == 6
	{
		print("button ", element.usagePage, element.usage)
	}
	let valueHID = IOHIDValueCreateWithIntegerValue(nil, element.elementRef!, 0, 0)
	var valueRef = Unmanaged<IOHIDValue>.passUnretained(valueHID)
	if IOHIDDeviceGetValue(device.deviceHID!, element.elementRef!, &valueRef) == kIOReturnSuccess
	{
		let value = IOHIDValueGetIntegerValue(valueRef.takeUnretainedValue()) as Int
		/* record min and max for auto calibration */
		if (value < element.minReport) {
			element.minReport = value;
		}
		if (value > element.maxReport) {
			element.maxReport = value;
		}

		return true
	}
	
	return false
}

func JoystickUpdate0()
{
//	guard let dev0 = deviceList.first else {return }
//	for dev in deviceList
//	{
//		return JoystickUpdate(dev)
//	}
}

func JoystickUpdate(_ device: recDevice) // for joytick ID/ref
{
	let timestamp = 123 //SDL_GetTicksNS();

	if device.removed
	{
		/* device was unplugged; ignore it. */
		return
	}
	
	for (_, var element) in device.axisList.enumerated()
	{
		var value = 0
		let goodRead = GetHIDScaledCalibratedState(device, &element, -32768, 32767, &value);
		if goodRead
		{
//			print("axis", timestamp, i, value)
//			SDL_SendJoystickAxis(timestamp, joystick, i, value);
		}
	}
	
	for (i, var element) in device.buttonList.enumerated()
	{
		var value = 0
		let goodRead = GetHIDElementState(device, &element, &value)
		if goodRead
		{
			if value > 1
			{
				/* handle pressure-sensitive buttons */
				value = 1
			}
			if i == 0 {
				print("button", timestamp, i, value)
			}
//			SDL_SendJoystickButton(timestamp, joystick, i, value);
		}

	}
	
	for (i,var element) in device.hatList.enumerated()
	{
		var pos = 0
		var value = 0
		let range = element.max - element.min + 1
		let goodRead = GetHIDElementState(device, &element, &value)
		if goodRead
		{
			value -= element.min;
			if range == 4
			{
				/* 4 position hatswitch - scale up value */
				value *= 2;
			}
			else if range != 8
			{
				/* Neither a 4 nor 8 positions - fall back to default position (centered) */
				value = -1;
			}
			let HAT_CENTERED  =  0x00
			let HAT_UP  =      0x01
			let HAT_RIGHT =    0x02
			let HAT_DOWN   =   0x04
			let HAT_LEFT    =  0x08
			let HAT_RIGHTUP =    (HAT_RIGHT|HAT_UP)
			let HAT_RIGHTDOWN =  (HAT_RIGHT|HAT_DOWN)
			let HAT_LEFTUP    =  (HAT_LEFT|HAT_UP)
			let HAT_LEFTDOWN  =  (HAT_LEFT|HAT_DOWN)

			switch value
			{
			case 0:
				pos = HAT_UP;
				break;
			case 1:
				pos = HAT_RIGHTUP;
				break;
			case 2:
				pos = HAT_RIGHT;
				break;
			case 3:
				pos = HAT_RIGHTDOWN;
				break;
			case 4:
				pos = HAT_DOWN;
				break;
			case 5:
				pos = HAT_LEFTDOWN;
				break;
			case 6:
				pos = HAT_LEFT;
				break;
			case 7:
				pos = HAT_LEFTUP;
				break;
			default:
				/* Every other value is mapped to center. We do that because some
				 * joysticks use 8 and some 15 for this value, and apparently
				 * there are even more variants out there - so we try to be generous.
				 */
				pos = HAT_CENTERED;
				break;
			}

			print("hat",timestamp,i,pos)
//			SDL_SendJoystickHat(timestamp, joystick, i, pos);

		}
	}
}

/*
********
CreateHIDManager
 vals CreateHIDDeviceMatchDictionary x 3
 ---
   dictionary [kIOHIDDeviceUsagePageKey:page number , kIOHIDDeviceUsageKey: usage number]
 array kCFTypeArrayCallBacks x 3  >>> this is an immutable array
 vals release x 3
 
 IOHIDManagerCreate
 array ConfigHIDManager
 ---
 IOHIDManagerOpen
 IOHIDManagerSetDeviceMatchingMultiple
 IOHIDManagerRegisterDeviceMatchingCallback  JoystickDeviceWasAddedCallback
 IOHIDManagerScheduleWithRunLoop
 
 while CFRunLoopRunInMode  kCFRunLoopRunHandledSource
 
 
 
 //==
 JoystickDeviceWasAddedCallback
 JoystickAlreadyKnown ioHIDDeviceObject
 GetDeviceInfo   device->product, device->guid
   AddHIDElements(array, pDevice);
   AddHIDElement IOHIDElementRef recDevice
    
 IOHIDDeviceRegisterRemovalCallback  JoystickDeviceWasRemovedCallback device
 IOHIDDeviceScheduleWithRunLoop
 device->runLoopAttached
 device->instance_id
 ioservice = IOHIDDeviceGetService
 add to list
 
 
 
 
 DARWIN_JoystickUpdate()
 GetHIDScaledCalibratedState
 
 GetHIDElementState
 IOHIDDeviceGetValue
 IOHIDValueGetIntegerValue
********
 */

