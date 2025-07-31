#ifndef TOOLKIT_AUDIO_KEY_H
#define TOOLKIT_AUDIO_KEY_H

#include <toolkit/core/types.h>
#include <tuple>

namespace TOOLKIT_NS { namespace audio
{

	struct Key
	{
		int Octave = 0;
		uint Tone = 0;

		bool operator < (const Key & o) const
		{ return std::tie(Octave, Tone) < std::tie(Octave, Tone); }
		bool operator == (const Key & o) const
		{ return std::tie(Octave, Tone) == std::tie(Octave, Tone); }
	};

}}

#endif
