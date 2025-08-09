#ifndef TOOLKIT_AUDIO_PCM_SOURCE_H
#define TOOLKIT_AUDIO_PCM_SOURCE_H

#include <toolkit/audio/ISource.h>
#include <toolkit/io/IStream.h>

namespace TOOLKIT_NS { namespace audio
{
	class PCMSource : public ISourceWithBaseFrequency
	{
		std::vector<float> 	_samples;
		unsigned			_sampleRate;
		float				_baseFreq;
		float				_freq;
		size_t				_pos = 0;

	public:
		PCMSource(std::vector<float> samples, unsigned sampleRate, float baseFreq = 440.0f);

		void SetFrequency(float freq) override
		{ _freq = freq; }
		float GetFrequency() const override
		{ return _freq; }

		void SetBaseFrequency(float baseFreq)
		{ _baseFreq = baseFreq; }
		float GetBaseFrequency() const
		{ return _baseFreq; }

		void Get(float dt, FloatBuffer buffer) override;

		void Restart()
		{ _pos = 0; }

		static std::vector<float> Resample(ConstFloatBuffer buf, unsigned srcRate, unsigned dstRate);

		static PCMSource Read(io::ISkippableInputStream & stream, unsigned sampleRate, float baseFreq = 440.0f);
		static PCMSource Read(const std::string &path, unsigned sampleRate, float baseFreq = 440.0f);

	private:
		static std::vector<float> Downsample(ConstFloatBuffer buf, unsigned srcRate, unsigned dstRate);
		static std::vector<float> Upsample(ConstFloatBuffer buf, unsigned srcRate, unsigned dstRate);
	};
}}

#endif
