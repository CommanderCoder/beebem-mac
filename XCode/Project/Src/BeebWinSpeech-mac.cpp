//
//  BeebWinSpeech-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 05/05/2024.
//

#include <stdio.h>


// BeebWin text to speech support

#include <windows.h>

#define STRSAFE_NO_DEPRECATE
#include <sphelper.h>

#include <assert.h>
#include <stdio.h>

#include "BeebWin.h"
#include "DebugTrace.h"
#include "Main.h"
#include "Messages.h"
#include "Resource.h"
#include "StringUtils.h"


/****************************************************************************/
bool BeebWin::InitTextToSpeech()
{

	return true;
}

void BeebWin::TextToSpeechResetState()
{
}

void BeebWin::CloseTextToSpeech()
{
}

bool BeebWin::InitTextToSpeechVoices()
{


	return true;
}

static int GetMenuItemPosition(HMENU hMenu, const char *pszMenuItem)
{
	return 0;
}

void BeebWin::InitVoiceMenu()
{
}

int BeebWin::TextToSpeechGetSelectedVoice()
{
	return 0;
}

// Returns the token for the selected voice, or nullptr to use the default voice.

ISpObjectToken* BeebWin::TextToSpeechGetSelectedVoiceToken()
{
	ISpObjectToken* pToken = nullptr;

	return pToken;
}

void BeebWin::TextToSpeechSetVoice(int Index)
{
	
	
}

void BeebWin::TextToSpeechSetVoice(ISpObjectToken* pToken)
{
	
}

void BeebWin::TextToSpeechSelectVoiceMenuItem(int Index)
{
}

void BeebWin::Speak(const char *text, DWORD flags)
{
	
}

static constexpr bool IsEndSentence(char c)
{
	return c == '.' || c == '!' || c == '?';
}

void BeebWin::SpeakChar(unsigned char c)
{
	
}

void BeebWin::TextToSpeechClearBuffer()
{
	
}

// Toggle text to speech for all text writes through WRCHV

void BeebWin::TextToSpeechToggleAutoSpeak()
{
	
}

// Toggle speaking of punctuation

void BeebWin::TextToSpeechToggleSpeakPunctuation()
{
	
}

void BeebWin::TextToSpeechIncreaseRate()
{
	
}

void BeebWin::TextToSpeechDecreaseRate()
{
	
}

bool BeebWin::TextToSpeechSearch(TextToSpeechSearchDirection dir,
								 TextToSpeechSearchType type)
{
	return false;
}

void BeebWin::TextToSpeechReadChar()
{
}

void BeebWin::TextToSpeechReadWord()
{
}

void BeebWin::TextToSpeechReadLine()
{
}

void BeebWin::TextToSpeechReadSentence()
{

}

void BeebWin::TextToSpeechReadScreen()
{
	
}

void BeebWin::TextToSpeechKey(WPARAM wParam)
{
	
}

/****************************************************************************/


void BeebWin::InitTextView()
{
	
	
}

void BeebWin::CloseTextView()
{
	
	
}

void BeebWin::TextView()
{
	
}

void BeebWin::TextViewSpeechSync()
{
	}

void BeebWin::TextViewSetCursorPos(int line, int col)
{
}

void BeebWin::TextViewSyncWithBeebCursor()
{
}

