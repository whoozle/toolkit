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
			Point delta = clipRect.TopLeft() - dstRect.TopLeft();
			//if we're clipping dstRect left/top position, we need to move src rectangle for the same amount too
			if (delta.X < 0)
				delta.X = 0;
			if (delta.Y < 0)
				delta.Y = 0;
			dstRect.Intersect(clipRect);
			srcRect.Left += delta.X;
			srcRect.Top += delta.Y;
			//printf("%s %d <- %s %d\n", dstRect.ToString().c_str(), dstRect.Valid(), srcRect.ToString().c_str(), srcRect.Valid());
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
