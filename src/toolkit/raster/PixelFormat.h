#ifndef TOOLKIT_RASTER_PIXELFORMAT_H
#define TOOLKIT_RASTER_PIXELFORMAT_H

#include <toolkit/core/types.h>
#include <toolkit/raster/Color.h>

TOOLKIT_NS_BEGIN
namespace raster
{
	namespace impl
	{
		template<unsigned Offset_, unsigned Size_>
		struct ColorChannel
		{
			static const unsigned Offset = Offset_;
			static const unsigned Size = Size_;
			static const unsigned Loss = 8 - Size;
			static const unsigned Mask = (1 << Size) - 1;

			template<typename MappedType>
			static u8 Unmap(MappedType value)
			{ return ((value >> Offset) & Mask) << Loss; }

			template<typename MappedType>
			static MappedType Map(u8 value)
			{ return static_cast<MappedType>(value >> Loss) << Offset; }
		};

		template<unsigned Offset_>
		struct ColorChannel<Offset_, 8>
		{
			static const unsigned Offset = Offset_;
			static const unsigned Size = 8;
			static const unsigned Loss = 8 - Size;
			static const unsigned Mask = (1 << Size) - 1;

			template<typename MappedType>
			static u8 Unmap(MappedType value)
			{ return value >> Offset; }

			template<typename MappedType>
			static MappedType Map(u8 value)
			{ return static_cast<MappedType>(value) << Offset; }
		};

		template<unsigned Offset_>
		struct ColorChannel<Offset_, 0>
		{
			static const unsigned Offset = Offset_;
			static const unsigned Size = 0;
			static const unsigned Loss = 8;
			static const unsigned Mask = 0;

			template<typename MappedType>
			static u8 Unmap(MappedType value)
			{ return 0xff; } //return A = 255

			template<typename MappedType>
			static MappedType Map(u8 value)
			{ return 0; }
		};

		template<typename MappedType_,
			unsigned ROffset, unsigned RSize,
			unsigned GOffset, unsigned GSize,
			unsigned BOffset, unsigned BSize,
			unsigned AOffset, unsigned ASize
		>
		struct PixelFormat
		{
			using MappedType = MappedType_;

			static const unsigned BytesPerPixel = sizeof(MappedType);
			static const unsigned BitsPerPixel = BytesPerPixel * 8;

			using R = ColorChannel<ROffset, RSize>;
			using G = ColorChannel<GOffset, GSize>;
			using B = ColorChannel<BOffset, BSize>;
			using A = ColorChannel<AOffset, ASize>;

			static MappedType Map(Color color)
			{
				return
					R::template Map<MappedType_>(color.R) |
					G::template Map<MappedType_>(color.G) |
					B::template Map<MappedType_>(color.B) |
					A::template Map<MappedType_>(color.A);
			}

			static Color Unmap(MappedType value)
			{
				return Color(
					R::template Unmap<MappedType>(value),
					G::template Unmap<MappedType>(value),
					B::template Unmap<MappedType>(value),
					A::template Unmap<MappedType>(value)
				);
			}
		};
	}

	using RGB565	= impl::PixelFormat<u16,  0, 5,  5, 6,  11, 5,  0, 0>;
	using BGR565	= impl::PixelFormat<u16,  11,5,  5, 6,  0,  5,  0, 0>;
	using RGB555	= impl::PixelFormat<u16,  0, 5,  5, 5,  10, 5,  0, 0>;
	using BGR555	= impl::PixelFormat<u16,  10,5,  5, 5,  0,  5,  0, 0>;
	using RGB24		= impl::PixelFormat<u32,  0, 8,  8, 8,  16, 8,  0, 0>;
	using ARGB		= impl::PixelFormat<u32,  0, 8,  8, 8,  16, 8,  24, 8>;
}
TOOLKIT_NS_END

#endif
