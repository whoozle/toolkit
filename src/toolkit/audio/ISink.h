#ifndef TOOLKIT_AUDIO_ISINK_H
#define TOOLKIT_AUDIO_ISINK_H

#include <toolkit/core/types.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{

	struct ISink
	{
		virtual ~ISink() = default;

		virtual void HandleBeat(int beat) = 0;
	};
	TOOLKIT_DECLARE_PTR(ISink);

}}

#endif
