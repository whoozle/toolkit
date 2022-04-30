#ifndef TOOLKIT_AUDIO_SAWTOOTH_OSCILLATOR_H
#define TOOLKIT_AUDIO_SAWTOOTH_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class SawtoothOscillator : public BaseOscillator
	{
	protected:
		float	_freq;
		float	_volume;
		float	_t;

	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{
			auto x = fmodf(t, 1.0f);
			x = 2 * (x - floorf(x + 0.5f));
			return x;
		}

		float Next(uint sampleRate)
		{ return Func(BaseOscillator::Next(sampleRate, 1)); }

		void Generate(SampleFormat format, uint sampleRate, Buffer buffer)
		{ GenerateBuffer(*this, format, sampleRate, buffer); }
	};

}}

#endif
