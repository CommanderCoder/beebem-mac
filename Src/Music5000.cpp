/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 2016  Mike Wyatt

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.
****************************************************************/

//
// BeebEm Music 5000 emulation
//
// Mike Wyatt - Jan 2016
//

#include <windows.h>

#include <stdio.h>

#include "Music5000.h"
#include "SoundStreamer.h"
#include "UefState.h"

bool Music5000Enabled = false;
unsigned char JimPageSelectRegister;
extern int SoundVolume;

#define RAM_SIZE              2048
#define WAVE_TABLE_SIZE       128
#define WAVE_TABLES           14
#define NUM_CHANNELS          16
#define NUM_REG_SETS          2

// Control register bits
#define CTRL_STEREO_POS       (0xf)
#define CTRL_INVERT_WAVE      (1 << 4)
#define CTRL_MODULATE_ADJ     (1 << 5)

// Data bits
#define DATA_SIGN             (1 << 7)
#define DATA_VALUE            (0x7f)
#define FREQ_DISABLE          (1 << 0)

// Channel register sets
#define REG_SET_NORMAL        0
#define REG_SET_ALT           1

struct CHANNELREGS
{
	unsigned char FreqLo[NUM_CHANNELS];
	unsigned char FreqMed[NUM_CHANNELS];
	unsigned char FreqHi[NUM_CHANNELS];
	unsigned char Unused1[NUM_CHANNELS];
	unsigned char Unused2[NUM_CHANNELS];
	unsigned char WaveformReg[NUM_CHANNELS];
	unsigned char AmplitudeReg[NUM_CHANNELS];
	unsigned char ControlReg[NUM_CHANNELS];
};

struct WAVERAM
{
	unsigned char WaveTable[WAVE_TABLES][WAVE_TABLE_SIZE];
	CHANNELREGS ChannelRegs[NUM_REG_SETS];  // Normal & Alt
};

static unsigned char WaveRam[RAM_SIZE];
static WAVERAM* pWaveRam = (WAVERAM*)WaveRam;
static unsigned int PhaseRam[NUM_CHANNELS];

static const double ChordBase[8] = { 0, 8.25, 24.75, 57.75, 123.75, 255.75, 519.75, 1047.75 };
static const double StepInc[8] = { 0.5, 1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0 };
static int D2ATable[128];

static const int StereoLeft[16]  = {   0,  0,  0,  0,  0,  0,  0,  0,100,100,100, 83, 67, 50, 33, 17 };
static const int StereoRight[16] = { 100,100,100,100,100,100,100,100,  0,  0,  0, 17, 33, 50, 67, 83 };

// 6MHz clock, 128 cycles to update all channels
static unsigned int CycleCount = 0;

static short *SampleBuf = nullptr;
static unsigned int SampleBufSize = 0;
static unsigned int SampleWritePtr = 0;

static unsigned int CurCh;
static unsigned int ActiveRegSet;
static int SampleLeft;
static int SampleRight;

static SoundStreamer *pSoundStreamer = nullptr;

#ifdef __APPLE__
static bool alreadyCreated = false;
#endif

void Music5000Init()
{
#ifdef __APPLE__
	if (!alreadyCreated)
	{
#endif
	static_assert(sizeof(WAVERAM) == RAM_SIZE, "WAVERAM size");

	ZeroMemory(WaveRam, sizeof(WaveRam));
	ZeroMemory(PhaseRam, sizeof(PhaseRam));
	CycleCount = 0;
	CurCh = 0;
	ActiveRegSet = REG_SET_NORMAL;
	SampleLeft = 0;
	SampleRight = 0;
	SampleWritePtr = 0;

	// Build the D2A table
	int i = 0;

	for (int chord = 0; chord < 8; chord++)
	{
		double val = ChordBase[chord];

		for (int step = 0; step < 16; step++)
		{
			D2ATable[i] = (INT)(val * 4); // Multiply up to get an integer
			val += StepInc[chord];
			i++;
		}
	}

	// Init the streamer
	delete pSoundStreamer;
	pSoundStreamer = CreateSoundStreamer(46875, 16, 2);

	if (!pSoundStreamer)
	{
		Music5000Enabled = false;
	}
	else
	{
		pSoundStreamer->Play();
		SampleBufSize = (unsigned int)pSoundStreamer->BufferSize();
		if (SampleBuf)
			free(SampleBuf);
		SampleBuf = (short*)malloc(SampleBufSize * 4);
		if (SampleBuf == nullptr)
		{
			Music5000Enabled = false;
		}
	}
#ifdef __APPLE__
	alreadyCreated = true;
	}
#endif
}

void Music5000Reset()
{
#ifdef __APPLE__
	if (!alreadyCreated)
	{
#endif
	delete pSoundStreamer;
	pSoundStreamer = nullptr;
#ifdef __APPLE__
	}
#endif
	JimPageSelectRegister = 0;
}

void Music5000Write(int address, unsigned char value)
{
	if (!Music5000Enabled)
		return;

	if (address == 0xfcff)
	{
		JimPageSelectRegister = value;
		return;
	}

	if ((JimPageSelectRegister & 0xf0) != 0x30)
		return;

	if ((address & 0xff00) == 0xfd00) {
		// Bit0 unused
		UINT offset = ((JimPageSelectRegister & 0x0e) << 7) + (address & 0xff);
		WaveRam[offset] = value;
	}

#if 0
	DebugTrace("M5KWrite %02x ?%02x=%02x\n", page, address, value);
#endif
}

