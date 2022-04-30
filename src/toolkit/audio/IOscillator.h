#ifndef TOOLKIT_AUDIO_IOSCILLATOR_H
#define TOOLKIT_AUDIO_IOSCILLATOR_H

#include <toolkit/audio/ISource.h>

namespace TOOLKIT_NS { namespace audio
{

	struct IOscillator : public ISource
	{
		virtual ~IOscillator() = default;

		virtual void SetFrequency(float freq) = 0;
		virtual float GetFrequency() const = 0;

		virtual void SetVolume(float volume) = 0;
		virtual float GetVolume() const = 0;
	};
	TOOLKIT_DECLARE_PTR(IOscillator);

}}

#endif
