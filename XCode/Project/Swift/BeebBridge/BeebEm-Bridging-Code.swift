//
//  BeebEm5-Bridging-Code.swift
//  BeebEm5
//
//  Created by Commander Coder on 06/07/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

import Foundation
import Cocoa
import AVFoundation

@objc enum FileFilter :Int {
    case DISC
    case UEFFILE
    case IFD
    case KEYBOARD
    case DISCFILE
	case HARDDRIVE
    case PRINTFILE
    case ROMCFG
}


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


//https://developer.apple.com/documentation/uniformtypeidentifiers/uttype
//https://useyourloaf.com/blog/swiftui-importing-and-exporting-files/
extension UTType {
	public static let ssd = UTType(importedAs: "com.commandercoder.BeebEm.ssd")
	public static let hdd = UTType(importedAs: "com.commandercoder.BeebEm.dat")
	public static let tape = UTType(importedAs: "com.commandercoder.BeebEm.uef")
	public static let rom = UTType(importedAs: "com.commandercoder.BeebEm.rom")

	public static let keymap = UTType(exportedAs: "com.commandercoder.BeebEm.kmap")
}


// allow access to this in C
@_cdecl("swift_GetFilesWithPreview")
func swift_GetFilesWithPreview(filepath : UnsafeMutablePointer<CChar>, bytes: Int, directory : UnsafeMutablePointer<CChar>, fileexts : FileFilter, multiFiles : Bool = false) -> Int
{
    let dialog = NSOpenPanel()
    
    dialog.title                   = "Choose a file | BeebEm5"
    dialog.showsResizeIndicator    = true
    dialog.showsHiddenFiles        = false
    dialog.allowsMultipleSelection = multiFiles
    dialog.canChooseDirectories    = false
    dialog.canChooseFiles    = true
    dialog.allowsOtherFileTypes = true


    // FUTURE
//    let launcherLogPath = NSString("~/Documents").expandingTildeInPath  // need to expand the path if it includes ~
    let launcherLogPath = String( cString: directory)
    dialog.directoryURL = NSURL.fileURL(withPath: launcherLogPath, isDirectory: true)
    dialog.canCreateDirectories = true
    
    switch fileexts {
    case .DISC:
		dialog.allowedContentTypes        = [.ssd] // ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"]
		break
	case .UEFFILE:
        dialog.allowedContentTypes        = [.tape] // ["uef", "csw"]
		break
    case .IFD:
        dialog.allowedContentTypes        = [] // ["ssd", "dsd", "inf"]
		break
    case .KEYBOARD:
        dialog.allowedContentTypes        = [.keymap] // ["kmap"]
		break
	case .HARDDRIVE:
		dialog.allowedContentTypes        = [.hdd] //nil  // ["inf"]
		break
    case .DISCFILE:
		dialog.allowedContentTypes        = [.data] //nil  // ["inf"]
		break
    case .ROMCFG:
        dialog.allowedContentTypes        = [.rom] // ["rom"]
		break
    case .PRINTFILE:
        break
    }
    
    if (dialog.runModal() ==  NSApplication.ModalResponse.OK) {
        let result = dialog.urls // Pathnames of the files

        if (result.count != 0) {
            
            var path: String = (dialog.directoryURL?.path ?? "") + "\0"
            if result.count == 1
            {
                path = (dialog.url?.path ?? "") + "\0"
            }
            else
            {
                for f in result
                {
                    path = path + f.lastPathComponent + "\0"
                }
            }
            
            // path contains the file path e.g
            // /Users/ourcodeworld/Desktop/file.txt
            
            // set the filepath back in the C code - fill with zeros first
            filepath.assign(repeating: 0, count: bytes)
            filepath.assign(from: path, count: path.count)

            return 0 // no issues
        }
    }
//      User clicked on "Cancel"
// or   the result URL was nil
    return 1 // error
}

