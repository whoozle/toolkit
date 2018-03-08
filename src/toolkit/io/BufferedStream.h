#ifndef TOOLKIT_IO_BUFFEREDSTREAM_H
#define TOOLKIT_IO_BUFFEREDSTREAM_H

#include <toolkit/io/IStream.h>
#include <toolkit/core/Noncopyable.h>
#include <vector>

TOOLKIT_SUBNS_BEGIN(io)

	class BufferedStream : public IBufferedStream, Noncopyable
	{
		IStreamPtr		_stream;
		std::vector<u8> _buffer;
		size_t			_writeBufferOffset;

	public:
		BufferedStream(IStreamPtr stream, size_t bufferSize): _stream(stream), _buffer(bufferSize), _writeBufferOffset(0)
		{ }

		~BufferedStream()
		{ Flush(); }

		void Flush() override
		{
			if (_writeBufferOffset)
			{
				if (_stream->Write(ConstByteData(_buffer.data(), _writeBufferOffset)) != _writeBufferOffset)
					throw Exception("short write");
				_writeBufferOffset = 0;
			}
		}

		size_t Read(ByteData data) override
		{
			Flush();
			return _stream->Read(data);
		}

		off_t Seek(s64 offset, SeekMode mode) override
		{
			Flush();
			return _stream->Seek(offset, mode);
		}

		void Sync(SyncMode mode) override
		{
			Flush();
			_stream->Sync(mode);
		}

		off_t Tell() override
		{
			off_t streamOffset = _stream->Tell();
			size_t remainingData = 0; //readbuffer offset
			return streamOffset - remainingData + _writeBufferOffset;
		}

		size_t Write(ConstByteData data) override
		{
			auto srcData = data.GetPointer();
			auto remainingSrcData = data.GetSize();
			while(remainingSrcData != 0)
			{
				auto r = std::min(remainingSrcData, _buffer.size() - _writeBufferOffset);
				std::copy(srcData, srcData + r, _buffer.data() + _writeBufferOffset);
				srcData += r;
				_writeBufferOffset += r;
				remainingSrcData -= r;
				if (_writeBufferOffset == _buffer.size())
					Flush();
			}
			return data.GetSize();
		}
	};

TOOLKIT_SUBNS_END

#endif
