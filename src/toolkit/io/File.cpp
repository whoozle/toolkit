#include <toolkit/io/File.h>
#include <toolkit/io/SystemException.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

TOOLKIT_SUBNS_BEGIN(io)

	File::File(const std::string &path, FileOpenMode mode): _fd(::open(path.c_str(), MapMode(mode), 0600))
	{
		if (_fd < 0)
			throw SystemException("open(\"" + path + "\")");
	}

	File::~File()
	{
		::close(_fd); //fixme: add error checking here
	}

	int File::MapMode(FileOpenMode mode)
	{
		switch(mode)
		{
			case FileOpenMode::Readonly:
				return O_RDONLY;
			case FileOpenMode::ReadWrite:
				return O_RDWR | O_CREAT;
			default:
				throw Exception("invalid open mode " + std::to_string((int)mode));
		}
	}

	int File::MapMode(SeekMode mode)
	{
		switch(mode)
		{
			case SeekMode::Begin:
				return SEEK_SET;
			case SeekMode::Current:
				return SEEK_CUR;
			case SeekMode::End:
				return SEEK_END;
			default:
				throw Exception("invalid seek mode " + std::to_string((int)mode));
		}
	}

	void File::Sync(SyncMode mode)
	{
		switch(mode)
		{
			case SyncMode::Data:
				if (::fdatasync(_fd) != 0)
					throw SystemException("fdatasync failed");
			case SyncMode::Everything:
				if (::fsync(_fd) != 0)
					throw SystemException("fsync failed");
		}
	}

	off_t File::Seek(off_t offset, SeekMode mode)
	{
		off_t r = ::lseek(_fd, offset, MapMode(mode));
		if (r < 0)
			throw SystemException("lseek failed");
		return r;
	}

	off_t File::Tell()
	{
		off_t offset = ::lseek(_fd, 0, SEEK_CUR);
		if (offset < 0)
			throw SystemException("lseek failed");
		return offset;
	}

	size_t File::Write(ConstByteData data)
	{
		ssize_t r = ::write(_fd, data.GetPointer(), data.GetSize());
		if (r < 0)
			throw SystemException("write failed");
		return r;
	}

	size_t File::Read(ByteData data)
	{
		ssize_t r = ::read(_fd, data.GetPointer(), data.GetSize());
		if (r < 0)
			throw SystemException("read failed");
		return r;
	}

	struct stat File::GetStatus()
	{
		struct stat buf;
		if (::fstat(_fd, &buf) != 0)
			throw SystemException("fstat failed");
		return buf;
	}

	void File::Allocate(int mode, off_t offset, off_t len)
	{
		if (fallocate(_fd, mode, offset, len) != 0)
			throw SystemException("fallocate failed");
	}

TOOLKIT_SUBNS_END
