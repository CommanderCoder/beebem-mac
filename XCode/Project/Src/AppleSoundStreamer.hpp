//
//  AppleSoundStreamer.hpp
//  BeebEm
//
//  Created by Commander Coder on 06/06/2024.
//

#ifndef AppleSoundStreamer_hpp
#define AppleSoundStreamer_hpp

#include <stdio.h>
#include "apples.h"
#include "SoundStreamer.h"
#include "Main.h"



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
			  std::size_t channels) override;

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

	virtual void Stream(const void*pSamples) override;

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


#endif /* AppleSoundStreamer_hpp */
