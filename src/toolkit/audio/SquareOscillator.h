#ifndef TOOLKIT_AUDIO_SQUARE_OSCILLATOR_H
#define TOOLKIT_AUDIO_SQUARE_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class SquareOscillator : public BaseOscillator
	{
	protected:
		float	_freq;
		float	_volume;
		float	_t;

	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{ return t < 0.5f? 1.0f: -1.0f; }

		float Next(uint sampleRate)
		{ return Func(BaseOscillator::Next(sampleRate, 1)); }

		void Generate(SampleFormat format, uint sampleRate, Buffer buffer)
		{ GenerateImpl<SquareOscillator>(format, sampleRate, buffer); }
	};

}}

#endif
