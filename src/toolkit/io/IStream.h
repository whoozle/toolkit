#ifndef TOOLKIT_IO_ISTREAM_H
#define TOOLKIT_IO_ISTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/core/Buffer.h>
#include <sys/types.h>
#include <memory>

namespace TOOLKIT_NS { namespace io
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

		virtual off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) = 0;
		virtual off_t Tell() = 0;
	};
	DECLARE_PTR(ISeekable);

	struct IOutputStream
	{
		virtual ~IOutputStream() = default;

		virtual size_t Write(ConstBuffer data) = 0;
	};
	DECLARE_PTR(IOutputStream);

	struct IInputStream
	{
		virtual ~IInputStream() = default;

		virtual size_t Read(Buffer) = 0;
	};
	DECLARE_PTR(IInputStream);

	struct IBidirectionalStream :
		public virtual IInputStream,
		public virtual IOutputStream
	{ };
	DECLARE_PTR(IBidirectionalStream);

	struct ISeekableInputStream :
			public virtual IInputStream,
			public virtual ISeekable
	{ };
	DECLARE_PTR(ISeekableInputStream);

	struct ISeekableOutputStream :
			public virtual IOutputStream,
			public virtual ISeekable
	{ };
	DECLARE_PTR(ISeekableOutputStream);

	struct IStorage :
		public virtual ISeekableInputStream,
		public virtual ISeekableOutputStream
	{
		virtual void Sync(SyncMode mode) = 0;
		virtual void Truncate(size_t size) = 0;
	};
	DECLARE_PTR(IStorage);

	struct IBufferedStream : public IStorage
	{
		virtual void Flush() = 0;
	};
	DECLARE_PTR(IBufferedStream);

}}


#endif

