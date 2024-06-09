//
//  BeebEm-Sound.swift
//  BeebEm
//
//  Created by Commander Coder on 19/04/2024.
//

import Foundation
import AVFoundation

// have to put a barrier around the freelist read/write otherwise
// it will crash as this async write could occur at the same time
// as a read.

// should be using actor rather than class
// as the 'freelist' is update in async code
class FreeList : CustomStringConvertible
{
	private var n:Int
	init(_ v:Int)
	{
		items = Array(0..<v)
		n = v
	}
	
	var available: Int {
		get { return items.count }
	  }
	var used: Int {
		get { return self.n - items.count }
	  }

	private var items : [Int] = []
	
	func give(_ n: Int)
	{
		items.append(n)
	}
	func take() -> Int
	{
		return items.popLast() ?? -1
	}
	var description: String {
	  return "\(self.items)"
	}
}

class AudioFormat
{
	var soundrate : Int = 0
	var soundlength : Int = 0
	var audioformat : AVAudioFormat
	var playernode : AVAudioPlayerNode
	var audiobuffers : [AVAudioPCMBuffer] = []
	var bytesPerSample : Int = 0
	var freelist: FreeList
	var channels : Int = 0

	init(soundrate: Int,
		 soundlength : Int,
		 audioformat : AVAudioFormat,
		 playernode : AVAudioPlayerNode,
		 audiobuffers : [AVAudioPCMBuffer],
		 freelist : FreeList,
		 bytesPerSample : Int,
		 channels : Int
	){
		self.soundrate=soundrate
		self.soundlength=soundlength
		self.audioformat=audioformat
		self.playernode=playernode
		self.audiobuffers=audiobuffers
		self.freelist=freelist
		self.bytesPerSample=bytesPerSample
		self.channels=channels
	}
	
}

var StreamKey = 0
var AudioStreams : [Int:AudioFormat] = [:]

let kbufs = 10 // internal buffers per stream but there are also external ones

//44100 * 20/1000 = 882  = 44100 / 50fps
// incoming sound length ;Int (@50 fps), rate / 50


// some help?
//    https://gist.github.com/michaeldorner/746c659476429a86a9970faaa6f95ec4

// the audio engine - MUST NOT DEALLOCATE hence OUTSIDE FUNC
var audioEngine = AVAudioEngine()

// mixer - to configure the output
// mixer has multiple inputs but one output that has 2 channels - for mainmixer this goes to the audio output
var mainMixer = audioEngine.mainMixerNode

// audio input is from mic
// audio player is from buffer/segments
// audio output is to loudspeaker

var successfullystarted = false



@_cdecl("swift_ReleaseSoundBuffer")
public func swift_ReleaseSoundBuffer(_ index : Int)
{
	AudioStreams.removeValue(forKey: index)
//	print("release: ",index, AudioStreams.keys)
}

@_cdecl("swift_CreateSoundBuffer")
public func swift_CreateSoundBuffer(
	_ nChannels : Int16,
	_ nSamplesPerSec : Int, //dword
	_ nAvgBytesPerSec : Int, //dword
	_ nBlockAlign : Int16,
	_ wBytesPerSample : Int16
) -> Int
{
	let beebAudioFormat = AVAudioFormat(
		commonFormat: AVAudioCommonFormat.pcmFormatFloat32,
		sampleRate: Double(nSamplesPerSec),
		channels: AVAudioChannelCount(2), // always 2
		interleaved: false)!

	let size = nSamplesPerSec * 20 / 1000
	
	let dat = AudioFormat(
		soundrate: nSamplesPerSec,
		soundlength: size,
		audioformat: beebAudioFormat,
		playernode: AVAudioPlayerNode(), // the player used for scheduling from buffer
		audiobuffers: (0 ..< kbufs).map { _ in AVAudioPCMBuffer(pcmFormat: beebAudioFormat, frameCapacity: UInt32(size))! }, //create a pool of buffers with the correct format - give it enough room to hold onto as much data as needed
		freelist : FreeList(kbufs), // buffers not currently in use
		bytesPerSample: Int(wBytesPerSample),
		channels: Int(nChannels)
	)
	
	let playing = AudioStreams.filter{ a in a.value.playernode.isPlaying }
	// go through the attached AVAudioPlayerNodes and make an
	// array of those that are playing

	// stop the engine before attaching and connecting nodes
//	audioEngine.stop()
//	print ("stop:")

	audioEngine.attach(dat.playernode) // attach the player - which can play PCM or from files.
	// schedule playing from the buffer, now, and looping, so doesn't complete
	audioEngine.connect(dat.playernode, to: mainMixer, format: nil)  // connect player to the mixer using the player format

	// don't remove streams  - or the IDs that are being returned will not match up
	let index = StreamKey
	StreamKey += 1
	AudioStreams[index] = dat
//	print("create: ", index, AudioStreams.keys)
	
	// replay those nodes that were playing
	for p in playing { swift_PlayStream(p.key) }
	
	return index
	
	// probably shoudn't use BUS 0 by default
//        print (dat.playernode.outputFormat(forBus: 0))
//        print (mainMixer.outputFormat(forBus: 0))

//        print (beebAudioFormat)
//        print (dat.audiobuffers[0].format)

//        dat.playernode.scheduleBuffer(buffer, at: nil, options: .loops, completionHandler: nil)
	
}


