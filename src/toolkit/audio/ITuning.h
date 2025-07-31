#ifndef TOOLKIT_AUDIO_ITUNING_H
#define TOOLKIT_AUDIO_ITUNING_H

#include <toolkit/audio/Key.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{

	struct ITuning
	{
		virtual ~ITuning() = default;

		virtual float GetFrequency(Key note) const = 0;
	};
	TOOLKIT_DECLARE_PTR(ITuning);

}}

#endif
