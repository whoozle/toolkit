#include <toolkit/audio/PCMSource.h>
#include <toolkit/audio/WaveFile.h>
#include <toolkit/io/File.h>
#include <toolkit/log/Logger.h>
#include <cmath>

namespace TOOLKIT_NS { namespace audio
{
	namespace { log::Logger Log("pcm-source"); }

	PCMSource::PCMSource(std::vector<float> samples, unsigned sampleRate, float baseFreq):
		_samples(std::move(samples)), _sampleRate(sampleRate), _baseFreq(baseFreq), _freq(baseFreq)
	{ }

	void PCMSource::Get(float dt, FloatBuffer buffer)
	{
		auto size = _samples.size() - _pos;
		auto toCopy = std::min(buffer.size(), size);
		std::copy(_samples.begin() + _pos, _samples.begin() + _pos + toCopy, buffer.begin());
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
				out = src[bIdx] * bPart;
				if (eIdx < srcSize)
					out += src[eIdx] * (1 - bPart);
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

