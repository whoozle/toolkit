#include <toolkit/audio/PCMSource.h>
#include <toolkit/audio/MDCT.h>
#include <toolkit/audio/WaveFile.h>
#include <toolkit/io/File.h>
#include <toolkit/log/Logger.h>
#include <array>
#include <cmath>

namespace TOOLKIT_NS { namespace audio
{
	namespace { log::Logger Log("pcm-source"); }

	PCMSource::PCMSource(std::vector<float> samples, unsigned sampleRate, float baseFreq):
		_samples(std::move(samples)), _sampleRate(sampleRate), _baseFreq(baseFreq), _freq(baseFreq)
	{ }

	void PCMSource::Regenerate()
	{
		auto pitch = _freq / _baseFreq;
		if (pitch == 1)
		{
			_pitchedSamples.clear();
			return;
		}

		Log.Debug() << "scale sample, " << _freq << "/" << _baseFreq << ", rel pitch: " << pitch;

		using MDCTType = MDCT<float, 4096>;
		static MDCTType mdct = { window::Sine<float>{} };

		size_t nSamples = _samples.size();
		_pitchedSamples.resize(nSamples);

		std::array<float, MDCTType::N> input = {};
		std::array<float, MDCTType::N2> overlap = {};

		for(size_t offset = 0; offset < nSamples; offset += MDCTType::N2)
		{
			for(unsigned i = 0; i < MDCTType::N; ++i)
			{
				size_t idx = offset + i;
				float v = idx < nSamples? _samples[idx]: 0;
				input[i] = v;
			}
			mdct.ApplyWindow(input.data());
			mdct.Forward(input.data());
			std::array<float, MDCTType::N2> shifted = {};
			if (pitch < 1)
			{
				for(unsigned i = 0; i != MDCTType::N2; ++i)
				{
					float b = pitch * i;
					float e = b + pitch;
					size_t bIdx = b;
					size_t eIdx = e;
					if (bIdx >= MDCTType::N2)
						break;
					auto v = input[i];
					if (bIdx != eIdx)
					{
						auto bPart = (eIdx - b) / pitch;
						auto v1 = bPart * v;
						shifted[bIdx] += v1;
						if (eIdx < MDCTType::N2)
							shifted[eIdx] += v - v1;
					}
					else
						shifted[bIdx] += v;
				}
			}
			else
			{
				auto ipitch = _baseFreq / _freq;
				for(unsigned i = 0; i != MDCTType::N2; ++i)
				{
					float b = i * ipitch;
					float e = b + ipitch;
					size_t bIdx = b;
					size_t eIdx = e;
					if (bIdx != eIdx)
					{
						size_t nextIdx = bIdx + 1;
						float bPart = (nextIdx - b) / ipitch;
						float v0 = input[bIdx];
						float v1 = eIdx < MDCTType::N2? input[eIdx]: v0;
						shifted[i] = v0 + bPart * (v1 - v0);
					}
					else
						shifted[i] = input[bIdx];
				}
			}
			std::copy(shifted.begin(), shifted.end(), input.begin());
			mdct.Inverse(input.data());
			mdct.ApplyWindow(input.data());
			for(unsigned i = 0; i < MDCTType::N2; ++i)
			{
				auto &dst = input[i];
				dst += overlap[i];
				size_t idx = offset + i;
				if (idx < nSamples)
					_pitchedSamples[idx] = dst;
				overlap[i] = input[i + MDCTType::N2];
			}
		}
	}

	void PCMSource::SetFrequency(float freq)
	{
		_freq = freq;
		Regenerate();
	}

	void PCMSource::SetBaseFrequency(float baseFreq)
	{
		_baseFreq = baseFreq;
		Regenerate();
	}

	void PCMSource::Get(float dt, FloatBuffer buffer)
	{
		auto &src = !_pitchedSamples.empty()? _pitchedSamples: _samples;
		auto size = src.size() - _pos;
		auto toCopy = std::min(buffer.size(), size);
		std::copy(src.begin() + _pos, src.begin() + _pos + toCopy, buffer.begin());
		std::fill(buffer.begin() + toCopy, buffer.end(), 0);
		_pos += toCopy;
	}

