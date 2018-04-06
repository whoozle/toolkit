#include <toolkit/io/File.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/log/Logger.h>
#include <fcntl.h>
#include <unistd.h>

TOOLKIT_NS_BEGIN
namespace io
{

	File::File(const std::string &path, FileOpenMode mode): _fd(::open(path.c_str(), MapMode(mode), 0600))
	{
		if (_fd < 0)
			throw SystemException("open(\"" + path + "\")");
	}

	File::~File()
	{
		auto r = close(_fd); //fixme: add error checking here
		if (r != 0)
		{
			auto error = SystemException::GetErrorMessage();
			static log::Logger log("file");
			log.Error() << "close failed: " << error;
		}
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
	{ SYSTEM_CALL_RETURN(lseek(_fd, offset, MapMode(mode))); }

	off_t File::Tell()
	{ SYSTEM_CALL_RETURN(lseek(_fd, 0, SEEK_CUR)); }

	size_t File::Write(ConstByteData data)
	{ SYSTEM_CALL_RETURN(write(_fd, data.GetPointer(), data.GetSize())); }

	size_t File::Read(ByteData data)
	{ SYSTEM_CALL_RETURN(read(_fd, data.GetPointer(), data.GetSize())); }

	struct stat File::GetStatus()
	{
		struct stat buf;
		SYSTEM_CALL(fstat(_fd, &buf));
		return buf;
	}

	struct statvfs File::GetVFSStatus()
	{
		struct statvfs buf;
		SYSTEM_CALL(fstatvfs(_fd, &buf));
		return buf;
	}


	void File::Allocate(int mode, off_t offset, off_t len)
	{
		SYSTEM_CALL(fallocate(_fd, mode, offset, len));
	}
	void File::Truncate(size_t size)
	{
		SYSTEM_CALL(ftruncate(_fd, size));
	}


}
TOOLKIT_NS_END
