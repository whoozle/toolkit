#ifndef TOOLKIT_AUDIO_BASE_OSCILLATOR_H
#define TOOLKIT_AUDIO_BASE_OSCILLATOR_H

#include <toolkit/audio/AudioFormat.h>
#include <toolkit/audio/IOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class BaseOscillator : public IOscillator
	{
	protected:
		float	_freq;
		float	_volume;
		float	_t;

	public:
		BaseOscillator(float freq, float volume = 0.75f): _freq(freq), _volume(volume), _t(0)
		{ }

		void SetFrequency(float freq) override
		{
			if (freq)
				_t *= _freq / freq;
			_freq = freq;
		}
		float GetFrequency() const override
		{ return _freq; }

		void SetVolume(float volume) override
		{ _volume = volume; }

		float GetVolume() const override
		{ return _volume; }

		float Tick(float dt)
		{
			auto t = _t;
			_t += _freq * dt;
			return t;
		}

		float Tick(float dt, float period)
		{
			auto t = _t;
			_t += _freq * dt;
			_t = fmod(_t, period);
			return t;
		}

		virtual float Next(float dt) = 0;

		void Get(float dt, FloatBuffer buffer) override
		{
			auto n = buffer.size();
			auto* dst = buffer.data();
			while(n--)
				*dst++ = Next(dt) * _volume;
		}
	};

}}

#endif
