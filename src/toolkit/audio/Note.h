#ifndef TOOLKIT_AUDIO_NOTE_H
#define TOOLKIT_AUDIO_NOTE_H

#include <toolkit/core/types.h>
#include <tuple>

namespace TOOLKIT_NS { namespace audio
{

	struct Note
	{
		int Octave = 0;
		uint Tone = 0;

		bool operator < (const Note & o) const
		{ return std::tie(Octave, Tone) < std::tie(Octave, Tone); }
		bool operator == (const Note & o) const
		{ return std::tie(Octave, Tone) == std::tie(Octave, Tone); }
	};

}}

#endif
