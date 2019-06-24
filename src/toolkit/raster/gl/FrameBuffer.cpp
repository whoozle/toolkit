#include <toolkit/raster/gl/FrameBuffer.h>
#include <toolkit/raster/gl/Utils.h>

namespace TOOLKIT_NS { namespace gl
{
	FrameBuffer::FrameBuffer()
	{
		TOOLKIT_GL_CALL(glGenFramebuffers(1, &_fbo));
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &_fbo);
	}

	void FrameBuffer::Bind(GLenum target, GLuint fb)
	{
		TOOLKIT_GL_CALL(glBindFramebuffer(target, fb));
	}

	void FrameBuffer::Bind(GLenum target)
	{
		Bind(target, _fbo);
	}

}}
