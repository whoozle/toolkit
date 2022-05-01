#ifndef TOOLKIT_AUDIO_SINE_OSCILLATOR_H
#define TOOLKIT_AUDIO_SINE_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class SineOscillator final : public BaseOscillator
	{
	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{ return sin(t * M_PI * 2); }

		float Next(float dt) override
		{ return Func(Tick(dt, 1)); }
	};

}}

#endif
