#ifndef TOOLKIT_RASTER_SIZE_H
#define TOOLKIT_RASTER_SIZE_H

#include <toolkit/core/core.h>
#include <toolkit/text/StringOutputStream.h>
#include <string>

namespace TOOLKIT_NS { namespace raster
{
	template <typename Type>
	struct SizeBase
	{
		Type Width, Height;

		SizeBase(): Width(0), Height(0)
		{ }

		SizeBase(Type w, Type h): Width(w), Height(h)
		{ }

		SizeBase operator + (SizeBase size) const
		{ return SizeBase(Width + size.Width, Height + size.Height); }

		SizeBase operator - (SizeBase size) const
		{ return SizeBase(Width - size.Width, Height - size.Height); }

		SizeBase operator * (Type m) const
		{ return SizeBase(Width * m, Height * m); }

		SizeBase operator / (Type m) const
		{ return SizeBase(Width / m, Height / m); }

		bool operator == (SizeBase other) const
		{ return Width == other.Width && Height == other.Height; }

		bool operator != (SizeBase other) const
		{ return !(*this == other); }

		bool Valid() const
		{ return Width > 0 && Height > 0; }

		template<typename DstType>
		SizeBase<DstType> As() const
		{ return {static_cast<DstType>(Width), static_cast<DstType>(Height)}; }

		void ToString(text::StringOutputStream & ss) const
		{ ss << Width << 'x' << Height; }

		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
	};
	using Size 	= SizeBase<int>;
	using SizeF = SizeBase<float>;
}}


#endif
