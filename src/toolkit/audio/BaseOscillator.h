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

	protected:
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

		template<typename OscillatorType>
		void GenerateImpl(SampleFormat format, uint sampleRate, Buffer buffer)
		{
			switch(format)
			{
				case SampleFormat::S8:
					GenerateImpl<OscillatorType, Format<SampleFormat::S8>>(sampleRate, reinterpret_cast<s8*>(buffer.data()), buffer.size());
					break;
				case SampleFormat::S16:
					GenerateImpl<OscillatorType, Format<SampleFormat::S16>>(sampleRate, reinterpret_cast<s16*>(buffer.data()), buffer.size() / 2);
					break;
				default:
					throw Exception("unsupported sample format");
			}
		}
	};

}}

#endif
