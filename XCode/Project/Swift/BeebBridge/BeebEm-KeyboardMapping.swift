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

import os

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

// vendor and produce numbers for PS3 controller
let PS3vendorIdNum = 0x054C as Int
let PS3productIdNum = 0x0268 as Int

let vendorIDKey = kIOHIDVendorIDKey as CFString
let productIDKey = kIOHIDProductIDKey as CFString


let deviceCriteria:NSArray = [
	[
		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
		kIOHIDDeviceUsageKey: kHIDUsage_GD_Joystick
	],
//	[
//		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
//		kIOHIDDeviceUsageKey: kHIDUsage_GD_GamePad
//	]
//	,
//	[
//		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
//		kIOHIDDeviceUsageKey: kHIDUsage_GD_Hatswitch
//	]
//	,
//	[
//		kIOHIDDeviceUsagePageKey: kHIDPage_GenericDesktop,
//		kIOHIDDeviceUsageKey: kHIDUsage_GD_MultiAxisController
//	]
//	,
//	[
//		kIOHIDVendorIDKey: PS3vendorIdNum,
//		kIOHIDProductIDKey: PS3productIdNum,
//	]
//	,
]

let versionKey = kIOHIDVersionNumberKey as CFString
let productKey = kIOHIDProductKey as CFString
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
	let logger = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "HID")
	
	let elementTypeID = CFGetTypeID(element)
	if (elementTypeID == IOHIDElementGetTypeID())
	{
		let cookie = IOHIDElementGetCookie(element)
		let usagePage = Int(IOHIDElementGetUsagePage(element))
		let usage = Int(IOHIDElementGetUsage(element))
		
		// https://opensource.apple.com/source/IOHIDFamily/IOHIDFamily-421.6/IOHIDFamily/IOHIDUsageTables.h.auto.html
		let etype = IOHIDElementGetType(element)

		/* look at types of interest */
		switch (etype)
		{
		case kIOHIDElementTypeInput_Misc:
			fallthrough
		case kIOHIDElementTypeInput_Button:
			fallthrough
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
					logger.debug("Generic Desktop axis")
					if AddElement(to:&device.axisList, element: element, cookie:cookie, usagePage: usagePage, usage: usage)
					{
						/* add to list */
						device.axes += 1
						device.elements += 1
					}

					break
				case kHIDUsage_GD_Hatswitch:
					logger.debug("Generic Desktop hat")
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
					logger.debug("Generic Desktop buttons")
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
				logger.debug("Buttons")
				if !device.buttonList.contains(where: { $0.cookie == cookie })
				{
//					print("adding")
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
			logger.debug("Collection")

			if let array = IOHIDElementGetChildren(element) as? [IOHIDElement]
			{
				logger.debug("-->")
				AddHIDElements(array, &device)
			}

			break
		case kIOHIDElementTypeOutput:
			logger.debug("Output")
			break
		case kIOHIDElementTypeFeature:
			logger.debug("Feature")
			break
		default:
			logger.debug("?")
			break
		}
		let msg = String(format: "usagepage %02x, usage %02x, etype %02x, cookie %04x, [\(device.product)]", usagePage, usage, etype.rawValue, cookie)
		logger.debug("\(msg)")
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
	
	vendor = IOHIDDeviceGetProperty(hidDevice, vendorIDKey) as! CFNumber as! Int
	product = IOHIDDeviceGetProperty(hidDevice, productIDKey) as! CFNumber as! Int
//	version = IOHIDDeviceGetProperty(hidDevice, versionKey) as! CFNumber as! Int

	/* get device name */
	manufacturer_string = IOHIDDeviceGetProperty(hidDevice, manufacturerKey) as! String
	product_string = IOHIDDeviceGetProperty(hidDevice, productKey) as! String

	device.product = "\(String(vendor, radix:16)) \(String(product, radix:16)) \(manufacturer_string) \(product_string)"

	// if SDL_JoystickHandledByAnotherDriver { return false; }
	// CreateJoystickGUID
	
	let array = IOHIDDeviceCopyMatchingElements(hidDevice, nil, IOOptionBits(kIOHIDOptionsTypeNone)) as! [IOHIDElement]
	AddHIDElements(array, &device)
	
	if device.product.contains("Sony PLAYSTATION(R)3 Controller")
	{
		activatePS3(&device)
	}

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
	
	// set value callback
//	IOHIDDeviceRegisterInputValueCallback(hiddevice, valueCallback, nil)
	
	print("HIDDIVICE", hiddevice)
	print("attached: \(device.product)")
}

let JOY_BUTTON1 =        0x0001
let JOY_BUTTON2 =        0x0002

var valueCallback : IOHIDValueCallback = {
	(context, result, sender, value) in

	let logger = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "joystick values")
	
	
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
			logger.debug("device \(thisDev.usagePage) \(thisDev.usage) - ")
			logger.debug("button \(cookie) \(thisButton.usagePage) \(thisButton.usage) \(code)")
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
			logger.debug("device \(thisDev.usagePage) \(thisDev.usage) - ")

			switch thisAxis.usage
			{
			case kHIDUsage_GD_X:
				logger.debug("left X axis \(cookie) \(thisAxis.usagePage) \(thisAxis.usage) \(code)")
				let lparam = toLParam(x:CGFloat(Double(code)/256.0),y:CGFloat(127/256.0))
				beeb_handlejoystick(kEventMouseMoved, 0, lparam)
				break
			case kHIDUsage_GD_Y:
				logger.debug("left Y axis \(cookie) \(thisAxis.usagePage) \(thisAxis.usage) \(code)")
				let lparam = toLParam(x:CGFloat(127/256.0),y:CGFloat(Double(code)/256.0))
				beeb_handlejoystick(kEventMouseMoved, 0, lparam)
				break
			case kHIDUsage_GD_Z:
				logger.debug("left Z axis \(cookie) \(thisAxis.usagePage) \(thisAxis.usage) \(code)")
				break
			case kHIDUsage_GD_Rx: fallthrough
			case kHIDUsage_GD_Ry: fallthrough
			case kHIDUsage_GD_Rz:
//				logger.debug("right xyz axis", cookie, thisAxis.usagePage, thisAxis.usage, code)
				break
			default:
//				logger.debug("other", cookie, thisAxis.usagePage, thisAxis.usage, code)
				break
			}


		}
		
	}
	else
	{
		logger.debug("not success")
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
	let valueHID = IOHIDValueCreateWithIntegerValue(nil, element.elementRef!, 0, 0)
	var valueRef = Unmanaged<IOHIDValue>.passUnretained(valueHID)
	if IOHIDDeviceGetValue(device.deviceHID!, element.elementRef!, &valueRef) == kIOReturnSuccess
	{
		value = IOHIDValueGetIntegerValue(valueRef.takeUnretainedValue()) as Int
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
	for dev in deviceList
	{
		return JoystickUpdate(dev)
	}
}

let axisMapping = [
	kHIDUsage_GD_X : 0,
	kHIDUsage_GD_Y : 1,
	kHIDUsage_GD_Rx : 2,
	kHIDUsage_GD_Ry : 3,
]

let buttonMapping = [
	1 : 1,
	2 : 2,
	3 : 3,
	4 : 4,
]

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
			beeb_handlejoystick(aEventJoystick1Axis, axisMapping[element.usage] ?? 0 , value)
		}
	}

	for (_, var element) in device.buttonList.enumerated()
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
			
			// element usage is the button number
			beeb_handlejoystick(aEventJoystick1Button, buttonMapping[element.usage] ?? 0,  value)
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