	std::vector<float> PCMSource::Resample(ConstFloatBuffer src, unsigned srcRate, unsigned dstRate)
	{
		Log.Debug() << "resampling " << src.size() << " samples from " << srcRate << " to " << dstRate;
		if (srcRate == dstRate)
		{
			std::vector<float> dst(src.begin(), src.end());
			return dst;
		}
		return (srcRate > dstRate)? Downsample(src, srcRate, dstRate): Upsample(src, srcRate, dstRate);
	}

	std::vector<float> PCMSource::Downsample(ConstFloatBuffer srcBuf, unsigned srcRate, unsigned dstRate)
	{
		float srcDt = dstRate * 1.0f / srcRate;
		std::vector<float> dst(std::ceil(srcBuf.size() * srcDt));
		Log.Debug() << "generating " << dst.size() << " samples...";
		const auto *src = srcBuf.data();
		for(unsigned srcIdx = 0, n = srcBuf.size(); srcIdx != n; ++srcIdx)
		{
			auto v = src[srcIdx] * srcDt;
			float b = srcIdx * srcDt;
			float e = b + srcDt;
			size_t bIdx = b;
			size_t eIdx = e;
			if (bIdx != eIdx)
			{
				float mid = std::floor(e);
				auto bPart = (mid - b) / srcDt;
				auto v1 = bPart * v;
				dst[bIdx] += v1;
				if (eIdx < n)
					dst[eIdx] += v - v1;
			}
			else
				dst[bIdx] += v;
		}
		return dst;
	}

	std::vector<float> PCMSource::Upsample(ConstFloatBuffer srcBuf, unsigned srcRate, unsigned dstRate)
	{
		auto dstDt = srcRate * 1.0f / dstRate;

		auto srcSize = srcBuf.size();
		const auto *src = srcBuf.data();
		std::vector<float> dst(std::ceil(1.0 * srcSize * dstRate / srcRate));
		size_t dstIdx = 0, dstSize = dst.size();
		while(dstIdx < dstSize)
		{
			auto b = dstIdx * dstDt;
			auto e = b + dstDt;
			size_t bIdx = b;
			size_t eIdx = e;
			float out;

			if (bIdx != eIdx)
			{
				out = src[bIdx];
				float mid = std::floor(e);
				float bPart = (mid - b) / dstDt;
				auto v0 = src[bIdx];
				auto v1 = eIdx < srcSize? src[eIdx]: v0;
				out = v0 + bPart * (v1 - v0);
			}
			else
			{
				out = src[bIdx];
			}

			dst[dstIdx++] = out;
		}

		return dst;
	}

	PCMSource PCMSource::Read(io::ISkippableInputStream & stream, unsigned sampleRate, float baseFreq)
	{
		auto wave = WaveFile::Read(stream);
		auto &spec = wave.GetSpecification();
		ASSERT(spec.Channels == 1, Exception, "Only mono samples are supported for now");
		ASSERT(spec.Format == SampleFormat::S16, Exception, "Only 16 bit samples are supported for now");

		Log.Debug() << "loaded sample, sample rate: " << spec.SampleRate << ", format: " << static_cast<int>(spec.Format) << ", channels: " << spec.Channels;
		std::vector<float> srcSamples;
		ConstBuffer wavSamples(wave.GetSamples());
		srcSamples.resize(wavSamples.size() / spec.SampleSize());
		Convert<Format<SampleFormat::Float32>, Format<SampleFormat::S16>>(srcSamples, wavSamples.Reinterpret<const s16>());

		auto dstSamples = Resample(srcSamples, spec.SampleRate, sampleRate);

		return {std::move(dstSamples), sampleRate, baseFreq};
	}

	PCMSource PCMSource::Read(const std::string &path, unsigned sampleRate, float baseFreq)
	{
		io::File file(path);
		return Read(file, sampleRate, baseFreq);
	}

}}

