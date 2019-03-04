#include <toolkit/io/MemoryMapping.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/log/Logger.h>
#include <unistd.h>

TOOLKIT_NS_BEGIN
namespace io
{
	MemoryMapping::MemoryMapping(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
	{
		void *ptr = mmap(addr, length, prot, flags, fd, offset);
		if (ptr == MAP_FAILED)
			throw SystemException("mmap failed");
		_data = Buffer(static_cast<u8*>(ptr), length);
	}

	MemoryMapping::~MemoryMapping()
	{
		int r = munmap(_data.data(), _data.size());
		if (r != 0)
		{
			auto error = SystemException::GetErrorMessage();
			static log::Logger log("memory-mapping");
			log.Error() << "munmap failed: " << error;
		}
	}

	void MemoryMapping::Sync(void *addr, size_t size, int flags)
	{ SYSTEM_CALL(msync(addr, size, flags)); }

	size_t MemoryMapping::GetPageSize()
	{ return sysconf(_SC_PAGESIZE); }
}
TOOLKIT_NS_END