/*
 fix for PS3 controller not working
 https://github.com/libsdl-org/SDL/issues/4923
  
 */
let TESTRUMBLE = false

func activatePS3(_ device: inout recDevice)
{
	// simple activation
	let controlBlob : [UInt8] = [ 0x42, 0x0C, 0x00, 0x00 ]
	
	controlBlob.withUnsafeBytes { (unsafeBytes) in
		let bytes = unsafeBytes.bindMemory(to: UInt8.self).baseAddress!
		let _ = IOHIDDeviceSetReport(device.deviceHID!, kIOHIDReportTypeFeature, 0xF4, bytes, unsafeBytes.count)
	}
	
	print ("Activating PS3 device ... ")

	if TESTRUMBLE
	{
	// And the following sequence can be used to activate rumble support and additionally disable LEDs
	var rumbleBlob  : [UInt8] = [
		0x01,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00, // rumble values [0x00, right-timeout, right-force, left-timeout, left-force]
		   0x00,
		   0x00, // Gyro
		   0x00,
		   0x00,
		   0x00, // 0x02=LED1 .. 0x10=LED4
		   /*
			* the total time the led is active (0xff means forever)
			* |     duty_length: how long a cycle is in deciseconds:
			* |     |                              (0 means "blink very fast")
			* |     |     ??? (Maybe a phase shift or duty_length multiplier?)
			* |     |     |     % of duty_length led is off (0xff means 100%)
			* |     |     |     |     % of duty_length led is on (0xff is 100%)
			* |     |     |     |     |
			* 0xff, 0x27, 0x10, 0x00, 0x32,
			*/
		   0xff,
		   0x27,
		   0x10,
		   0x00,
		   0x32, // LED 4
		   0xff,
		   0x27,
		   0x10,
		   0x00,
		   0x32, // LED 3
		   0xff,
		   0x27,
		   0x10,
		   0x00,
		   0x32, // LED 2
		   0xff,
		   0x27,
		   0x10,
		   0x00,
		   0x32, // LED 1
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   // Necessary for Fake DS3
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		   0x00,
		]
	
	let RumbleLengthL = 4;
	let RumblePowerL = 5;
	let RumbleLengthR = 2;
	let RumblePowerR = 3;
	rumbleBlob[RumbleLengthL] = 80
	rumbleBlob[RumbleLengthR] = 80
	rumbleBlob[RumblePowerL] = 255
	rumbleBlob[RumblePowerR]   = 1
	
	rumbleBlob.withUnsafeBytes { (unsafeBytes) in
		let bytes = unsafeBytes.bindMemory(to: UInt8.self).baseAddress!
		let r = IOHIDDeviceSetReport(device.deviceHID!, kIOHIDReportTypeFeature, 1, bytes, unsafeBytes.count)
		print(String(format: "0x%x",bytes))
		print(String(format: "0x%x",r), r==kIOReturnSuccess)
	}
	
	print("  Should be rumbling!\n");
	}
}
