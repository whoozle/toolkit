#ifndef TOOLKIT_IO_ISTREAM_H
#define TOOLKIT_IO_ISTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/io/ByteData.h>
#include <sys/types.h>
#include <memory>

TOOLKIT_SUBNS_BEGIN(io)

	enum struct SyncMode
	{
		Everything,
		Data
	};

	enum struct SeekMode
	{
		Begin,
		Current,
		End
	};

	struct IStream
	{
		virtual ~IStream() {}

		virtual void Sync(SyncMode mode) = 0;
		virtual off_t Seek(s64 offset, SeekMode mode = SeekMode::Begin) = 0;
		virtual off_t Tell() = 0;

		virtual size_t Write(ConstByteData data) = 0;
		virtual size_t Read(ByteData) = 0;
	};
	DECLARE_PTR(IStream);

	struct IBufferedStream : public IStream
	{
		virtual void Flush() = 0;
	};
	DECLARE_PTR(IBufferedStream);

TOOLKIT_SUBNS_END

#endif

