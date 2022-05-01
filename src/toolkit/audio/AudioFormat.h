#ifndef TOOLKIT_AUDIO_AUDIO_FORMAT_H
#define TOOLKIT_AUDIO_AUDIO_FORMAT_H

#include <toolkit/core/types.h>
#include <toolkit/core/type_traits.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/core/Exception.h>
#include <limits>
#include <assert.h>

namespace TOOLKIT_NS { namespace audio
{
	enum struct SampleFormat
	{
		S8, U8,
		S16, U16,
		//FIXME: add 24-bit integer types, spec?
		Float32
	};

	struct Specification
	{
		SampleFormat	Format;
		uint			Channels;
		uint			SampleRate;

		uint SampleSize() const
		{
			switch(Format)
			{
			case SampleFormat::S8:
			case SampleFormat::U8:
				return 1;
			case SampleFormat::S16:
			case SampleFormat::U16:
				return 2;
			case SampleFormat::Float32:
				return 4;
			default:
				throw Exception("SampleSize: Invalid audio format type");
			}
		}

		s32 Min() const
		{
			switch(Format)
			{
			case SampleFormat::S8:	return std::numeric_limits<s8>::min();
			case SampleFormat::U8:	return std::numeric_limits<u8>::min();
			case SampleFormat::S16:	return std::numeric_limits<s16>::min();
			case SampleFormat::U16:	return std::numeric_limits<u16>::min();
			case SampleFormat::Float32:	return -1;
			default: throw Exception("Min: Invalid audio format type");
			}
		}

		s32 Max() const
		{
			switch(Format)
			{
			case SampleFormat::S8:	return std::numeric_limits<s8>::max();
			case SampleFormat::U8:	return std::numeric_limits<u8>::max();
			case SampleFormat::S16:	return std::numeric_limits<s16>::max();
			case SampleFormat::U16:	return std::numeric_limits<u16>::max();
			case SampleFormat::Float32:	return 1;
			default: throw Exception("Min: Invalid audio format type");
			}
		}
	};

	namespace impl
	{
		template<typename T, T ZeroValue>
		struct Format
		{
			using Type = T;
			using DoubleType = typename TOOLKIT_NS::DoubleSize<T>::Type;
			static auto Max() { return std::numeric_limits<T>::max(); }
			static auto Min() { return std::numeric_limits<T>::min(); }
			static auto Zero() { return ZeroValue; }
			static auto Range() { return Max() - Zero(); }
		};

		template<typename T, typename D>
		struct FloatFormat
		{
			using Type = T;
			using DoubleType = D;
			static Type Max() { return 1; }
			static Type Min() { return -1; }
			static Type Zero() { return 0; }
			static Type Range() { return Max() - Zero(); }
		};
	}

	template<SampleFormat>
	struct Format;

	template<> struct Format<SampleFormat::S8>	: impl::Format<s8, 0> {};
	template<> struct Format<SampleFormat::S16>	: impl::Format<s16, 0> {};

	template<> struct Format<SampleFormat::U8>	: impl::Format<u8, 1 << 7> {};
	template<> struct Format<SampleFormat::U16>	: impl::Format<u16, 1 << 15> {};

	template<> struct Format<SampleFormat::Float32>	: impl::FloatFormat<float, double> {};

	template<typename DestinationFormat, typename SourceFormat>
	void Convert(Buffer dstBuffer, ConstBuffer srcBuffer)
	{
		using DstSampleType = typename DestinationFormat::Type;
		using SrcSampleType = typename SourceFormat::Type;

		size_t dstSize = dstBuffer.GetSize() / sizeof(DstSampleType);
		size_t srcSize = srcBuffer.GetSize() / sizeof(SrcSampleType);
		assert(dstSize == srcSize);
		DstSampleType * dst = reinterpret_cast<DstSampleType *>(dstBuffer.data());
		const SrcSampleType * src = reinterpret_cast<const SrcSampleType *>(srcBuffer.data());
		while(dstSize--)
		{
			float sample = (*src++ - SourceFormat::Zero()) / float(SourceFormat::Max());
			*dst++ = (sample * DestinationFormat::Max()) + DestinationFormat::Zero();
		}
	}

}}

#endif
