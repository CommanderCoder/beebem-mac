/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 1994  David Alan Gilbert
Copyright (C) 1997  Mike Wyatt
Copyright (C) 2001  Richard Gellman
Copyright (C) 2008  Rich Talbot-Watkins

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

#include "SoundStreamer.h"
#ifndef __APPLE__
#include "DirectSoundStreamer.h"
#endif
#include "Main.h"
#ifndef __APPLE__
#include "XAudio2Streamer.h"
#endif

std::list<SoundStreamer*> SoundStreamer::m_streamers;

SoundStreamer::SoundStreamer()
{
	m_streamers.push_back(this);
}

SoundStreamer::~SoundStreamer()
{
	m_streamers.remove(this);
}

void SoundStreamer::PlayAll()
{
	std::list<SoundStreamer*>::iterator li;
	for (li = m_streamers.begin(); li != m_streamers.end(); li++)
		(*li)->Play();
}

void SoundStreamer::PauseAll()
{
	std::list<SoundStreamer*>::iterator li;
	for (li = m_streamers.begin(); li != m_streamers.end(); li++)
		(*li)->Pause();
}


#ifdef __APPLE__
class AppleSoundStreamer : public SoundStreamer
{

public:
	AppleSoundStreamer() :
		m_samples(0),
		m_pDirectSoundBuffer(-1),
		m_index(0),
		m_rate(0),
		m_size(0),
		m_bytespersample(0),
		m_channels(0)
	{
	}

	virtual ~AppleSoundStreamer()
	{
		if (m_pDirectSoundBuffer>=0)
		{
			swift_ReleaseSoundBuffer(m_pDirectSoundBuffer);
		}
	}

	bool Init(std::size_t rate,
			  std::size_t bits_per_sample,
			  std::size_t channels) override
	{
		if (bits_per_sample != 8 && bits_per_sample != 16)
		{
			return false;
		}

		if (channels != 1 && channels != 2)
		{
			return false;
		}

		m_rate = rate;
		m_size = rate * 20 / 1000;
		m_bytespersample = bits_per_sample / 8;
		m_channels = channels;

		// Allocate memory for buffers
		m_samples.resize(m_count * m_size * m_bytespersample * m_channels,
						 bits_per_sample == 8 ? 128 : 0);


		WORD blockalign = m_channels * ((WORD)(m_bytespersample * 8)/8);
		// See BeebEm-Sound.swift for actual sound setup
		m_pDirectSoundBuffer = swift_CreateSoundBuffer((WORD)m_channels, (DWORD)m_rate, (DWORD)m_rate * blockalign, blockalign, (WORD)(m_bytespersample));

		// Start source voice
		Play();

		return true;
	}

	virtual std::size_t BufferSize() const override
	{
		return m_size;
	}

	virtual void Play() override
	{
		swift_PlayStream(m_pDirectSoundBuffer);
	}

	virtual void Pause() override
	{
		swift_StopStream(m_pDirectSoundBuffer);
	}

	virtual void Stream(const void*pSamples) override
	{
		// Verify buffer availability
		int BuffersQueued = swift_BufferedStreams(m_pDirectSoundBuffer);
		
		if (BuffersQueued == m_count)
			return;
		
		// Copy samples to buffer
		Sample *pBuffer = &m_samples[m_index * m_size * m_bytespersample * m_channels];

		memcpy(pBuffer, pSamples, m_size * m_bytespersample * m_channels);

		// Submit buffer to voice

		
		//		printf("C 0x%x 0x%x\n", (INT16)SampleBuf[0] , (INT16)SampleBuf[1]);
				// outputtype 2 = 16 bit, 2 channel

		int AudioBytes = (UINT32)(m_size * m_bytespersample * m_channels);
		swift_SubmitStream(m_pDirectSoundBuffer, (BYTE *)pBuffer, AudioBytes);
		
		// Select next buffer
	 if (++m_index == m_count)
	 {
		 m_index = 0;
	 }

	}

private:
	typedef unsigned char Sample;
	typedef std::vector<Sample> Samples;
	Samples m_samples;

	int m_pDirectSoundBuffer;

	std::size_t m_index;
	std::size_t m_rate;
	std::size_t m_size;
	std::size_t m_bytespersample;
	std::size_t m_channels;

	static std::size_t const m_count = 4;
};
#endif

SoundStreamer *CreateSoundStreamer(int samplerate, int bits_per_sample, int channels)
{
#ifndef __APPLE__
	if (SoundConfig::Selection == SoundConfig::XAudio2)
	{
		SoundStreamer *pSoundStreamer = new XAudio2Streamer();

		if (pSoundStreamer->Init(samplerate, bits_per_sample, channels))
		{
			return pSoundStreamer;
		}
		else
		{
			delete pSoundStreamer;
			pSoundStreamer= nullptr;

			SoundConfig::Selection = SoundConfig::DirectSound;
		}
	}

	SoundStreamer *pSoundStreamer = new DirectSoundStreamer();

	if (pSoundStreamer->Init(samplerate, bits_per_sample, channels))
	{
		return pSoundStreamer;
	}
	else
	{
		delete pSoundStreamer;
		pSoundStreamer= nullptr;

		mainWin->Report(MessageType::Error, "Attempt to start sound system failed");

		return nullptr;
	}
#else

	SoundStreamer *pSoundStreamer = new AppleSoundStreamer();
	if (pSoundStreamer->Init(samplerate, bits_per_sample, channels))
	{
		return pSoundStreamer;
	}
	else
	{
	   delete pSoundStreamer;
	   pSoundStreamer= nullptr;
		mainWin->Report(MessageType::Error, "Attempt to start sound system failed");

		return nullptr;
	}

#endif

	return pSoundStreamer;
}
