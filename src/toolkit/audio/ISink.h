#ifndef TOOLKIT_AUDIO_ISINK_H
#define TOOLKIT_AUDIO_ISINK_H

#include <toolkit/audio/Key.h>
#include <toolkit/core/types.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{
	struct KeyEvent
	{
		audio::Key 	Key;
		float		Pressure = 1.0f;
	};

	struct ISink
	{
		virtual ~ISink() = default;

		virtual void HandleBeat(int beat) = 0;
		virtual void HandlePress(const KeyEvent &key) = 0;
		virtual void HandleRelease(const KeyEvent &key) = 0;
	};
	TOOLKIT_DECLARE_PTR(ISink);

}}

#endif
