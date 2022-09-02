#ifndef TOOLKIT_AUDIO_WAVE_FILE_H
#define TOOLKIT_AUDIO_WAVE_FILE_H

#include <toolkit/audio/AudioFormat.h>
#include <toolkit/core/ByteArray.h>
#include <toolkit/io/IStream.h>

namespace TOOLKIT_NS { namespace audio
{

	class WaveFile
	{
		Specification 	_spec;
		ByteArray		_samples;

	public:
		WaveFile(Specification spec, ByteArray samples):
			_spec(std::move(spec)), _samples(std::move(samples))
		{ }

		const Specification & GetSpecification() const
		{ return _spec; }

		const ByteArray & GetSamples() const
		{ return _samples; }

		static WaveFile Read(io::ISkippableInputStream & stream);

		void Write(io::IOutputStream & stream);
	};

}}

#endif
