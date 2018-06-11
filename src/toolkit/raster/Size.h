#ifndef TOOLKIT_RASTER_SIZE_H
#define TOOLKIT_RASTER_SIZE_H

TOOLKIT_NS_BEGIN
namespace raster
{
	struct Size
	{
		int Width, Height;

		Size(): Width(0), Height(0)
		{ }

		Size(int w, int h): Width(w), Height(h)
		{ }

		Size operator + (Size size) const
		{ return Size(Width + size.Width, Height + size.Height); }

		Size operator - (Size size) const
		{ return Size(Width - size.Width, Height - size.Height); }

		Size operator * (int m) const
		{ return Size(Width * m, Height * m); }

		Size operator / (int m) const
		{ return Size(Width / m, Height / m); }

		bool operator == (Size other) const
		{ return Width == other.Width && Height == other.Height; }

		bool operator != (Size other) const
		{ return !(*this == other); }
	};
}
TOOLKIT_NS_END

#endif