bool Music5000Read(int address, unsigned char *value)
{
	if (!Music5000Enabled)
		return false;

	if ((JimPageSelectRegister & 0xf0) != 0x30)
		return false;

	if (address == 0xfcff)
	{
		*value = JimPageSelectRegister;
		return true;
	}

	if ((address & 0xff00) == 0xfd00)
	{
		// Bit0 unused
		UINT offset = ((JimPageSelectRegister & 0x0e) << 7) + address;
		*value = WaveRam[offset];
		return true;
	}

#if 0
	DebugTrace("M5KRead %02x ?%02x=%02x\n", page, address, value);
#endif

	return false;
}

void Music5000Update(UINT cycles)
{
	CHANNELREGS *pChRegs;
	unsigned int freq;
	unsigned int c4d;
	unsigned int wavetable, offset;
	unsigned int amplitude;
	unsigned int control;
	unsigned int data;
	unsigned int sign;
	unsigned int pos;
	int sample;

	// Convert 2MHz 6502 cycles to 6MHz Music5000 cycles
	CycleCount += cycles * 3;

	// Need 8 cycles to update a channel
	while (CycleCount >= 8)
	{
		// Update phase for active register set
		pChRegs = &pWaveRam->ChannelRegs[ActiveRegSet];

		if (pChRegs->FreqLo[CurCh] & FREQ_DISABLE)
		{
			PhaseRam[CurCh] = 0;
			c4d = 0;
		}
		else
		{
			freq = (pChRegs->FreqHi[CurCh] << 16) +
				(pChRegs->FreqMed[CurCh] << 8) + (pChRegs->FreqLo[CurCh] & ~FREQ_DISABLE);
			PhaseRam[CurCh] += freq;
			c4d = PhaseRam[CurCh] & (1<<24);
			PhaseRam[CurCh] &= 0xffffff;
		}

		// Pull wave sample out for the active register set
		offset = (PhaseRam[CurCh] >> 17) & 0x7f;
		wavetable = (pChRegs->WaveformReg[CurCh] >> 4);
		data = pWaveRam->WaveTable[wavetable][offset];
		amplitude = pChRegs->AmplitudeReg[CurCh];
		control = pChRegs->ControlReg[CurCh];
		sign = data & DATA_SIGN;
		data &= DATA_VALUE;

		// Modulate the next channel?
		if ((control & CTRL_MODULATE_ADJ) && (sign || c4d))
			ActiveRegSet = REG_SET_ALT;
		else
			ActiveRegSet = REG_SET_NORMAL;

		if (amplitude > 0x80)
			amplitude = 0x80;
		data = data * amplitude / 0x80;

		sample = D2ATable[data];
		if (control & CTRL_INVERT_WAVE)
			sign ^= DATA_SIGN;
		if (sign)
			sample = -sample;

#if 0
		if (sample > 0)
		{
			DebugTrace("S %d, amplitude %d, data %d\n", sample, amplitude, data);
		}
#endif

		// Stereo
		pos = (control & CTRL_STEREO_POS);
		SampleLeft += sample * StereoLeft[pos] / 100;
		SampleRight += sample * StereoRight[pos] / 100;

		if (++CurCh == NUM_CHANNELS)
		{
			CurCh = 0;
			ActiveRegSet = REG_SET_NORMAL;

			// Apply volume
			SampleLeft = (SampleLeft * SoundVolume)/100;
			SampleRight = (SampleRight * SoundVolume)/100;

			// Range check
			if (SampleLeft < -32768)
				SampleLeft = -32768;
			else if (SampleLeft > 32767)
				SampleLeft = 32767;

			if (SampleRight < -32768)
				SampleRight = -32768;
			else if (SampleRight > 32767)
				SampleRight = 32767;

			SampleBuf[SampleWritePtr++] = (short)SampleLeft;
			SampleBuf[SampleWritePtr++] = (short)SampleRight;

			if (SampleWritePtr/2 >= SampleBufSize)
			{
				pSoundStreamer->Stream(SampleBuf);
				SampleWritePtr = 0;
			}

			SampleLeft = 0;
			SampleRight = 0;
		}

		CycleCount -= 8;
	}
}

void SaveMusic5000UEF(FILE *SUEF)
{
	UEFWriteBuf(WaveRam, RAM_SIZE, SUEF);
	UEFWriteBuf(PhaseRam, sizeof(PhaseRam), SUEF);
	UEFWrite8(CurCh, SUEF);
	UEFWrite8(CycleCount, SUEF);
	UEFWrite8(ActiveRegSet, SUEF);
	UEFWrite8(0, SUEF); // Unused pad
	UEFWrite32(SampleLeft, SUEF);
	UEFWrite32(SampleRight, SUEF);
	UEFWrite8(JimPageSelectRegister, SUEF);
}

void LoadMusic5000UEF(FILE *SUEF, int Version)
{
	UEFReadBuf(WaveRam, RAM_SIZE, SUEF);
	UEFReadBuf(PhaseRam, sizeof(PhaseRam), SUEF);
	CurCh = UEFRead8(SUEF);
	CycleCount = UEFRead8(SUEF);
	ActiveRegSet = UEFRead8(SUEF);
	UEFRead8(SUEF); // Unused pad
	SampleLeft = UEFRead32(SUEF);
	SampleRight = UEFRead32(SUEF);

	if (Version >= 13)
	{
		JimPageSelectRegister = UEFRead8(SUEF);
	}
}

void LoadMusic5000JIMPageRegUEF(FILE *SUEF)
{
	JimPageSelectRegister = UEFRead8(SUEF);
}
