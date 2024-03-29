#ifndef TOOLKIT_AUDIO_SAWTOOTH_OSCILLATOR_H
#define TOOLKIT_AUDIO_SAWTOOTH_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class SawtoothOscillator final : public BaseOscillator
	{
		bool _inverted;

	public:
		SawtoothOscillator(float freq, float volume = BaseOscillator::DefaultVolume, bool inverted = false):
			BaseOscillator(freq, volume), _inverted(inverted)
		{ }

		float Func(float t)
		{
			auto x = t / 2;
			x = 2 * (x - floorf(x + 0.5f));
			return _inverted? 1 - x: x;
		}

		float Next(float dt) override
		{ return Func(Tick(dt, 1)); }
	};

}}

#endif
