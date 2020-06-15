#ifndef TOOLKIT_IO_FILE_H
#define TOOLKIT_IO_FILE_H

#include <toolkit/io/IStream.h>
#include <toolkit/io/IPollable.h>
#include <toolkit/io/MemoryMapping.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/core/Noncopyable.h>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/ioctl.h>

namespace TOOLKIT_NS { namespace io
{

	enum struct FileOpenMode
	{
		Readonly,
		ReadWrite,
		Overwrite
	};

	class File :
		public virtual IStorage,
		public virtual IPollable,
		public Noncopyable
	{
	protected:
		int _fd;

	public:
		explicit File(int fd): _fd(fd) { }
		File(const std::string &path, FileOpenMode mode = FileOpenMode::Readonly);
		File(File && o) : _fd(o._fd)
		{ o._fd = -1; }
		File & operator = (File && o);
		~File();

		int GetFileDescriptor() const override
		{ return _fd; }

		int GetFlags() const;
		void SetFlags(int flags);

		void Sync(SyncMode mode) override;
		void Sync(off64_t offset, off64_t size, unsigned flags);
		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override;
		off_t Tell() override;

		size_t Write(ConstBuffer data) override;
		size_t Read(Buffer) override;

		size_t Write(ConstBuffer data, off_t offset);
		size_t Read(Buffer, off_t offset);

		void Allocate(int mode, off_t offset, off_t len);
		void Truncate(size_t size) override;
		struct stat GetStatus();
		struct statvfs GetVFSStatus();

		MemoryMappingPtr Map(off_t offset, size_t size, int prot, int flags, void *addr = nullptr)
		{ return std::make_shared<MemoryMapping>(addr, size, prot, flags, _fd, offset); }

		template<typename ... Args>
		int Ioctl(Args && ... args)
		{
			int r = ioctl(_fd, args ... );
			if (r < 0)
				throw SystemException("ioctl");
			return r;
		}

		static std::string ReadLink(const std::string & path);
		static void CreatePipe(int & readFd, int & writeFd, int flags = 0);

	private:
		void Close();
		static int MapMode(FileOpenMode mode);
		static int MapMode(SeekMode mode);
	};
	TOOLKIT_DECLARE_PTR(File);

}}


#endif
