//
//  BeebEm-Sound.swift
//  BeebEm
//
//  Created by Commander Coder on 19/04/2024.
//

import Foundation
import AVFoundation




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
