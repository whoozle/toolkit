#ifndef TOOLKIT_SERIALIZATION_BINARYREADERBASE_H
#define TOOLKIT_SERIALIZATION_BINARYREADERBASE_H

#include <functional>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename Type>
	class BaseBinaryReader : Noncopyable
	{
	public:
		using Callback = std::function<void (Type &&)>;

	protected:
		Callback _callback;

	public:
		BaseBinaryReader(Callback callback): _callback(callback)
		{ }
	};

	template<typename Type>
	struct BinaryReader;

}}

#endif
