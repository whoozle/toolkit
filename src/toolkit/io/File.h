#ifndef TOOLKIT_IO_FILE_H
#define TOOLKIT_IO_FILE_H

#include <toolkit/io/IStream.h>
#include <string>
#include <sys/stat.h>

TOOLKIT_SUBNS_BEGIN(io)

	enum struct FileOpenMode
	{
		Readonly,
		ReadWrite
	};

	class File : public IStream
	{
	private:
		int _fd;

	public:
		explicit File(int fd): _fd(fd) { }
		File(const std::string &path, FileOpenMode mode);
		~File();

		void Sync(SyncMode mode) override;
		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override;
		off_t Tell() override;

		size_t Write(ConstByteData data) override;
		size_t Read(ByteData) override;

		void Allocate(int mode, off_t offset, off_t len);
		struct stat GetStatus();

	private:
		static int MapMode(FileOpenMode mode);
		static int MapMode(SeekMode mode);
	};

TOOLKIT_SUBNS_END

#endif
