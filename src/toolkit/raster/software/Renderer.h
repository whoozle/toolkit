#ifndef TOOLKIT_SOFTWARERENDERER_H
#define TOOLKIT_SOFTWARERENDERER_H

#include <toolkit/raster/software/BaseRenderer.h>
#include <toolkit/raster/Blender.h>

namespace TOOLKIT_NS { namespace raster { namespace software
{

	struct Renderer : public BaseRenderer
	{
		using super = BaseRenderer;
		using super::super;

		template<typename DstSurface, typename SrcSurface>
		void Blend(DstSurface &dstSurface, raster::Rect clipRect, raster::Point dstPos, SrcSurface &srcSurface, raster::Rect srcRect, Color color)
		{
			using Blender = raster::software::Blender<typename DstSurface::PixelFormat, typename SrcSurface::PixelFormat>;

			srcRect.Intersect(srcSurface.GetSize());
			raster::Rect dstRect(dstPos, srcRect.GetSize());
			if (!super::ClipRect(clipRect, dstRect, srcRect))
				return;

			auto dstLock = dstSurface.Lock(dstRect);
			auto srcLock = srcSurface.Lock(srcRect);

			auto dstRow = dstLock.GetPixels();
			auto srcRow = srcLock.GetPixels();

			for(; dstRow--; ++dstRow, ++srcRow)
			{
				auto dst = dstRow.GetLine();
				auto src = srcRow.GetLine();
				for(; dst--; ++dst, ++src)
				{
					*dst = Blender::Blend(**dst, **src, color);
				}
			}
		}

		template<typename DstSurface, typename SrcSurface>
		void Blend(DstSurface &dstSurface, raster::Rect clipRect, raster::Rect dstRect, SrcSurface &srcSurface, raster::Rect srcRect, Color color)
		{
			using Blender = raster::software::Blender<typename DstSurface::PixelFormat, typename SrcSurface::PixelFormat>;
			if (dstRect.GetSize() == srcRect.GetSize())
			{
				Blend(dstSurface, clipRect, dstRect.TopLeft(), srcSurface, srcRect, color);
				return;
			}

			srcRect.Intersect(srcSurface.GetSize());
			if (!super::ClipRect(clipRect, dstRect, srcRect))
				return;

			auto dstLock = dstSurface.Lock(dstRect);
			auto srcLock = srcSurface.Lock(srcRect);

			auto dstRow = dstLock.GetPixels();
			const auto srcData = srcLock.GetPixels();

			auto srcSize = srcSurface.GetSize();
			auto dstSize = dstRect.GetSize();
			if (srcSize.Width > dstSize.Width) //h-downscale
			{
				for(unsigned dstY = 0; dstRow--; ++dstRow, ++dstY)
				{
					auto dst = dstRow.GetLine();
					auto src = (srcData + (srcSize.Height * dstY / dstSize.Height)).GetLine();
					for(unsigned dstX = 0, dstNext = 0; dst--; ++dst, ++dstX, dstNext += srcSize.Width)
					{
						while(dstNext > (unsigned)dstSize.Width)
						{
							*dst = Blender::Blend(**dst, **src, color);
							dstNext -= dstSize.Width;
							++src;
						}
					}
				}
			}
			else //h-upscale
			{
				for(unsigned dstY = 0; dstRow--; ++dstRow, ++dstY)
				{
					auto dst = dstRow.GetLine();
					const auto srcLine = (srcData + (srcSize.Height * dstY / dstSize.Height)).GetLine();
					for(unsigned dstX = 0; dst--; ++dst, ++dstX)
					{
						auto src = srcLine + srcSize.Width * dstX / dstSize.Width; //fixme: remove division
						*dst = Blender::Blend(**dst, **src, color);
					}
				}
			}
		}

		template<typename DstSurface, typename SrcSurface>
		void Blit(DstSurface &dstSurface, raster::Rect clipRect, raster::Point dstPos, SrcSurface &srcSurface, raster::Rect srcRect)
		{
			using DstSurfacePixelFormat = typename DstSurface::PixelFormat;
			using SrcSurfacePixelFormat = typename SrcSurface::PixelFormat;

			srcRect.Intersect(srcSurface.GetSize());
			raster::Rect dstRect(dstPos, srcRect.GetSize());
			if (!super::ClipRect(clipRect, dstRect, srcRect))
				return;

			auto dstLock = dstSurface.Lock(dstRect);
			auto srcLock = srcSurface.Lock(srcRect);

			auto dstRow = dstLock.GetPixels();
			auto srcRow = srcLock.GetPixels();

			for(; dstRow--; ++dstRow, ++srcRow)
			{
				auto dst = dstRow.GetLine();
				auto src = srcRow.GetLine();
				for(; dst--; ++dst, ++src)
				{
					*dst = DstSurfacePixelFormat::Map(SrcSurfacePixelFormat::Unmap(**src));
				}
			}
		}

		template<typename DstSurface, typename SrcSurface>
		typename std::enable_if<std::is_same<typename SrcSurface::PixelFormat, A8>::value, void>::type Blit(DstSurface &dstSurface, raster::Rect clipRect, raster::Point dstPos, SrcSurface &srcSurface, raster::Rect srcRect, raster::Color color)
		{
			using DstSurfacePixelFormat = typename DstSurface::PixelFormat;

			srcRect.Intersect(srcSurface.GetSize());
			raster::Rect dstRect(dstPos, srcRect.GetSize());
			if (!super::ClipRect(clipRect, dstRect, srcRect))
				return;

			auto dstLock = dstSurface.Lock(dstRect);
			auto srcLock = srcSurface.Lock(srcRect);

			auto dstRow = dstLock.GetPixels();
			auto srcRow = srcLock.GetPixels();

			for(; dstRow--; ++dstRow, ++srcRow)
			{
				auto dst = dstRow.GetLine();
				auto src = srcRow.GetLine();
				for(; dst--; ++dst, ++src)
				{
					color.A = **src;
					*dst = DstSurfacePixelFormat::Map(color);
				}
			}
		}

		template<typename DstSurface>
		void ClearRect(DstSurface &dstSurface, raster::Rect clipRect, raster::Rect rect, raster::Color color)
		{
			if (!super::ClipRect(clipRect, rect))
				return;

			auto lock = dstSurface.Lock(rect);
			auto mc = DstSurface::PixelFormat::Map(color);

			for(auto row = lock.GetPixels(); row--; ++row)
			{
				for(auto column = row.GetLine(); column--; ++column)
				{
					*column = mc;
				}
			}
		}

		template<typename DstSurface>
		void BlendRect(DstSurface &dstSurface, raster::Rect clipRect, raster::Rect rect, raster::Color color)
		{
			using Blender = raster::software::Blender<typename DstSurface::PixelFormat, typename DstSurface::PixelFormat>;
			auto srcColor = DstSurface::PixelFormat::Map(color);

			if (!super::ClipRect(clipRect, rect))
				return;

			auto lock = dstSurface.Lock(rect);

			for(auto row = lock.GetPixels(); row--; ++row)
			{
				for(auto column = row.GetLine(); column--; ++column)
				{
					*column = Blender::Blend(**column, srcColor, color);
				}
			}
		}
	};

}}}


#endif
