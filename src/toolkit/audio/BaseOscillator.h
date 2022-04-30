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

		float Next(uint sampleRate)
		{
			auto t = _t;
			_t += _freq / sampleRate;
			return t;
		}

		float Next(uint sampleRate, float period)
		{
			auto t = _t;
			_t += _freq / sampleRate;
			_t = fmod(_t, period);
			return t;
		}

		template<typename OscillatorType, typename FormatType>
		void GenerateImpl(uint sampleRate, typename FormatType::Type * buffer, size_t sampleCount)
		{
			auto zero = FormatType::Zero();
			auto range = FormatType::Range();
			while(sampleCount--)
				*buffer++ = static_cast<OscillatorType *>(this)->Next(sampleRate) * _volume * range + zero;
		}
	};

}}

#endif
