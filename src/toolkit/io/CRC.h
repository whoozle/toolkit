#ifndef TOOLKIT_IO_CRC_H
#define TOOLKIT_IO_CRC_H

#include <toolkit/core/types.h>
#include <toolkit/core/Buffer.h>

namespace TOOLKIT_NS { namespace io
{

	namespace impl
	{
		template <typename Type, Type Poly>
		class CRC
		{
		private:
			Type _value;

		public:
			CRC(Type value = Type(0)): _value(value) { }

			template<typename Iterator>
			void Update(Iterator begin, Iterator end)
			{
				_value = ~_value;
				while(begin != end)
				{
					_value ^= *begin++;
					for(uint k = 8; k--; )
						_value = (_value >> 1) ^ (Poly & (Type(0) - (_value & 1)));
				}
				_value = ~_value;
			}

			void Update(ConstBuffer data)
			{
				auto begin = data.data();
				auto end = begin + data.size();
				Update(begin, end);
			}

			Type Get() const
			{ return _value; }
		};
	}

	using CRC32		= impl::CRC<u32, 0xEDB88320>;
	using CRC32C	= impl::CRC<u32, 0x82F63B78>;

}}


#endif

