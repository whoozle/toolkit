#ifndef TOOLKIT_AUDIO_ADSR_ENVELOPE_H
#define TOOLKIT_AUDIO_ADSR_ENVELOPE_H

#include <toolkit/audio/IFilter.h>
#include <math.h>
#include <assert.h>

namespace TOOLKIT_NS { namespace audio
{

	class ADSREnvelope final : public IFilter
	{
		float		_attack;
		float		_decay;
		float		_sustain;
		float		_release;
		float		_length;
		float		_t;
		float		_active;

	public:
		ADSREnvelope(float attack = 0.1f, float decay = 0.3f, float sustainVolume = 0.8f, float release = 0.3f, float length = 0.0f):
			_attack(attack),
			_decay(decay),
			_sustain(sustainVolume),
			_release(release),
			_length(length),
			_t(-1),
			_active(false)
		{}

		void start()
		{
			_t = 0;
			_active = true;
		}

		float Next(float dt)
		{
			if (!_active)
				return 0;

			_t += dt;
			auto t = _t;
			if (t < _attack)
				return t / _attack;

			t -= _attack;
			if (t < _decay)
				return 1 - t / _decay * (1 - _sustain);

			t -= _decay;
			if (t < _length)
				return _sustain;

			t -= _length;
			if (t < _release)
				return (1 - t / _release) * _sustain;

			_active = false;
			return 0;
		}

		void Process(float dt, FloatBuffer dstBuffer, ConstFloatBuffer srcBuffer) override
		{
			assert(dstBuffer.size() == srcBuffer.size());
			size_t n = srcBuffer.size();
			auto * dst = dstBuffer.data();
			auto * src = srcBuffer.data();
			while(n--)
			{
				*dst = Next(dt) * *src++;
				printf("%g\n", *dst);
				++dst;
			}
		}
	};

}}

#endif
