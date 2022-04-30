#ifndef TOOLKIT_AUDIO_TRIANGLE_OSCILLATOR_H
#define TOOLKIT_AUDIO_TRIANGLE_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class TriangleOscillator : public BaseOscillator
	{
	protected:
		float	_freq;
		float	_volume;
		float	_t;

	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{
			float m = fmodf(t + 0.75f, 1);
			float a = fabs(m * 4 - 2);
			return a - 1;
		}

		float Next(uint sampleRate)
		{ return Func(BaseOscillator::Next(sampleRate, 1)); }

		void Generate(SampleFormat format, uint sampleRate, Buffer buffer)
		{ GenerateBuffer(*this, format, sampleRate, buffer); }
	};

}}

#endif
