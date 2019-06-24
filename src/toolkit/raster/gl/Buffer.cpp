#include <toolkit/raster/gl/Buffer.h>
#include <toolkit/raster/gl/Utils.h>

namespace TOOLKIT_NS { namespace gl
{
	Buffer::Buffer()
	{
		TOOLKIT_GL_CALL(glGenBuffers(1, &_buffer));
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &_buffer);
	}

	void Buffer::Bind(GLenum target)
	{
		TOOLKIT_GL_CALL(glBindBuffer(target, _buffer));
	}

	void Buffer::BufferData(GLenum target, gl::BufferData data, GLenum usage)
	{
		TOOLKIT_GL_CALL(glBufferData(target, data.Size, data.Data, usage));
	}

}}
