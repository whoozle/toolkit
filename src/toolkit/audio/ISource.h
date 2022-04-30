#ifndef TOOLKIT_AUDIO_ISOURCE_H
#define TOOLKIT_AUDIO_ISOURCE_H

#include <toolkit/audio/AudioFormat.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/core/types.h>
#include <memory>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	struct ISource
	{
		virtual ~ISource() = default;

		virtual void Generate(SampleFormat format, uint sampleRate, Buffer buffer) = 0;
	};
	TOOLKIT_DECLARE_PTR(ISource);


	template<typename OscillatorType>
	void GenerateBuffer(OscillatorType & osc, SampleFormat format, uint sampleRate, Buffer buffer)
	{
		switch(format)
		{
			case SampleFormat::S8:
				osc.template GenerateImpl<OscillatorType, Format<SampleFormat::S8>>(sampleRate, reinterpret_cast<s8*>(buffer.data()), buffer.size());
				break;
			case SampleFormat::S16:
				osc.template GenerateImpl<OscillatorType, Format<SampleFormat::S16>>(sampleRate, reinterpret_cast<s16*>(buffer.data()), buffer.size() / sizeof(s16));
				break;
			case SampleFormat::Float32:
				osc.template GenerateImpl<OscillatorType, Format<SampleFormat::Float32>>(sampleRate, reinterpret_cast<float*>(buffer.data()), buffer.size() / sizeof(float));
				break;
			default:
				throw Exception("unsupported sample format");
		}
	}


}}

#endif
