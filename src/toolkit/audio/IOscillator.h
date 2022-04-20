#ifndef TOOLKIT_AUDIO_IOSCILLATOR_H
#define TOOLKIT_AUDIO_IOSCILLATOR_H

#include <toolkit/audio/AudioFormat.h>
#include <toolkit/core/Buffer.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	struct IOscillator
	{
		virtual ~IOscillator() = default;

		virtual void SetFrequency(float freq) = 0;
		virtual float GetFrequency() const = 0;

		virtual void SetVolume(float volume) = 0;
		virtual float GetVolume() const = 0;

		virtual void Generate(SampleFormat format, uint sampleRate, Buffer buffer) = 0;
	};

}}

#endif
