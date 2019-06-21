#ifndef TOOLKIT_IO_FILE_H
#define TOOLKIT_IO_FILE_H

#include <toolkit/io/IStream.h>
#include <toolkit/io/IPollable.h>
#include <toolkit/io/MemoryMapping.h>
#include <toolkit/io/SystemException.h>
#include <string>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/ioctl.h>

namespace TOOLKIT_NS { namespace io
{

	enum struct FileOpenMode
	{
		Readonly,
		ReadWrite
	};

	class File :
		public virtual IStorage,
		public virtual IPollable
	{
	protected:
		int _fd;

	public:
		explicit File(int fd): _fd(fd) { }
		File(const std::string &path, FileOpenMode mode = FileOpenMode::Readonly);
		~File();

		int GetFileDescriptor() const override
		{ return _fd; }

		void Sync(SyncMode mode) override;
		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override;
		off_t Tell() override;

		size_t Write(ConstBuffer data) override;
		size_t Read(Buffer) override;

		void Allocate(int mode, off_t offset, off_t len);
		void Truncate(size_t size);
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

	private:
		static int MapMode(FileOpenMode mode);
		static int MapMode(SeekMode mode);
	};

}}


#endif
