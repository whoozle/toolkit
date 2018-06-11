#ifndef TOOLKIT_RASTER_POSITION_H
#define TOOLKIT_RASTER_POSITION_H

TOOLKIT_NS_BEGIN
namespace raster
{
	struct Position
	{
		int X, Y;

		Position(): X(0), Y(0)
		{ }

		Position(int x, int y): X(x), Y(y)
		{ }

		template<typename Size>
		explicit Position(Size size): X(size.Width), Y(size.Height)
		{ }

		Position operator - (Position p) const
		{ return Position(X - p.X, Y - p.Y); }

		Position operator - () const
		{ return Position(-X, -Y); }

		Position operator + (Position p) const
		{ return Position(X + p.X, Y + p.Y); }
	};
}
TOOLKIT_NS_END

#endif
