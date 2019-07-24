#include <toolkit/text/Formatters.h>

namespace TOOLKIT_NS { namespace text
{
	void HexDump::ToString(StringOutputStream & ss) const
	{
		if (!Name.empty())
			ss << Name << ": ";
		if (Buffer.empty())
		{
			ss << "[empty]";
			return;
		}

		for(size_t base = 0, size = Buffer.size(); base < size; base += 16)
		{
			ss << '\n' << Hex(base, 8) << ": ";
			size_t i;
			size_t lineSize = std::min<size_t>(size - base, 16);
			for(i = 0; i < lineSize; ++i)
			{
				ss << Hex(Buffer[base + i], 2) << ' ';
			}
			for(; i < 16; ++i)
				ss << "   ";

			ss << "\t";

			for(i = 0; i < lineSize; ++i)
			{
				char ch = Buffer[base + i];
				ss << (ch >= 0x20 && ch < 0x7f? ch: '.');
			}
		}
	}
}}
