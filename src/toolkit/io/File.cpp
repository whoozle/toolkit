#include <toolkit/io/File.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/log/Logger.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <unistd.h>

namespace TOOLKIT_NS { namespace io
{

	File::File(const std::string &path, FileOpenMode mode): _fd(::open(path.c_str(), MapMode(mode), 0600))
	{
		ASSERT(_fd != -1, SystemException, "open(\"" + path + "\")");
	}

	FilePtr File::Clone()
	{
		int fd = dup(_fd);
		ASSERT(fd != -1, SystemException, "dup");
		return std::make_shared<File>(fd);
	}


	void File::Close()
	{
		if (_fd < 0)
			return;

		auto r = close(_fd);
		if (r != 0)
		{
			auto error = SystemException::GetErrorMessage();
			static log::Logger log("File");
			log.Error() << "close failed: " << error;
		}
		else
			_fd = -1;
	}

	File & File::operator = (File && o)
	{
		Close();
		_fd = o._fd;
		o._fd = -1;
		return *this;
	}

	File::~File()
	{ Close(); }

	bool File::Access(const std::string & path, FileOpenMode mode)
	{
		int r = access(path.c_str(), MapMode(mode));
		return r == 0;
	}

	void File::MakeDirectory(const std::string & path, mode_t mode)
	{ SYSTEM_CALL(mkdir(path.c_str(), mode)); }

	void File::Unlink(const std::string & path)
	{ SYSTEM_CALL(unlink(path.c_str())); }

	int File::GetFlags() const
	{ SYSTEM_CALL_RETURN(fcntl(_fd, F_GETFL, 0)); }

	void File::SetFlags(int flags)
	{ SYSTEM_CALL(fcntl(_fd, F_SETFL, flags)); }

	int File::MapMode(FileOpenMode mode)
	{
		switch(mode)
		{
			case FileOpenMode::Readonly:
				return O_RDONLY;
			case FileOpenMode::ReadWrite:
				return O_RDWR | O_CREAT;
			case FileOpenMode::Overwrite:
				return O_RDWR | O_CREAT | O_TRUNC;
			default:
				THROW(Exception, "invalid open mode " + std::to_string((int)mode));
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
				THROW(Exception, "invalid seek mode " + std::to_string((int)mode));
		}
	}

	void File::Sync(SyncMode mode)
	{
		switch(mode)
		{
			case SyncMode::Data:
				ASSERT(::fdatasync(_fd) == 0, SystemException, "fdatasync failed");
				break;
			case SyncMode::Everything:
				ASSERT(::fsync(_fd) == 0, SystemException, "fsync failed");
				break;
		}
	}

	void File::Sync(off64_t offset, off64_t size, unsigned flags)
	{
		SYSTEM_CALL(sync_file_range(_fd, offset, size, flags));
	}

	off_t File::Seek(off_t offset, SeekMode mode)
	{ SYSTEM_CALL_RETURN(lseek(_fd, offset, MapMode(mode))); }

	off_t File::Tell()
	{ SYSTEM_CALL_RETURN(lseek(_fd, 0, SEEK_CUR)); }

	size_t File::Write(ConstBuffer data)
	{ SYSTEM_CALL_RETURN(write(_fd, data.GetPointer(), data.GetSize())); }

	size_t File::Read(Buffer data)
	{ SYSTEM_CALL_RETURN(read(_fd, data.GetPointer(), data.GetSize())); }

	size_t File::Write(ConstBuffer data, off_t offset)
	{ SYSTEM_CALL_RETURN(pwrite(_fd, data.GetPointer(), data.GetSize(), offset)); }

	size_t File::Read(Buffer data, off_t offset)
	{ SYSTEM_CALL_RETURN(pread(_fd, data.GetPointer(), data.GetSize(), offset)); }

	struct stat File::GetStatus()
	{
		struct stat buf;
		SYSTEM_CALL(fstat(_fd, &buf));
		return buf;
	}

	struct stat File::GetStatus(const std::string & path, bool followSymlinks)
	{
		struct stat buf;
		if (followSymlinks)
			SYSTEM_CALL(stat(path.c_str(), &buf));
		else
			SYSTEM_CALL(lstat(path.c_str(), &buf));
		return buf;
	}

	void File::Symlink(const std::string & target, const std::string & linkPath)
	{ SYSTEM_CALL(symlink(target.c_str(), linkPath.c_str())); }

	struct statvfs File::GetVFSStatus()
	{
		struct statvfs buf;
		SYSTEM_CALL(fstatvfs(_fd, &buf));
		return buf;
	}

	void File::Allocate(int mode, off_t offset, off_t len)
	{ SYSTEM_CALL(fallocate(_fd, mode, offset, len)); }

	void File::Truncate(size_t size)
	{ SYSTEM_CALL(ftruncate(_fd, size)); }

	void File::Truncate(const std::string & path, off_t size)
	{ SYSTEM_CALL(truncate(path.c_str(), size)); }

	std::string File::ReadLink(const std::string & path)
	{
		char buf[PATH_MAX];
		auto size = readlink(path.c_str(), buf, sizeof(buf));
		ASSERT(size >= 0, io::SystemException, "readlink");

		return std::string(buf, buf + size);
	}

	void File::CreatePipe(int & readFd, int & writeFd, int flags)
	{
		int fd[2];
		SYSTEM_CALL(pipe2(fd, flags));
		readFd = fd[0];
		writeFd = fd[1];
	}

	std::string File::MakeTempDirectory(const std::string &pattern)
	{
		std::vector<char> tempName(pattern.size() + 1);
		strcpy(tempName.data(), pattern.c_str());
		ASSERT(mkdtemp(tempName.data()), io::SystemException, "mkdtemp");
		return std::string(tempName.data());
	}

	void File::Rename(const std::string &oldName, const std::string &newName)
	{ SYSTEM_CALL(rename(oldName.c_str(), newName.c_str())); }

}}

