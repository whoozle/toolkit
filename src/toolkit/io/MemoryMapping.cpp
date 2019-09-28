#include <toolkit/io/MemoryMapping.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/log/Logger.h>
#include <unistd.h>

namespace TOOLKIT_NS { namespace io
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

	void MemoryMapping::Remap(size_t new_size, int flags)
	{
		void * addr = mremap(_data.data(), _data.size(), new_size, flags);
		if (addr == MAP_FAILED)
			throw SystemException("mremap");
		_data = Buffer(static_cast<u8*>(addr), new_size);
	}
	void MemoryMapping::Remap(size_t new_size, int flags, void *new_address)
	{
		void * addr = mremap(_data.data(), _data.size(), new_size, flags, new_address);
		if (addr == MAP_FAILED)
			throw SystemException("mremap");
		_data = Buffer(static_cast<u8*>(addr), new_size);
	}

	size_t MemoryMapping::GetPageSize()
	{ return sysconf(_SC_PAGESIZE); }
}}

