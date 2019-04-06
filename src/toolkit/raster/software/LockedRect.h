#ifndef TOOLKIT_SOFTWARE_LOCKEDRECT_H
#define TOOLKIT_SOFTWARE_LOCKEDRECT_H

#include <toolkit/raster/Rect.h>
#include <toolkit/raster/Color.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>

namespace TOOLKIT_NS { namespace raster { namespace software
{

	template<typename PixelFormat>
	class PixelIterator
	{
		u8 * 		_data;
		unsigned	_w;

	public:
		PixelIterator(u8 * data, unsigned w): _data(data), _w(w) { }

		bool operator--(int)
		{ return _w--; }

		void operator++()
		{ _data += PixelFormat::BytesPerPixel; }

		void operator+=(int pixels)
		{ _data += pixels * PixelFormat::BytesPerPixel; }

		PixelIterator operator + (int value) const
		{ PixelIterator p(*this); p += value; return p; }

		raster::MappedColor<PixelFormat> operator*()
		{ return raster::MappedColor<PixelFormat>(_data); }
	};

	template<typename PixelFormat>
	class PixelRowsIterator
	{
		static const unsigned BytesPerPixel = PixelFormat::BytesPerPixel;

		using MappedType = typename PixelFormat::MappedType;

		raster::Size		_size;
		unsigned			_y;
		u8 *				_data;
		const unsigned		_pitch;

	public:
		PixelRowsIterator(raster::Size size, u8 *data, unsigned pitch):
			_size(size), _y(size.Height), _data(data), _pitch(pitch)
		{ }

		bool operator -- (int)
		{ return _y--; }

		void operator ++ ()
		{ _data += _pitch; }

		void operator += (int lines)
		{ _data += _pitch * lines; }

		PixelRowsIterator operator + (int lines) const
		{ PixelRowsIterator p(*this); p += lines; return p; }

		u8 * GetData()
		{ return _data; }

		PixelIterator<PixelFormat> GetLine()
		{ return PixelIterator<PixelFormat>(_data, _size.Width); }
	};

	template<typename PixelFormat>
	class LockedRect : Noncopyable
	{
		raster::Rect	_rect;
		u8 *			_data;
		unsigned		_pitch;

	public:
		struct Data
		{
			u8 *		Data;
			unsigned	Pitch;
		};

		LockedRect(raster::Rect rect, u8 *data, unsigned pitch):
			_rect(rect), _data(data), _pitch(pitch)
		{ }

		LockedRect(raster::Rect rect, Data data):
			_rect(rect), _data(data.Data), _pitch(data.Pitch)
		{ }

		LockedRect(LockedRect &&r): _rect(std::move(r._rect)), _data(std::move(r._data)), _pitch(std::move(r._pitch))
		{ }

		u8 * GetData()
		{ return _data; }

		unsigned GetPitch() const
		{ return _pitch; }

		PixelRowsIterator<PixelFormat> GetPixels()
		{ return PixelRowsIterator<PixelFormat>(_rect.GetSize(), _data, _pitch); }
	};


}}}


#endif
