#ifndef TOOLKIT_AUDIO_SQUARE_OSCILLATOR_H
#define TOOLKIT_AUDIO_SQUARE_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class SquareOscillator final : public BaseOscillator
	{
	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{ return t < 0.5f? 1.0f: -1.0f; }

		float Next(float dt) override
		{ return Func(Tick(dt, 1)); }
	};

}}

#endif