@_cdecl("swift_SelectFolder")
func swift_SelectFolder(filepath : UnsafeMutablePointer<CChar>, bytes: Int) -> Int
{
    let dialog = NSOpenPanel();
    dialog.title                   = "Choose a folder | BeebEm5"
    dialog.showsResizeIndicator    = true
    dialog.canChooseDirectories    = true
    dialog.canChooseFiles          = false
    
    if (dialog.runModal() ==  NSApplication.ModalResponse.OK) {
        guard let result = dialog.url else { return 0} // Pathname of the file
        let path: String = result.path
        
        // path contains the folder e.g
        // /Users/ourcodeworld/Desktop/
        
        // set the filepath back in the C code.. fill with zeros first
        filepath.assign(repeating: 0, count: bytes)
        filepath.assign(from: path, count: path.count)
            
        print("Picked \(String(cString:filepath))")
        return 0
    }
    return 1 //err
}



@_cdecl("swift_SaveFile")
func swift_SaveFile(filepath : UnsafeMutablePointer<CChar>, bytes: Int, fileexts: FileFilter) -> Bool
{
	// test that PRINTDIALOG with no filename works
	let dialog = NSSavePanel()
    
    dialog.title                   = "Choose a file | BeebEm5"
    dialog.showsResizeIndicator    = true
    dialog.showsHiddenFiles        = false
    dialog.allowsOtherFileTypes    = true
	dialog.isExtensionHidden	= false
	
    switch fileexts {
    case .DISC:
        dialog.allowedContentTypes        = [.ssd] // ["ssd", "dsd", "wdd", "dos", "adl", "adf", "img"]
		break
    case .UEFFILE:
        dialog.allowedContentTypes        = [.tape] // ["uef", "csw"]
		break
    case .IFD:
        dialog.allowedContentTypes        = [] // ["ssd", "dsd", "inf"]
		break
    case .KEYBOARD:
        dialog.allowedContentTypes        = [.keymap] // ["kmap"]
		break
	case .HARDDRIVE:
		dialog.allowedContentTypes        = [.data] //nil  // ["inf"]
		break
    case .DISCFILE:
		dialog.allowedContentTypes        = [.data] //nil // ["inf"]
		break
    case .ROMCFG:
        dialog.allowedContentTypes        = [.rom] // ["rom"]
		break
    case .PRINTFILE:
		dialog.allowedContentTypes        = [.plainText] //nil  // ["inf"]
		break
    }

    if (dialog.runModal() ==  NSApplication.ModalResponse.OK) {
        guard let result = dialog.url else { return false} // Pathname of the file

        let path: String = result.path
        
        // path contains the file path e.g
        // /Users/ourcodeworld/Desktop/file.txt
        
        // set the filepath back in the C code.. fill with zeros first
        filepath.assign(repeating: 0, count: bytes)
        filepath.assign(from: path, count: path.count)
            
        print("Picked \(String(cString:filepath))")
        return true
    
    }
    return false
}

