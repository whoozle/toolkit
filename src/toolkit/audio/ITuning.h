#ifndef TOOLKIT_AUDIO_ITUNING_H
#define TOOLKIT_AUDIO_ITUNING_H

#include <toolkit/audio/Note.h>

namespace TOOLKIT_NS { namespace audio
{

	struct ITuning
	{
		virtual ~ITuning() = default;

		virtual float GetFrequency(Note note) const = 0;
	};

}}

#endif
