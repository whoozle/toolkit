#ifndef TOOLKIT_IO_ISTREAM_H
#define TOOLKIT_IO_ISTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/io/ByteData.h>
#include <sys/types.h>
#include <memory>

TOOLKIT_NS_BEGIN
namespace io
{

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

	struct ISeekable
	{
		virtual ~ISeekable() = default;

		virtual off_t Seek(s64 offset, SeekMode mode = SeekMode::Begin) = 0;
		virtual off_t Tell() = 0;
	};
	DECLARE_PTR(ISeekable);

	struct IOutputStream
	{
		virtual ~IOutputStream() = default;

		virtual size_t Write(ConstByteData data) = 0;
	};
	DECLARE_PTR(IOutputStream);

	struct IInputStream
	{
		virtual ~IInputStream() = default;

		virtual size_t Read(ByteData) = 0;
	};
	DECLARE_PTR(IInputStream);

	struct IStream :
		public virtual IOutputStream,
		public virtual IInputStream,
		public virtual ISeekable
	{
		virtual void Sync(SyncMode mode) = 0;
		virtual void Truncate(size_t size) = 0;
	};
	DECLARE_PTR(IStream);

	struct IBufferedStream : public IStream
	{
		virtual void Flush() = 0;
	};
	DECLARE_PTR(IBufferedStream);

}
TOOLKIT_NS_END

#endif