// initialise soundbuffers for normal sound, and music 5000 sound
@_cdecl("swift_SoundInit")
public func swift_SoundInit()
{

	if successfullystarted {
//		swift_CloseAudio()
		return
	}

	do {
		// start the engine
		try audioEngine.start()
//		print ("start:")

		successfullystarted = true
		for stream in AudioStreams
		{
//			print("init ", stream.key)
			let dat = stream.value
			dat.playernode.play()
			successfullystarted = successfullystarted && dat.playernode.isPlaying
		}
	}
	catch {
		print ("")
		print (error)
		print ("")
	}
	
	if !successfullystarted      {  swift_CloseAudio() }

}

/*
 
 start - start buffer playing, on termination set a flag
 soundstream - switch buffers, copy soundbuffer into that buffer, start that buffer playing, on termination set a flag

 async - play next buffer, on completion, play buffer
 soundstream - fill buffer, switch to next buffer (for next fill)
 
 each call to soundstream : fill buffer 1, schedule buffer 1, fill buffer 2, schedule buffer 2, fill buffer 3, schedule buffer 3, etc.
 initia
 
 */
let queue = DispatchQueue(label: "thread-safe-obj", attributes: .concurrent)

// supply data for the soundbuffer of either normal or music 5000
@_cdecl("swift_SubmitStream")
public func swift_SubmitStream(_ index : Int, _ soundbuffer: UnsafeMutablePointer<UInt8>, _ length: Int)
{
	// !2 = DEFAULT (8 bit 1 channel)
	// 2  = MUSIC5000 (16 bit 2 channel)
	guard audioEngine.isRunning else { print("early return"); return}

	// this comes through as a square wave with 8 bits mono
	// at a sample rate of 44100 Hz
	
	
	guard let dat = AudioStreams[index] else { print("bad index"); return}
	let buflen = dat.soundlength
	
	// read
	var _fl: Int!
	queue.sync {

		if dat.freelist.available == 0
		{
			// this will overwrite buffer 0 if all others are in use
			print ("stopplay",kbufs)
			// terminate all the existing scheduled buffers - this thread will wait while waiting for those to stop
			swift_StopStream(index)
			swift_PlayStream(index)
			
			// and possibly write
			dat.freelist = FreeList(kbufs)
		}
		
		// perform read and assign value
	_fl = dat.freelist.take()
	}
	let fl = _fl!

	let abuffer = dat.audiobuffers[fl]
	let bytesPerSample = dat.bytesPerSample
	let channels = dat.channels

	
	let srcLen = buflen * bytesPerSample * channels

//	print("buffer: ", index, fl, length, srcLen)

	var bindex = 0

	for srcIndex in stride(from:0, to: srcLen, by: channels * bytesPerSample) {
	
		var val_f: [Float] = [0,0]

		if (bytesPerSample == 1)
		{
			let sval : UInt16 = UInt16(soundbuffer[srcIndex])
			val_f[0] = (Float(sval)/Float(1<<8))-0.5
			val_f[1] = val_f[0]
		}
		
		if (bytesPerSample == 2)
		{
			// add in the upper byte of the word
			let svalL = (UInt16(soundbuffer[srcIndex+1]) << 8) + UInt16(soundbuffer[srcIndex+0])
			let svalR = (UInt16(soundbuffer[srcIndex+3]) << 8) + UInt16(soundbuffer[srcIndex+2])

			// convert to float : original data is INT16 (signed)
			//range -1..1

			val_f[0] = (svalL > 0x7fff ? -Float(~svalL + 1) : Float(svalL))/32767.0
			val_f[1] = (svalR > 0x7fff ? -Float(~svalR + 1) : Float(svalR))/32767.0
		}
		
		for c in 0...1 // always 2 channels
		{
			abuffer.floatChannelData![c][bindex] = val_f[c]
		}
		
		if bindex >= Int(abuffer.frameCapacity)
		{
			print("overflow: ",fl, srcIndex, bindex)
		}
		bindex += 1
	}
	// set the length to the amount of data in the buffer
	abuffer.frameLength = AVAudioFrameCount(bindex)

//	print ("schedule: ", index, fl, dat.freelist)
	dat.playernode.scheduleBuffer(abuffer){
		queue.async(flags: .barrier) {
			dat.freelist.give(fl)
//			print ("done: ", index, fl, dat.freelist)
		}
	}

	// NOTES:
	
	// THIS EMULATOR IS RUNNING QUICKLY
	// SOUNDS WILL BE SHORTER AND MAYBE HIGH PITCHED
	
	// BBC BASIC TEST
	// SOUND 1,-15,400,20
}



@_cdecl("swift_BufferedStreams")
public func swift_BufferedStreams(_ index: Int) -> Int
{
	guard let dat = AudioStreams[index] else {print("bad bufferedstreams index");return 0}
	return dat.freelist.used
}

@_cdecl("swift_PlayStream")
public func swift_PlayStream(_ index: Int)
{
	guard let dat = AudioStreams[index] else {print("bad play index");return}
	if (audioEngine.isRunning)
	{
		dat.playernode.play()
//		print ("streamplay: ",index)
	}
}

@_cdecl("swift_StopStream")
public func swift_StopStream(_ index: Int)
{
	guard let dat = AudioStreams[index] else {print("bad stop index");return}
	dat.playernode.stop()
//	print ("stopstream: ",index)
}


// close down all the sound buffers
@_cdecl("swift_CloseAudio")
public func swift_CloseAudio()
{
	audioEngine.stop()
	print ("stop:")
	for stream in AudioStreams
	{
		swift_PlayStream(stream.key)

		let dat = stream.value
		audioEngine.disconnectNodeInput(dat.playernode)
		audioEngine.detach(dat.playernode)
		dat.freelist = FreeList(kbufs)
	}
	successfullystarted=false
}


