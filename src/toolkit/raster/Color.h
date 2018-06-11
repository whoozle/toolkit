#ifndef TOOLKIT_RASTER_COLOR_H
#define TOOLKIT_RASTER_COLOR_H

#include <toolkit/core/types.h>

TOOLKIT_NS_BEGIN
namespace raster
{

	struct Color
	{
		u8 R, G, B, A;

		Color(): R(), G(), B(), A() { }
		Color(u8 r, u8 g, u8 b, u8 a = 255): R(r), G(g), B(b), A(a) { }
	};

	template<typename PixelFormat_>
	struct MappedColor
	{
		using PixelFormat = PixelFormat_;
		using MappedType = typename PixelFormat::MappedType;

		MappedType *_data;

		MappedColor(void *data): _data(static_cast<MappedType *>(data)) { }

		template<typename SrcPixelFormat>
		MappedColor & operator = (MappedColor<SrcPixelFormat> other)
		{
			*this = SrcPixelFormat::Unmap(*other);
			return *this;
		}

		MappedColor & operator = (MappedColor other)
		{ *this = *other; return *this; }

		MappedType operator*() const
		{ return *_data; }

		MappedColor & operator = (Color color)
		{
			*_data = PixelFormat::Map(color);
			return *this;
		}

		MappedColor & operator = (MappedType color)
		{
			*_data = color;
			return *this;
		}
	};
}
TOOLKIT_NS_END

#endif
