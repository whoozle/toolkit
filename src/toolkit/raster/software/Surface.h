#ifndef TOOLKIT_RASTER_SOFTWARE_SURFACE_H
#define TOOLKIT_RASTER_SOFTWARE_SURFACE_H

#include <toolkit/raster/software/LockedRect.h>
#include <toolkit/raster/Color.h>
#include <toolkit/raster/Rect.h>
#include <toolkit/core/types.h>

#include <vector>

namespace TOOLKIT_NS { namespace raster { namespace software
{

	template<typename PixelFormat_>
	class MappedSurface
	{
	protected:
		u8 *			_data;
		size_t			_dataSize;
		raster::Size	_size;
		unsigned		_pitch;
		u8				_alpha;

	public:
		typedef PixelFormat_ PixelFormat;

		MappedSurface(u8 *data, size_t dataSize, raster::Size size, unsigned pitch, u8 alpha = 0xff):
			_data(data), _dataSize(dataSize), _size(size), _pitch(pitch), _alpha(alpha)
		{ }

		LockedRect<PixelFormat> Lock(raster::Rect rect)
		{ return LockedRect<PixelFormat>(rect, _data + rect.Top * _pitch + PixelFormat::BytesPerPixel * rect.Left, _pitch); }

		raster::Size GetSize() const
		{ return _size; }

		u8 GetAlpha() const
		{ return _alpha; }

		void SetAlpha(u8 a)
		{ _alpha = a; }
	};

	template<typename PixelFormat>
	class Surface : public MappedSurface<PixelFormat>
	{
		std::vector<u8> _storage;

	public:
		using super = MappedSurface<PixelFormat>;

		static const unsigned Alignment = 16;

		static unsigned GetOptimalPitch(unsigned width, unsigned alignment = Alignment)
		{ return (width * PixelFormat::BytesPerPixel + alignment - 1) / alignment * alignment; }

		Surface(raster::Size size, unsigned pitch, u8 alpha = 0xff):
			super(NULL, pitch * size.Height, size, pitch, alpha),
			_storage(super::_dataSize)
		{ this->_data = _storage.data(); }

		Surface(raster::Size size): Surface(size, GetOptimalPitch(size.Width, Alignment))
		{ }
	};

}}}


#endif
