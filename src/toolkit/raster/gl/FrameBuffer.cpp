#include <toolkit/raster/gl/FrameBuffer.h>
#include <toolkit/raster/gl/Utils.h>

namespace TOOLKIT_NS { namespace gl
{
	FrameBuffer::FrameBuffer()
	{
		glGenFramebuffers(1, &_fbo);
		CheckGLError();
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &_fbo);
	}

	void FrameBuffer::Bind(GLenum target, GLuint fb)
	{
		glBindFramebuffer(target, fb);
		CheckGLError();
	}

	void FrameBuffer::Bind(GLenum target)
	{
		Bind(target, _fbo);
	}

}}
