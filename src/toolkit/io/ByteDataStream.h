#ifndef TOOLKIT_BYTEDATASTREAM_H
#define TOOLKIT_BYTEDATASTREAM_H

#include <toolkit/io/ByteData.h>
#include <toolkit/io/IStream.h>
#include <string.h>

TOOLKIT_NS_BEGIN
namespace io
{

	class ByteDataOutputStream : public IOutputStream
	{
		ByteData	_data;
		size_t		_offset;

	public:
		ByteDataOutputStream(ByteData data): _data(data), _offset(0) { }

		size_t Write(ConstByteData src) override
		{
			ByteData dst(_data, _offset);
			size_t n = std::min(dst.size(), src.size());
			_offset += n;
			memcpy(dst.data(), src.data(), n);
			return n;
		}
	};

}
TOOLKIT_NS_END

#endif
