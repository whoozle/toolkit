#ifndef TOOLKIT_RASTER_POINT_H
#define TOOLKIT_RASTER_POINT_H

#include <toolkit/core/core.h>
#include <toolkit/text/StringOutputStream.h>

namespace TOOLKIT_NS { namespace raster
{
	template <typename Type>
	struct PointBase
	{
		Type X, Y;

		PointBase(): X(0), Y(0)
		{ }

		PointBase(Type x, Type y): X(x), Y(y)
		{ }

		template<typename Size>
		explicit PointBase(Size size): X(size.Width), Y(size.Height)
		{ }

		PointBase operator - (PointBase p) const
		{ return PointBase(X - p.X, Y - p.Y); }

		PointBase operator - (Type scalar) const
		{ return *this - PointBase(scalar, scalar); }

		PointBase operator - () const
		{ return PointBase(-X, -Y); }

		PointBase operator + (PointBase p) const
		{ return PointBase(X + p.X, Y + p.Y); }

		PointBase operator + (Type scalar) const
		{ return *this + PointBase(scalar, scalar); }

		PointBase operator + () const
		{ return *this; }

		PointBase operator * (PointBase p) const
		{ return PointBase(X * p.X, Y * p.Y); }

		PointBase operator * (Type scalar) const
		{ return *this * PointBase(scalar, scalar); }

		PointBase operator / (PointBase p) const
		{ return PointBase(X / p.X, Y / p.Y); }

		PointBase operator / (Type scalar) const
		{ return *this / PointBase(scalar, scalar); }

		PointBase & operator -= (const PointBase & o)
		{
			X -= o.X;
			Y -= o.Y;
			return *this;
		}

		PointBase & operator += (const PointBase & o)
		{
			X += o.X;
			Y += o.Y;
			return *this;
		}

		bool IsZero() const
		{ return (X | Y) == 0; }

		void ToString(text::StringOutputStream & ss) const
		{ ss << X << ", " << Y; }

		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
	};

	using Point 	= PointBase<int>;
	using PointF 	= PointBase<float>;
}}


#endif
