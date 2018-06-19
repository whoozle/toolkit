#ifndef TOOLKIT_RASTER_RECT_H
#define TOOLKIT_RASTER_RECT_H

#include <toolkit/core/core.h>
#include <toolkit/raster/Size.h>
#include <toolkit/raster/Position.h>
#include <sstream>

TOOLKIT_NS_BEGIN
namespace raster
{

	struct Rect
	{
		int Left, Top, Right, Bottom;

		Rect(): Left(0), Top(0), Right(0), Bottom(0) { }
		Rect(Size size): Left(0), Top(0), Right(size.Width), Bottom(size.Height) { }
		Rect(Position pos, Size size): Left(pos.X), Top(pos.Y), Right(pos.X + size.Width), Bottom(pos.Y + size.Height) { }
		Rect(Position pos1, Position pos2): Left(pos1.X), Top(pos1.Y), Right(pos2.X), Bottom(pos2.Y) { }

		int Width() const
		{ return Right - Left; }
		int Height() const
		{ return Bottom - Top; }
		Size GetSize() const
		{ return Size(Width(), Height()); }
		void SetWidth(int w)
		{ Right = Left + w; }
		void SetHeight(int h)
		{ Bottom = Top + h; }

		Position TopLeft() const
		{ return Position(Left, Top); }
		Position TopRight() const
		{ return Position(Right, Top); }
		Position BottomLeft() const
		{ return Position(Left, Bottom); }
		Position BottomRight() const
		{ return Position(Right, Bottom); }

		void Move(Position offset)
		{
			Left += offset.X;
			Right += offset.X;
			Top += offset.Y;
			Bottom += offset.Y;
		}

		void MoveToX(int x)
		{
			Right += x - Left;
			Left = x;
		}

		void MoveToY(int y)
		{
			Bottom += y - Top;
			Top = y;
		}

		void MoveTo(Position topLeft)
		{
			MoveToX(topLeft.X);
			MoveToY(topLeft.Y);
		}

		void Intersect(Rect other)
		{
			if (Left < other.Left)
				Left = other.Left;
			if (Right > other.Right)
				Right = other.Right;
			if (Top < other.Top)
				Top = other.Top;
			if (Bottom > other.Bottom)
				Bottom = other.Bottom;
		}

		static Rect Intersect(Rect r1, Rect r2)
		{
			Rect r(r1);
			r.Intersect(r2);
			return r;
		}

		bool Contains(Position point) const
		{ return point.X >= Left && point.X < Right && point.Y >= Top && point.Y < Bottom; }

		bool IntersectsWith(Rect other) const
		{ return Left < other.Right && Right > other.Left && Top < other.Bottom && Bottom > other.Top; }

		static bool IntersectsWith(Rect r1, Rect r2)
		{ return r1.IntersectsWith(r2); }

		bool Valid() const
		{ return Right > Left && Bottom > Top; }

		Rect operator - (Rect other) const
		{ return Rect(Position(Left - other.Left, Top - other.Top), Position(Right - other.Right, Bottom - other.Bottom)); }

		Rect operator + (Rect other) const
		{ return Rect(Position(Left + other.Left, Top + other.Top), Position(Right + other.Right, Bottom + other.Bottom)); }

		Rect & operator += (Position pos)
		{ Left += pos.X; Right += pos.X; Top += pos.Y; Bottom += pos.Y; return *this; }

		Rect & operator -= (Position pos)
		{ Left -= pos.X; Right -= pos.X; Top -= pos.Y; Bottom -= pos.Y; return *this; }

		std::string ToString() const
		{ std::stringstream ss; ss << '[' << Left << ", " << Top << ", " << Right << ", " << Bottom << ']'; return ss.str(); }
	};

}
TOOLKIT_NS_END

#endif
