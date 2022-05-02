#ifndef TOOLKIT_AUDIO_IOSCILLATOR_H
#define TOOLKIT_AUDIO_IOSCILLATOR_H

#include <toolkit/audio/ISource.h>

namespace TOOLKIT_NS { namespace audio
{

	struct IOscillator : public ISourceWithBaseFrequency
	{
		virtual ~IOscillator() = default;

		virtual void SetVolume(float volume) = 0;
		virtual float GetVolume() const = 0;

		virtual float GetPhase() const = 0;
		virtual void SetPhase(float phase) = 0;
	};
	TOOLKIT_DECLARE_PTR(IOscillator);

}}

#endif
