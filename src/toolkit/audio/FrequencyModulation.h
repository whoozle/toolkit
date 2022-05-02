#ifndef TOOLKIT_AUDIO_FREQUENCY_MODULATION_H
#define TOOLKIT_AUDIO_FREQUENCY_MODULATION_H

#include <toolkit/audio/IFilter.h>
#include <toolkit/audio/ISource.h>
#include <math.h>
#include <vector>

namespace TOOLKIT_NS { namespace audio
{

	class FrequencyModulation final : public ISource
	{
		ISourceWithBaseFrequencyPtr 	_source;
		ISourcePtr						_mod;
		std::vector<float>				_buffer;
		float							_minFreq;
		float							_maxFreq;
		float							_freqRange;

	public:
		FrequencyModulation(const ISourceWithBaseFrequencyPtr & source, const ISourcePtr & mod, float minFreq, float maxFreq):
			_source(source), _mod(mod), _minFreq(minFreq), _maxFreq(maxFreq), _freqRange(maxFreq - minFreq)
		{ }

		void Get(float dt, FloatBuffer dstBuffer) override
		{
			auto n = dstBuffer.size();
			if (_buffer.size() != n)
				_buffer.resize(n);

			_mod->Get(dt, _buffer);

			auto * dst = dstBuffer.data();
			for(size_t i = 0; i != n; ++i)
			{
				float freq = (_buffer[i] + 1) * _freqRange / 2 + _minFreq;
				_source->SetFrequency(freq);
				float sample;
				FloatBuffer src(&sample, 1);
				_source->Get(dt, src);
				*dst++ = src[0];
			}
		}
	};

}}

#endif
