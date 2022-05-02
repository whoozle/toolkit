#ifndef TOOLKIT_AUDIO_AMPLITUDE_MODULATION_H
#define TOOLKIT_AUDIO_AMPLITUDE_MODULATION_H

#include <toolkit/audio/IFilter.h>
#include <toolkit/audio/ISource.h>
#include <math.h>
#include <vector>

namespace TOOLKIT_NS { namespace audio
{

	class AmplitudeModulation final : public IFilter
	{
		ISourcePtr 				_mod;
		std::vector<float>		_buffer;

	public:
		AmplitudeModulation(const ISourcePtr & mod): _mod(mod)
		{ }

		void Process(float dt, FloatBuffer dstBuffer, ConstFloatBuffer srcBuffer) override
		{
			auto n = dstBuffer.size();
			if (_buffer.size() != n)
				_buffer.resize(n);
			_mod->Get(dt, _buffer);
			auto * dst = dstBuffer.data();
			auto * src = srcBuffer.data();
			auto * mod = _buffer.data();
			for(size_t i = 0; i != n; ++i)
			{
				*dst++ = *src++ * *mod++;
			}
		}
	};

}}

#endif
