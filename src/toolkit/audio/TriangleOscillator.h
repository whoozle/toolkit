#ifndef TOOLKIT_AUDIO_TRIANGLE_OSCILLATOR_H
#define TOOLKIT_AUDIO_TRIANGLE_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class TriangleOscillator final : public BaseOscillator
	{
	public:
		using BaseOscillator::BaseOscillator;

		static float Func(float t)
		{
			float m = fmodf(t + 0.75f, 1);
			float a = fabs(m * 4 - 2);
			return a - 1;
		}

		float Next(float dt) override
		{ return Func(Tick(dt, 1)); }
	};

}}

#endif
