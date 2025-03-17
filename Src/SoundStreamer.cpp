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

#include <algorithm>

#include "SoundStreamer.h"
#ifndef __APPLE__
#include "DirectSoundStreamer.h"
#endif
#include "Main.h"
#include "Sound.h"
#ifndef __APPLE__
#include "XAudio2Streamer.h"
#endif
#ifdef __APPLE__
#include "AppleSoundStreamer.hpp"
#endif

std::vector<SoundStreamer*> SoundStreamer::m_streamers;

/****************************************************************************/

SoundStreamer::SoundStreamer()
{
	m_streamers.push_back(this);
}

/****************************************************************************/

SoundStreamer::~SoundStreamer()
{
	m_streamers.erase(std::find(m_streamers.begin(), m_streamers.end(), this));
}

/****************************************************************************/

void SoundStreamer::PlayAll()
{
	for (auto i = m_streamers.begin(); i != m_streamers.end(); ++i)
	{
		(*i)->Play();
	}
}

/****************************************************************************/

void SoundStreamer::PauseAll()
{
	for (auto i = m_streamers.begin(); i != m_streamers.end(); ++i)
	{
		(*i)->Pause();
	}
}

/****************************************************************************/

SoundStreamer *CreateSoundStreamer(int samplerate, int bits_per_sample, int channels)
{
#ifndef __APPLE__
	if (SelectedSoundStreamer == SoundStreamerType::XAudio2)
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

			SelectedSoundStreamer = SoundStreamerType::DirectSound;
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
		// create streams in the sound systems - now init (start) the sound system
		swift_SoundInit();

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

/****************************************************************************/
