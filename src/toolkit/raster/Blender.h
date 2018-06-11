#ifndef TOOLKIT_RASTER_BLENDER_H
#define TOOLKIT_RASTER_BLENDER_H

#include <toolkit/raster/PixelFormat.h>

TOOLKIT_NS_BEGIN

namespace raster
{

	namespace software
	{
		//https://habrahabr.ru/post/128773/
		//16 bit blending with 1 mul per pixel

		template<typename DstPixelFormat, typename SrcPixelFormat>
		struct Blender
		{
			using DstMappedType = typename DstPixelFormat::MappedType;
			using SrcMappedType = typename SrcPixelFormat::MappedType;

			static DstMappedType Blend(DstMappedType dst, SrcMappedType src)
			{
				Color cs = SrcPixelFormat::Unmap(src);
				if (cs.A == 0)
					return dst;

				Color cd = DstPixelFormat::Unmap(dst);
				return DstPixelFormat::Map(Color(
					cd.R + ((cs.A * ((s16)cs.R - cd.R) + 255) >> 8),
					cd.G + ((cs.A * ((s16)cs.G - cd.G) + 255) >> 8),
					cd.B + ((cs.A * ((s16)cs.B - cd.B) + 255) >> 8),
					std::max(cs.A, cd.A) //replace with a + b - a * b?
				));
			}
		};

		template<>
		struct Blender<ARGB, ARGB>
		{
			using MappedType = ARGB::MappedType;

			static const MappedType RBMask	= 0x00ff00ff;
			static const MappedType GMask	= 0x0000ff00;

			static MappedType Blend(MappedType dst, MappedType src)
			{
				u8 srcAlpha = ARGB::A::Unmap(src);
				if (srcAlpha == 0)
					return dst;

				MappedType srcRB = src & RBMask, dstRB = dst & RBMask;
				MappedType srcG = src & GMask, dstG = dst & GMask;
				MappedType RB = ((srcRB - dstRB) * srcAlpha) >> 8; //fixme: scale ?
				MappedType G = ((srcG - dstG) * srcAlpha) >> 8;

				u8 a = std::max<u8>(srcAlpha, 255 - srcAlpha);
				return
					((RB + dstRB) & RBMask) |
					((G + dstG) & GMask)|
					ARGB::A::Map<u32>(a);
			}
		};
	}

}

TOOLKIT_NS_END

#endif
