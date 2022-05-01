#ifndef TOOLKIT_AUDIO_SAWTOOTH_OSCILLATOR_H
#define TOOLKIT_AUDIO_SAWTOOTH_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class SawtoothOscillator final : public BaseOscillator
	{
	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{
			auto x = fmodf(t, 1.0f);
			x = 2 * (x - floorf(x + 0.5f));
			return x;
		}

		float Next(float dt) override
		{ return Func(Tick(dt, 1)); }
	};

}}

#endif
