#ifndef TOOLKIT_RASTER_COLOR_H
#define TOOLKIT_RASTER_COLOR_H

#include <toolkit/core/types.h>
#include <toolkit/text/StringOutputStream.h>

namespace TOOLKIT_NS { namespace raster
{

	struct Color
	{
		u8 R, G, B, A;

		Color(): R(), G(), B(), A() { }
		Color(u8 r, u8 g, u8 b, u8 a = 255): R(r), G(g), B(b), A(a) { }

		static Color OpaqueWhite()
		{ return Color(255, 255, 255, 255); }

		bool IsWhite() const
		{ return (R & G & B) == 255; }

		bool IsTransparent() const
		{ return A == 0; }

		bool IsOpaque() const
		{ return A == 255; }

		void ToString(text::StringOutputStream & ss) const
		{ ss << "(r: " << R << ", g: " << G << ", b: " << B << ", a: " << A << ')'; }

		static u8 Blend(u8 a, u8 b, u8 n)
		{
			auto d = static_cast<u16>(b - a);
			return a + ((d * n + 255) >> 8);
		}

		static Color Blend(const Color &a, const Color &b, u8 n)
		{
			return {
				Blend(a.R, b.R, n),
				Blend(a.G, b.G, n),
				Blend(a.B, b.B, n),
				Blend(a.A, b.A, n),
			};
		}

		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
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
}}


#endif
