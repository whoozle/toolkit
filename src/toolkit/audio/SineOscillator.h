#ifndef TOOLKIT_AUDIO_SINE_OSCILLATOR_H
#define TOOLKIT_AUDIO_SINE_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class SineOscillator : public BaseOscillator
	{
	protected:
		float	_freq;
		float	_volume;
		float	_t;

	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{ return sin(t * M_PI * 2); }

		float Next(uint sampleRate)
		{ return Func(BaseOscillator::Next(sampleRate, 1)); }

		void Generate(SampleFormat format, uint sampleRate, Buffer buffer)
		{ GenerateImpl<SineOscillator>(format, sampleRate, buffer); }
	};

}}

#endif
