#ifndef TOOLKIT_AUDIO_ISINK_H
#define TOOLKIT_AUDIO_ISINK_H

#include <toolkit/audio/Key.h>
#include <toolkit/text/StringOutputStream.h>
#include <toolkit/core/types.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{
	struct BeatEvent
	{
		unsigned Bar;
		unsigned Unit;
		unsigned Units;

		void ToString(text::StringOutputStream &sos) const
		{ sos << "BeatEvent { bar: " << Bar << ", " << Unit << "/" << Units << "}"; }
		TOOLKIT_DECLARE_SIMPLE_TOSTRING();
	};
	struct KeyEvent
	{
		audio::Key 	Key;
		float		Pressure = 1.0f;
		void ToString(text::StringOutputStream &sos) const
		{ sos << "KeyEvent { " << Key << ", pressure: " << Pressure << "}"; }
		TOOLKIT_DECLARE_SIMPLE_TOSTRING();
	};

	struct ISink
	{
		virtual ~ISink() = default;

		virtual void HandleBeat(const BeatEvent &beat) = 0;
		virtual void HandlePress(const KeyEvent &key) = 0;
		virtual void HandleRelease(const KeyEvent &key) = 0;
	};
	TOOLKIT_DECLARE_PTR(ISink);

}}

#endif
