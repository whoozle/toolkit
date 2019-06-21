#ifndef TOOLKIT_BASERENDERER_H
#define TOOLKIT_BASERENDERER_H

#include <toolkit/raster/Point.h>
#include <toolkit/raster/Rect.h>

namespace TOOLKIT_NS { namespace raster { namespace software
{

	struct BaseRenderer
	{
		bool ClipRect(const raster::Rect &clipRect, raster::Rect &dstRect, raster::Rect &srcRect)
		{
			auto result = Rect::Intersect(dstRect, clipRect);
			auto tl = result.TopLeft() - dstRect.TopLeft();
			auto br = result.BottomRight() - dstRect.BottomRight();
			if (srcRect.GetSize() == dstRect.GetSize())
			{
				srcRect.Left 	+= tl.X;
				srcRect.Top 	+= tl.Y;
				srcRect.Right 	+= br.X;
				srcRect.Bottom 	+= br.Y;
			}
			else
			{
				auto mx = srcRect.Width() * 1.0f / dstRect.Width();
				auto my = srcRect.Height() * 1.0f / dstRect.Height();
				srcRect.Left 	+= tl.X * mx;
				srcRect.Top 	+= tl.Y * my;
				srcRect.Right 	+= br.X * mx;
				srcRect.Bottom 	+= br.Y * my;
			}
			dstRect = result;
			return dstRect.Valid() && srcRect.Valid();
		}

		bool ClipRect(const raster::Rect &clipRect, raster::Rect &dstRect)
		{
			dstRect.Intersect(clipRect);
			return dstRect.Valid();
		}
	};

}}}


#endif
