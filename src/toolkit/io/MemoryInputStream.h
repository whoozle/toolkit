#ifndef TOOLKIT_MEMORYINPUTSTREAM_H
#define TOOLKIT_MEMORYINPUTSTREAM_H

#include <toolkit/core/ByteArray.h>
#include <toolkit/io/IStream.h>
#include <string.h>
#include <vector>
#include <algorithm>

namespace TOOLKIT_NS { namespace io
{

	class MemoryInputStream : public ISeekableInputStream
	{
	public:
		using Storage = std::vector<u8>;

	private:
		Storage 	_data;
		size_t 		_pos;

	public:
		template<typename ...Arg>
		MemoryInputStream(Arg && ... arg): _data(std::forward<Arg>(arg)...), _pos(0)
		{ }

		size_t Read(Buffer buffer) override
		{
			size_t to_read = std::min(buffer.size(), _data.size() - _pos);
			std::copy_n(_data.data() + _pos, to_read, buffer.data());
			return to_read;
		}

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			switch(mode)
			{
			case SeekMode::Begin:
				_pos = offset;
				break;
			case SeekMode::Current:
				_pos += offset;
				break;
			case SeekMode::End:
				_pos = _data.size() + offset;
				break;
			}
			return _pos;
		}

		off_t Tell() override
		{ return _pos; }

		size_t GetSize() const
		{ return _data.size(); }

		const Storage & GetStorage() const
		{ return _data; }

		Storage & GetStorage()
		{ return _data; }
	};

}}


#endif
