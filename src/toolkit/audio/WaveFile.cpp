#include <toolkit/audio/WaveFile.h>
#include <toolkit/io/MemoryOutputStream.h>
#include <toolkit/io/DataStream.h>
#include <toolkit/audio/AudioFormat.h>

namespace TOOLKIT_NS { namespace audio
{
	namespace
	{
		size_t BlockSize(size_t dataSize)
		{ return dataSize + 8; }

		static constexpr u32 RIFF	= 0x46464952;
		static constexpr u32 WAVE	= 0x45564157;
		static constexpr u32 FMT	= 0x20746d66;
		static constexpr u32 DATA	= 0x61746164;
	}

	WaveFile WaveFile::Read(io::ISkippableInputStream & stream)
	{
		io::LittleEndianDataInputStream is(stream);
		if (is.ReadU32() != RIFF)
			THROW(Exception, "Invalid RIFF file signature");

		auto size = is.ReadU32();
		auto container = is.ReadU32();
		if (container != WAVE)
			THROW(Exception, "Expected WAVE format");
		size_t offset = 4;

		audio::Specification spec;
		ByteArray data;

		while(offset < size)
		{
			auto tag = is.ReadU32();
			auto tagSize = is.ReadU32();
			offset += 8 + tagSize;
			switch(tag)
			{
			case FMT:
				ASSERT(is.ReadU16() == 1, Exception, "Only format v1 is supported.");
				ASSERT(tagSize == 16, Exception, "Format v1 should have 16 bytes in length.");
				spec.Channels = is.ReadU16();
				spec.SampleRate = is.ReadU32();
				stream.Skip(6);
				{
					u16 bpp = is.ReadU16();
					switch(bpp)
					{
					case 8:		spec.Format = SampleFormat::S8; break;
					case 16: 	spec.Format = SampleFormat::S16; break;
					case 32:	spec.Format = SampleFormat::Float32; break;
					default: THROW(Exception, "Invalid bits per sample value");
					}
				}
				break;
			case DATA:
				data.Resize(tagSize);
				stream.Read(data);
				break;
			default:
				stream.Skip(tagSize);
			}
		}
		return WaveFile(spec, std::move(data));
	}

	void WaveFile::Write(io::IOutputStream & stream)
	{
		io::LittleEndianDataOutputStream os(stream);
		os.WriteU32(RIFF); //RIFF
		auto data_size = 4 + BlockSize(_samples.size()) + BlockSize(0x10) /*fmt*/;
		os.WriteU32(data_size);
		os.WriteU32(WAVE); //WAVE
		os.WriteU32(FMT); //fmt
		os.WriteU32(16); //fmt size

		os.WriteU16(1); //format
		os.WriteU16(_spec.Channels);
		os.WriteU32(_spec.SampleRate);
		os.WriteU32(_spec.SampleRate * _spec.Channels * _spec.SampleSize());
		os.WriteU16(_spec.Channels * _spec.SampleSize());
		os.WriteU16(_spec.SampleSize() * 8);

		os.WriteU32(DATA); //data
		os.WriteU32(_samples.size());
		os.Write(ConstBuffer(_samples));
	}

}}
