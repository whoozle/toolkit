#ifndef TOOLKIT_IO_MEMORYMAPPING_H
#define TOOLKIT_IO_MEMORYMAPPING_H

#include <toolkit/core/core.h>
#include <toolkit/core/Enum.h>
#include <toolkit/core/Buffer.h>
#include <memory>
#include <sys/mman.h>

namespace TOOLKIT_NS { namespace io
{
#if 0 //fixme: later
	enum struct MemoryProtection : int
	{
		None		= 0,
		Read		= 1,
		Write		= 2,
		ReadWrite	= 3,
		Execute		= 4
	};
	DECLARE_ENUM_BIT_OPERATORS(MemoryProtection)
#endif

	class MemoryMapping
	{
		Buffer _data;

	public:
		MemoryMapping(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
		~MemoryMapping();

		void Remap(size_t new_size, int flags, void *new_address = nullptr);

		static void Sync(void *addr, size_t size, int flags);
		static void Sync(Buffer data, int flags)
		{ Sync(data.data(), data.size(), flags); }

		void Sync(int flags)
		{ Sync(_data, flags); }

		Buffer GetData()
		{ return _data; }

		ConstBuffer GetData() const
		{ return _data; }

		static size_t GetPageSize();
	private:
		//int Map(MemoryProtection prot);
	};
	TOOLKIT_DECLARE_PTR(MemoryMapping);
}}



#endif
