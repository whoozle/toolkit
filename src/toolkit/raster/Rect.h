#ifndef TOOLKIT_RASTER_RECT_H
#define TOOLKIT_RASTER_RECT_H

#include <toolkit/core/core.h>
#include <toolkit/text/StringOutputStream.h>
#include <toolkit/raster/Size.h>
#include <toolkit/raster/Point.h>

namespace TOOLKIT_NS { namespace raster
{

	struct Rect
	{
		int Left, Top, Right, Bottom;

		Rect(): Left(0), Top(0), Right(0), Bottom(0) { }
		Rect(int left, int top, int right, int bottom):
			Left(left), Top(top), Right(right), Bottom(bottom)
		{ }
		Rect(Size size): Left(0), Top(0), Right(size.Width), Bottom(size.Height) { }
		Rect(Point pos, Size size): Left(pos.X), Top(pos.Y), Right(pos.X + size.Width), Bottom(pos.Y + size.Height) { }
		Rect(Point pos1, Point pos2): Left(pos1.X), Top(pos1.Y), Right(pos2.X), Bottom(pos2.Y) { }

		void Reset()
		{ Left = Top = Right = Bottom = 0; }

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

		Point TopLeft() const
		{ return Point(Left, Top); }
		Point TopRight() const
		{ return Point(Right, Top); }
		Point BottomLeft() const
		{ return Point(Left, Bottom); }
		Point BottomRight() const
		{ return Point(Right, Bottom); }

		void Move(Point offset)
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

		void MoveTo(Point topLeft)
		{
			MoveToX(topLeft.X);
			MoveToY(topLeft.Y);
		}

		void Intersect(Rect other)
		{
			if (!other.Valid() || !Valid())
			{
				Reset();
				return;
			}

			Left 	= std::max(Left, other.Left);
			Top		= std::max(Top, other.Top);
			Right 	= std::min(Right, other.Right);
			Bottom	= std::min(Bottom, other.Bottom);
		}

		static Rect Intersect(Rect r1, Rect r2)
		{
			r1.Intersect(r2);
			return r1;
		}

		bool Contains(Point point) const
		{ return point.X >= Left && point.X < Right && point.Y >= Top && point.Y < Bottom; }

		bool IntersectsWith(Rect other) const
		{ return Left < other.Right && Right > other.Left && Top < other.Bottom && Bottom > other.Top; }

		static bool IntersectsWith(Rect r1, Rect r2)
		{ return r1.IntersectsWith(r2); }

		void Union(Rect other)
		{
			if (!other.Valid())
				return;

			if (!Valid())
			{
				*this = other;
				return;
			}

			Left 	= std::min(Left, other.Left);
			Top		= std::min(Top, other.Top);
			Right 	= std::max(Right, other.Right);
			Bottom	= std::max(Bottom, other.Bottom);
		}

		static Rect Union(Rect r1, Rect r2)
		{
			r1.Union(r2);
			return r1;
		}

		bool Valid() const
		{ return Right > Left && Bottom > Top; }

		int Area() const
		{ return Width() * Height(); }

		Rect operator - (Rect other) const
		{ return Rect(Point(Left - other.Left, Top - other.Top), Point(Right - other.Right, Bottom - other.Bottom)); }

		Rect operator + (Rect other) const
		{ return Rect(Point(Left + other.Left, Top + other.Top), Point(Right + other.Right, Bottom + other.Bottom)); }

		Rect & operator += (Point pos)
		{ Left += pos.X; Right += pos.X; Top += pos.Y; Bottom += pos.Y; return *this; }

		Rect & operator -= (Point pos)
		{ Left -= pos.X; Right -= pos.X; Top -= pos.Y; Bottom -= pos.Y; return *this; }

		void ToString(text::StringOutputStream & ss) const
		{ ss << '[' << Left << ", " << Top << ", " << Right << ", " << Bottom << ']'; }

		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
	};

}}


#endif
