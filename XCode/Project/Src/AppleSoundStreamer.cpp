//
//  AppleSoundStreamer.cpp
//  BeebEm
//
//  Created by Commander Coder on 06/06/2024.
//

#include "AppleSoundStreamer.hpp"



bool AppleSoundStreamer::Init(std::size_t rate,
		  std::size_t bits_per_sample,
		  std::size_t channels)
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


void AppleSoundStreamer::Stream(const void*pSamples)
{
	// Verify buffer availability
	int BuffersQueued = swift_BufferedStreams(m_pDirectSoundBuffer);
	
	if (BuffersQueued >= m_count)
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
