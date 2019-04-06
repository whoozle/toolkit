#ifndef TOOLKIT_RASTER_BLENDER_H
#define TOOLKIT_RASTER_BLENDER_H

#include <toolkit/raster/PixelFormat.h>
#include <toolkit/core/type_traits.h>

namespace TOOLKIT_NS
{

namespace raster { namespace software
{
		//https://habrahabr.ru/post/128773/
		//16 bit blending with 1 mul per pixel

		template<typename R, typename I, typename T1, typename T2 = T1>
		R Mul255(T1 a, T2 b)
		{
			I v = a * b;
			//accurate approximation
			//return (v + 1 + (v >> 8)) >> 8;

			//a bit innacurate(-1 errors), but giving proper result in corner cases
			return (v + 255) >> 8;
		}

		template<typename DstPixelFormat, typename SrcPixelFormat>
		struct Blender
		{
			using DstMappedType = typename DstPixelFormat::MappedType;
			using SrcMappedType = typename SrcPixelFormat::MappedType;

			static DstMappedType Blend(DstMappedType dst, SrcMappedType src, Color color)
			{
				Color cs = SrcPixelFormat::Unmap(src);
				if (cs.A == 0)
					return dst;

				Color cd = DstPixelFormat::Unmap(dst);
				return DstPixelFormat::Map(Color(
					cd.R + Mul255<u8, s16, u8, s16>(cs.A, (s16)cs.R - cd.R),
					cd.G + Mul255<u8, s16, u8, s16>(cs.A, (s16)cs.G - cd.G),
					cd.B + Mul255<u8, s16, u8, s16>(cs.A, (s16)cs.B - cd.B),
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

			static MappedType Blend(MappedType dst, MappedType src, Color color)
			{
				u8 srcAlpha = ARGB::A::Unmap(src);
				if (srcAlpha == 0)
					return dst;

				MappedType srcRB = src & RBMask, dstRB = dst & RBMask;
				MappedType srcG  = src & GMask, dstG = dst & GMask;
				MappedType RB = Mul255<MappedType, MappedType, MappedType>(srcRB - dstRB, srcAlpha);
				MappedType G  = Mul255<MappedType, MappedType, MappedType>(srcG - dstG, srcAlpha);

				u8 a = std::max<u8>(srcAlpha, 255 - srcAlpha);
				return
					((RB + dstRB) & RBMask) |
					((G + dstG) & GMask)|
					ARGB::A::Map<u32>(a);
			}
		};

		template<typename DstPixelFormat>
		struct Blender<DstPixelFormat, A8>
		{
			using DstMappedType = typename DstPixelFormat::MappedType;
			using SrcMappedType = typename A8::MappedType;

			static DstMappedType Blend(DstMappedType dst, SrcMappedType src, Color color)
			{
				color.A = Mul255<u8, u16, u8, u8>(color.A, src);
				return Blender<ARGB, ARGB>::Blend(dst, DstPixelFormat::Map(color), Color::OpaqueWhite());
			}
		};
}}

}

#endif
