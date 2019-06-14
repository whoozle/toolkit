#include <toolkit/raster/gl/Buffer.h>
#include <toolkit/raster/gl/Utils.h>

namespace TOOLKIT_NS { namespace gl
{
	Buffer::Buffer()
	{
		glGenBuffers(1, &_buffer);
		CheckGLError();
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &_buffer);
	}

	void Buffer::Bind(GLenum target)
	{
		glBindBuffer(target, _buffer);
		CheckGLError();
	}

	void Buffer::BufferData(GLenum target, gl::BufferData data, GLenum usage)
	{
		glBufferData(target, data.Size, data.Data, usage);
		CheckGLError();
	}

}}