// allow access to this in C
@_cdecl("swift_MoveFile")
public func swift_MoveFile(srcpath : UnsafeMutablePointer<CChar>, destpath : UnsafeMutablePointer<CChar>) -> Bool
{
    let src = String(cString: srcpath)
    let dest = String(cString: destpath)
    do
    {
        try FileManager.default.moveItem(atPath: src, toPath: dest)
    }
    catch  {
        print("Unexpected error: \(error).")
        return false
    }
    return true
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

// convert a string of 4 characters to a UInt32
func conv(_ str: String) -> UInt32
{
    var  v: UInt32 = 0
    let  i = str.count-1
    for (index, element) in str.unicodeScalars.enumerated()
    {
        if index == 4
        {
            break
        }
        v += element.value << ((i-index)*8)
    }
    return v
}

// convert a UInt32 to a string of 4 characters
func conv(_ value: UInt32) -> String
{
    var  s: String = ""
    let  i = 4
    for index in 1...i
    {
        // if unicode not recognised - use '@'
        s.append( Character(UnicodeScalar(value >> ((i-index)*8)&0xff) ?? UnicodeScalar(64)))
    }
    return s
}

extension NSView {

    func subviewsRecursive() -> [NSView] {
        return subviews + subviews.flatMap { $0.subviewsRecursive() }
    }
}



func recurse(find id: String, menu: NSMenu) -> NSMenuItem? {
  for item in menu.items {
      if item.identifier?.rawValue == id //&& item.isEnabled
      {
        return item
      } else if let submenu = item.submenu {
        if let item = recurse(find: id, menu: submenu) {
            return item
       }
    }
  }
  return nil
}

func textFieldByIdentifier(id: String) -> NSTextField? {
    return nil
}


func menuItemByIdentifier(id: String) -> NSMenuItem? {
    guard let mainmenu = NSApplication.shared.mainMenu else { return nil }

    return recurse(find: id, menu: mainmenu)
}

// set the tick on the menu with a 4 character identifier
@_cdecl("swift_SetMenuCheck")
public func swift_SetMenuCheck(_ cmd: UInt32, _ check: Bool)
{
    let cmdSTR =  conv(cmd)

    if let n = menuItemByIdentifier(id:cmdSTR)
    {
//        print("\(#function)",cmdSTR,check)
        n.state = check ? .on : .off
    }
    else
    {
		print("\(#function): \(cmd) not found: \(cmdSTR)")
    }
}


// set the text on the menu with a 4 character identifier
@_cdecl("swift_ModifyMenu")
public func swift_ModifyMenu(_ cmd: UInt32, _ newitem: UInt32,  _ itemtext: UnsafePointer<CChar>)
{
    assert(cmd == newitem)

    let cmdSTR =  conv(cmd)
    var text = String(cString: itemtext)
    if let n = menuItemByIdentifier(id:cmdSTR)
    {
        if (text.first == "&" )
        {
            text.removeFirst()
            n.keyEquivalent = String(text.first!)
        }
        n.title = text
    }
}

// grey the menu with a 4 character identifier
@_cdecl("swift_SetMenuEnable")
public func swift_SetMenuEnable(_ cmd: UInt32, _ enable: Bool)
{
    // There is a checkbox in the Menu's Inspector called "Auto Enables Items" that was overriding my code.
    let cmdSTR =  conv(cmd)
    if let n = menuItemByIdentifier(id:cmdSTR)
    {
//        print("\(#function)",cmdSTR,enable)
        n.isEnabled = enable
    }
    else
    {
//        print("\(#function) not found: ",cmdSTR)
    }
}


@_cdecl("swift_SetMenuItemTextWithCString")
public func swift_SetMenuItemTextWithCString(_ cmd: UInt32, _ text: UnsafePointer<CChar>) -> Int
{
    let cmdSTR =  conv(cmd)
    if let n = menuItemByIdentifier(id:cmdSTR)
    {
        n.title = String(cString: text)
        return 0
    }
    return 1
}

// Rom Config

//@_cdecl("swift_RCSelectItem")
//public func swift_TCSelectItem(_ text: UnsafePointer<CChar>, _ b:Int, _ c: UnsafePointer<UInt>)
//{
////    print("\(#function) \(String(cString:text)) \(b) ")
//    tcViewControllerInstance?.selectRowInTable(UInt(b))
//}
//
//@_cdecl("swift_RCSelectItem")
//@_cdecl("swift_RCSelectItem")




@_cdecl("swift_Alert")
public func swift_Alert(_ text: UnsafePointer<CChar>, _ text2: UnsafePointer<CChar>,_ hasCancel:Bool) -> Int
{
    
    let a = NSAlert()
    a.messageText = String(cString: text)
   a.informativeText = String(cString: text2)
   //   .alertFirstButtonReturn
   a.addButton(withTitle: "OK")


   //   .alertSecondButtonReturn
    if hasCancel
    {
        a.addButton(withTitle: "Cancel")
    }
    
    a.alertStyle = .warning
    let res = a.runModal()
    let val = res == .alertFirstButtonReturn ? 1:2
    print(String(cString:text)+" "+String(cString:text2)+" "+String(hasCancel))
	// should reload file list BUT not in this alert.. do it before alert
//	swift_TCReloadFileList()
	return val
}


@_cdecl("swift_menuSetControlValue")
public func swift_menuSetControlValue(_ cmd: UInt32, _ state: Int)
{
    let cmdSTR = conv(cmd)
    if let n = menuItemByIdentifier(id:cmdSTR)
    {
        print("scv",cmdSTR)
        n.state = (state==1) ? .on : .off
    }
}

@_cdecl("swift_menuGetControlValue")
public func swift_menuGetControlValue(_ cmd: UInt32) -> Int
{
    var val: Int = 0
    let cmdSTR =  conv(cmd)
    if let n = menuItemByIdentifier(id:cmdSTR)
    {
        print("gcv",cmdSTR)
        val = (n.state == .on) ? 1 : 0
    }
    return val
}

@_cdecl("swift_menuSetControlEditText")
public func swift_menuSetControlEditText(_ cmd: UInt32, _ text: NSString)
{
    let cmdSTR =  conv(cmd)
    if let n = textFieldByIdentifier(id:cmdSTR)
    {
        n.stringValue = text as String
    }
}

@_cdecl("swift_menuGetControlEditText")
public func swift_menuGetControlEditText(_ cmd: UInt32, _ text: NSMutableString, _ length:Int)
{
    let cmdSTR =  conv(cmd)
    if let n = textFieldByIdentifier(id:cmdSTR)
    {
        // set the text back in the C code - fill with zeros first
//        text.assign(repeating: 0, count: length)
//        text.assign(from: n.stringValue, count: length)
        text.setString(n.stringValue)
    }
    
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

@_cdecl("swift_saveScreen")
public func swift_saveScreen(_ text: UnsafePointer<CChar>)
{
    print("\(#function) \(text)")
    beebViewControllerInstance?.screenFilename = String(cString: text);
}

// allow access to this in C
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


// allow access to this in C
@_cdecl("swift_GetBundleDirectory")
public func swift_GetBundleDirectory( _ bundlePath: UnsafeMutablePointer<CChar>, _ length:Int)
{
    let dpath : String = Bundle.main.resourcePath!+"/"
    print("\(#function) \(dpath)")
    // set the filepath back in the C code - fill with zeros first
    bundlePath.assign(repeating: 0, count: length)
    bundlePath.assign(from: dpath, count: dpath.count)
}

// allow access to this in C
@_cdecl("swift_GetApplicationSupportDirectory")
public func swift_GetApplicationSupportDirectory( _ resourcePath: UnsafeMutablePointer<CChar>, _ length:Int)
{
    
    /* discover Application Support and Resources folder
     Opening files will always start at Documents or folder
     https://developer.apple.com/library/archive/documentation/FileManagement/Conceptual/FileSystemProgrammingGuide/MacOSXDirectories/MacOSXDirectories.html#//apple_ref/doc/uid/TP40010672-CH10-SW1
     
     https://developer.apple.com/library/archive/documentation/FileManagement/Conceptual/FileSystemProgrammingGuide/AccessingFilesandDirectories/AccessingFilesandDirectories.html#//apple_ref/doc/uid/TP40010672-CH3-SW7
     */
    
    
//        let patharray = FileManager.default.urls(for: .applicationSupportDirectory,
//                                                 in: .userDomainMask)
//        let userDataDirectory = patharray[0].appendingPathComponent("UserData")
//
//        print (userDataDirectory.path)
//
//        let teletextpath = Bundle.main.url(forResource: "teletext", withExtension: "fnt")!
//        let z80path = Bundle.main.url(forResource: "Z80", withExtension: "ROM",subdirectory: "UserData/BeebFile")!
//        let rompath = Bundle.main.url(forResource: "Roms", withExtension: "cfg", subdirectory: "UserData")!
//        let allpath = Bundle.main.urls( forResourcesWithExtension: nil, subdirectory: "UserData")!
//
//        print (teletextpath)
//        print (z80path)
//        print (rompath)
//        print (allpath)

//        if let fileContents = try? String(contentsOf: rompath) {
//            // we loaded the file into a string!
//            print(fileContents)
//        }
//
//        if let fileContents = try? Data(contentsOf: z80path) {
//            // we loaded the file into a binary datablock!
//            print(fileContents.description)
//            print(fileContents.base64EncodedString(options: .lineLength64Characters))
//            print(String(decoding: fileContents, as: UTF8.self))
//        }
    
    
    
    
    do
    {
        //  Find Application Support directory
        let fileManager = FileManager.default
        let appSupportURL = fileManager.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!
        //  Create subdirectory
        let directoryURL = appSupportURL.appendingPathComponent("BeebEm-mac")
        try fileManager.createDirectory (at: directoryURL, withIntermediateDirectories: true, attributes: nil)
        
        let dpath = directoryURL.path+"/"
        // set the filepath back in the C code - fill with zeros first
        resourcePath.assign(repeating: 0, count: length)
        resourcePath.assign(from: dpath, count: dpath.count)

    }
    catch
    {
        print("Couldn't find the ApplicationSupportDirectory")
    }
}

@_cdecl("swift_Remove")
public func swift_Remove( _ path: UnsafePointer<CChar>) -> Int
{
	let pathStr = String(cString: path)
	do
	{
		let folderURL = NSURL.fileURL(withPath: pathStr, isDirectory: true)
		try FileManager.default.removeItem(at: folderURL)
	}
	catch  {
		print("Unexpected error: \(error).")
		return -1
	}
	return 0
}


// allow access to this in C
@_cdecl("swift_CopyDirectoryRecursively")
public func swift_CopyDirectoryRecursively( _ sourcePath: UnsafePointer<CChar>, _ targetPath:UnsafePointer<CChar>) -> Bool
{
    let sourcePathStr = String(cString: sourcePath)
    let targetPathStr = String(cString: targetPath)

    print( "copy \(sourcePathStr) to \(targetPathStr)");
    
    do {
        if FileManager.default.fileExists(atPath: targetPathStr) {
            try FileManager.default.removeItem(atPath: targetPathStr)
        }
        try FileManager.default.copyItem(atPath: sourcePathStr, toPath:targetPathStr)
    } catch (let error) {
        print("Cannot copy item at \(sourcePathStr) to \(targetPathStr): \(error)")
        return false
    }
    return true
            
}


let SoundRate = 44100
let SoundRate5000 = 46875

//44100 * 20/1000 = 882  = 44100 / 50fps
let SoundLength = SoundRate * 20/1000 // incoming sound length ;Int (@50 fps), rate / 50
let SoundLength5000 = SoundRate5000 * 20/1000 // incoming sound length ;Int (@50 fps), rate / 50

@_cdecl("swift_GetSoundBufferLength")
public func swift_GetSoundBufferLength( _ outputType : UInt8) -> Int
{
    return outputType == 2 ? SoundLength5000 : SoundLength
}

// the audio engine - MUST NOT DEALLOCATE hence OUTSIDE FUNC
var audioEngine = AVAudioEngine()
// the player used for scheduling from bufffer
var audioBufferPlayerNode = AVAudioPlayerNode()

var audioBufferPlayerNode5000 = AVAudioPlayerNode()

//var beebAudioFormat = AVAudioFormat(commonFormat: AVAudioCommonFormat.pcmFormatInt32, sampleRate: SoundRate, channels: 2, interleaved: false)!
var beebAudioFormat = AVAudioFormat(commonFormat: AVAudioCommonFormat.pcmFormatFloat32,
                                    sampleRate: Double(SoundRate),
                                    channels: 2,
                                    interleaved: false)!

var beebAudioFormat5000 = AVAudioFormat(commonFormat: AVAudioCommonFormat.pcmFormatFloat32,
                                    sampleRate: Double(SoundRate5000),
                                    channels: 2,
                                    interleaved: false)!


//create the buffer with the correct format - give it enough room to hold onto as much data as needed
//var buffer = AVAudioPCMBuffer(pcmFormat: beebAudioFormat, frameCapacity: AVAudioFrameCount(SoundLength*6))!
//var buffer = AVAudioPCMBuffer(pcmFormat: audioBufferPlayer.outputFormat(forBus: 0), frameCapacity: 2000)!

//var buffer5000 = AVAudioPCMBuffer(pcmFormat: beebAudioFormat5000, frameCapacity: AVAudioFrameCount(SoundLength5000*6))!


// mixer - to configure the output
// mixer has multiple inputs but one output that has 2 channels - for mainmixer this goes to the audio output
var mainMixer = audioEngine.mainMixerNode
var successfullystarted = false

let kbufs = 36
// Create a pool of audio buffers.
//let audioBuffers = Array(repeating:AVAudioPCMBuffer(pcmFormat: beebAudioFormat, frameCapacity: UInt32(SoundLength)),count:2)
let audioBuffers = (0 ..< kbufs).map { _ in AVAudioPCMBuffer(pcmFormat: beebAudioFormat, frameCapacity: UInt32(SoundLength)) }
let audioBuffers5000 = (0 ..< kbufs).map { _ in AVAudioPCMBuffer(pcmFormat: beebAudioFormat5000, frameCapacity: UInt32(SoundLength5000)) }

// The index of the next buffer to fill.
var audiobufferIndex: Int = 0
var audiobufferIndex5000: Int = 0

// allow access to this in C
@_cdecl("swift_SoundInit")
public func swift_SoundInit()
{
    // audio input is from mic
    // audio player is from buffer/segments
    // audio output is to loudspeaker
//    if !successfullystarted    { return }

    if successfullystarted {
        swift_CloseAudio()
    }

    do {
        // probably shoudn't use BUS 0 by default
//        print (audioBufferPlayer.outputFormat(forBus: 0))
//        print (mainMixer.outputFormat(forBus: 0))

//        print (beebAudioFormat)
//        print (buffer.format)

        audioEngine.attach(audioBufferPlayerNode) // attach the player - which can play PCM or from files.
        // schedule playing from the buffer, now, and looping, so doesn't complete
        audioEngine.connect(audioBufferPlayerNode, to: mainMixer, format: nil)  // connect player to the mixer using the player format

        audioEngine.attach(audioBufferPlayerNode5000) // attach the player - which can play PCM or from files.
        // schedule playing from the buffer, now, and looping, so doesn't complete
        audioEngine.connect(audioBufferPlayerNode5000, to: mainMixer, format: nil)  // connect player to the mixer using the player format

        for n in (0 ..< kbufs) {
            audioBuffers[n]!.frameLength = AVAudioFrameCount(SoundLength)
            audioBuffers5000[n]!.frameLength = AVAudioFrameCount(SoundLength5000)
        }
        
//        audioBufferPlayer.scheduleBuffer(buffer, at: nil, options: .loops, completionHandler: nil)
        
        // start the engine
        try audioEngine.start()

        
        // some help?
//    https://gist.github.com/michaeldorner/746c659476429a86a9970faaa6f95ec4
        
        // play the buffer immediately
        audioBufferPlayerNode.play()

        audioBufferPlayerNode5000.play()

        successfullystarted = audioBufferPlayerNode.isPlaying && audioBufferPlayerNode5000.isPlaying

    }
    catch {
        print ("")
        print (error)
        print ("")
    }
    
    
    if !successfullystarted      {  swift_CloseAudio()}

}

/*
 
 start - start buffer playing, on termination set a flag
 soundstream - switch buffers, copy soundbuffer into that buffer, start that buffer playing, on termination set a flag

 async - play next buffer, on completion, play buffer
 soundstream - fill buffer, switch to next buffer (for next fill)
 
 each call to soundstream : fill buffer 1, schedule buffer 1, fill buffer 2, schedule buffer 2, fill buffer 3, schedule buffer 3, etc.
 initia
 
 */

var bufferIndex = SoundLength
var bufferIndex5000 = SoundLength5000

var aplaying = 0
var a5playing = 0

// allow access to this in C
@_cdecl("swift_SoundStream")
public func swift_SoundStream( _ soundbuffer: UnsafeMutablePointer<UInt8>, _ outputType : UInt8)
{
    // !2 = DEFAULT (8 bit 1 channel)
    // 2  = MUSIC5000 (16 bit 2 channel)
    if !successfullystarted{ print("early return");return}

    // this comes through as a square wave with 8 bits mono
    // at a sample rate of 44100 Hz
    
    var bytesPerSample = 1 //bits_per_sample / 8;
    var channels = 1 // mono
    var buflen = SoundLength
    var ab = audiobufferIndex
    var abuffer = audioBuffers[ab]

    if outputType == 1
    {
        if aplaying >= kbufs
        {
            print ("stopplay",ab, aplaying,kbufs)
            // terminate all the existing scheduled buffers - this thread will wait while waiting for those to stop
            audioBufferPlayerNode.stop()
            audioBufferPlayerNode.play()
            aplaying = 0
        }
//        print ("start",ab, aplaying)
    }



    if outputType == 2
    {
        bytesPerSample = 2
        channels = 2
        buflen = SoundLength5000
        ab = audiobufferIndex5000
        abuffer = audioBuffers5000[ab]

        if a5playing >= kbufs
        {
            print ("stopplay2",ab, a5playing,kbufs)
            // terminate all the existing scheduled buffers - this thread will wait while waiting for those to stop
            audioBufferPlayerNode5000.stop()
            audioBufferPlayerNode5000.play()
            a5playing = 0
        }
        //            print ("start2",ab,a5playing)

    }

    let srcLen = buflen * bytesPerSample * channels

    var bindex = 0

    for srcIndex in stride(from:0, to: srcLen, by: channels * bytesPerSample) {
        
        if outputType == 2
        {
            // add in the upper byte of the word
            let svalL = (UInt16(soundbuffer[srcIndex+1]) << 8) + UInt16(soundbuffer[srcIndex+0])
            let svalR = (UInt16(soundbuffer[srcIndex+3]) << 8) + UInt16(soundbuffer[srcIndex+2])

            // convert to float : original data is INT16 (signed)
            //range -1..1

            let val_fL = (svalL > 0x7fff ? -Float(~svalL + 1) : Float(svalL))/32767.0
            let val_fR = (svalR > 0x7fff ? -Float(~svalR + 1) : Float(svalR))/32767.0
            
            abuffer?.floatChannelData![0][bindex] = val_fL
            abuffer?.floatChannelData![1][bindex] = val_fR

            
            if bindex >= Int(abuffer!.frameCapacity)
            {
                print("overflow2",audiobufferIndex5000, srcIndex, bindex)
            }
            bindex += 1

        }
        else
        {
            let sval : UInt16 = UInt16(soundbuffer[srcIndex])
            
            
//            if beebAudioFormat.commonFormat == .pcmFormatInt32
//            {
//                let val_i = (Int32(sval)-128)<<24
//        //            print(String(format:"%02X", sval),val_i)
////                abuffer.int32ChannelData!.pointee[bufferIndex] = val_i
//                abuffer.int32ChannelData![0][bufferIndex] = val_i
//                abuffer.int32ChannelData![1][bufferIndex] = val_i
//            }
//            else
//            {
                // convert to float : original data is UINT8 (0x80 = zero)

                let val_f = (Float(sval)/Float(1<<8))-0.5
    //                print(String(format:"%02X", sval),val_f)
                //                abuffer.floatChannelData!.pointee[bufferIndex] = val_f
                abuffer?.floatChannelData![0][bindex] = val_f
                abuffer?.floatChannelData![1][bindex] = val_f
//            }
            
            if bindex >= Int(abuffer!.frameCapacity)
            {
                print("overflow",audiobufferIndex, srcIndex,bindex)
            }
            bindex += 1
            
//            bindex %= Int(abuffer!.frameCapacity)

        }

    }

    if outputType == 1
    {
        aplaying+=1
        audiobufferIndex = (audiobufferIndex + 1) % audioBuffers.count
//        print ("schedule", ab, aplaying)
        audioBufferPlayerNode.scheduleBuffer(abuffer!)
        {
            aplaying-=1
//            print ("done", ab, aplaying)
        }
    }
    
    if outputType == 2
    {
        a5playing+=1
        audiobufferIndex5000 = (audiobufferIndex5000 + 1) % audioBuffers5000.count
//        print ("schedule2", ab, aplaying)
        audioBufferPlayerNode5000.scheduleBuffer(abuffer!)
        {
            a5playing-=1
//            print ("done2", ab,a5playing)
        }

    }

    

//
//    if outputType == 2
//    {
//        // Schedule the buffer for playback and release it for reuse after
//        // playback has finished.
//        audioBufferPlayer5000.scheduleBuffer(audioBuffer!) {
//            audioSemaphore.signal()
//            return
//        }
//
//    }
    
//    const int DEFAULT_SAMPLE_RATE = 44100;
//    const int MAXBUFSIZE = 32768;
//
//    CreateSoundStreamer(SoundSampleRate, 8, 1);
//    std::size_t rate,
//                               std::size_t bits_per_sample,
//                               std::size_t channels)
    

    
    // have a buffer 4 times longer than needed
    // copy the soundBuffer into a part of that buffer
    
//    soundbuffer.
//    print(String(bytes: seq, encoding: .ascii ))
    
    // NOTES:
    
    // THIS EMULATOR IS RUNING QUICKLY
    // SOUNDS WILL BE SHORTER AND MAYBE HIGH PITCHED
    
    // BBC BASIC TEST
    // SOUND 1,-15,400,20


}



// allow access to this in C
@_cdecl("swift_CloseAudio")
public func swift_CloseAudio()
{
    print("closeaudio")
    //  Perhaps use the dispatch queue to stop the audioplayer
    //    DispatchQueue.main.async {
    //    }
    if audioBufferPlayerNode5000.isPlaying{        audioBufferPlayerNode5000.stop()}
    if audioBufferPlayerNode.isPlaying{        audioBufferPlayerNode.stop()}
    if audioEngine.isRunning {       audioEngine.stop()}
    successfullystarted=false
    audiobufferIndex=0
    audiobufferIndex5000=0

}

@_cdecl("swift_getPasteboard")
public func swift_getPasteboard(_ text: UnsafeMutablePointer<CChar>, _ length: Int) -> Int
{
    let pasteboard = NSPasteboard.general

    if let string = pasteboard.string(forType: .string) {
        // text was found and placed in the "string" constant
        
        // set the filepath back in the C code - fill with zeros first
        text.assign(repeating: 0, count: length)
        text.assign(from: string, count: string.count)
        return 1
    }
    return 0
}


@_cdecl("swift_setPasteboard")
public func swift_setPasteboard(_ text: UnsafePointer<CChar>, _ length: UInt)
{
    let pasteboard = NSPasteboard.general

    pasteboard.clearContents()
    pasteboard.setString(String(cString:text), forType: .string)
}


