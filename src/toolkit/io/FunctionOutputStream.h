#ifndef TOOLKIT_IO_FUNCTIONOUTPUTSTREAM_H
#define TOOLKIT_IO_FUNCTIONOUTPUTSTREAM_H

#include <toolkit/io/IStream.h>
#include <functional>

namespace TOOLKIT_NS { namespace io
{

	class FunctionOutputStream final : public IOutputStream
	{
	public:
		using OutputCallback = std::function<size_t (ConstBuffer)>;

	private:
		OutputCallback _cb;

	public:
		FunctionOutputStream(OutputCallback cb): _cb(std::move(cb))
		{ }

		size_t Write(ConstBuffer data) override
		{ return _cb(data); }
	};

}}

#endif
