#ifndef TOOLKIT_RASTER_GL_BUFFER_H
#define TOOLKIT_RASTER_GL_BUFFER_H

#include <toolkit/raster/gl/Utils.h>
#include <toolkit/core/Noncopyable.h>

namespace TOOLKIT_NS { namespace gl
{

	struct BufferData
	{
		const void *		Data;
		size_t				Size;
		GLenum				Type;

		BufferData(): Data(), Size(), Type() { }
		BufferData(const void *data, size_t size, GLenum type): Data(data), Size(size), Type(type) { }
	};

	class Buffer : public Noncopyable
	{
		GLuint		_buffer;

	public:
		Buffer();
		~Buffer();

		void Bind(GLenum target);

		static void BufferData(GLenum target, gl::BufferData data, GLenum usage);
	};
	TOOLKIT_DECLARE_PTR(Buffer);

}}

#endif
