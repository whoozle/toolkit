#ifndef TOOLKIT_AUDIO_ADSR_ENVELOPE_H
#define TOOLKIT_AUDIO_ADSR_ENVELOPE_H

#include <toolkit/audio/ISource.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class ADSREnvelope : public ISource
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

		float Next(uint sampleRate)
		{
			if (!_active)
				return 0;

			_t += 1.0f / sampleRate;
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

		template<typename OscillatorType, typename FormatType>
		void GenerateImpl(uint sampleRate, typename FormatType::Type * buffer, size_t sampleCount)
		{
			while(sampleCount--)
				*buffer++ *= static_cast<OscillatorType *>(this)->Next(sampleRate);
		}

		void Generate(SampleFormat format, uint sampleRate, Buffer buffer) override
		{ GenerateBuffer(*this, format, sampleRate, buffer); }
	};

}}

#endif
