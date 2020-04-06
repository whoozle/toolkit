#ifndef TOOLKIT_RASTER_GL_UTILS_H
#define TOOLKIT_RASTER_GL_UTILS_H

#include <toolkit/core/core.h>
#include <toolkit/text/StringOutputStream.h>
#include <stdio.h>
#include <string>
#include <stdexcept>

#define TOOLKIT_GL_GL 1
#define TOOLKIT_GL_GLES 2
#define TOOLKIT_GL_GLES2 3

#if TOOLKIT_GL_VARIANT == TOOLKIT_GL_GLES2
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#elif TOOLKIT_GL_VARIANT == TOOLKIT_GL_GLES
#	include <GLES/gl.h>
#	include <GLES/glext.h>
#elif TOOLKIT_GL_VARIANT == TOOLKIT_GL_GL
#	include <GL/gl.h>
#	include <GL/glext.h>
#else
#	error unknown TOOLKIT_GL_VARIANT
#endif

#if TOOLKIT_GL_VARIANT == TOOLKIT_GL_GLES || TOOLKIT_GL_VARIANT == TOOLKIT_GL_GLES2
#	define	glClearDepth			glClearDepthf
#	define	glDeleteVertexArrays	glDeleteVertexArraysOES
#	define	glGenVertexArrays		glGenVertexArraysOES
#	define	glBindVertexArray		glBindVertexArrayOES
#	define	glMapBuffer				glMapBufferOES
#	define	glUnmapBuffer   		glUnmapBufferOES
#	define	GL_DEPTH24_STENCIL8		GL_DEPTH24_STENCIL8_OES
#	define	GL_WRITE_ONLY			GL_WRITE_ONLY_OES
#	define	GL_GLEXT_PROTOTYPES		1
#	define	GL_BGRA					0x80E1
#endif

namespace TOOLKIT_NS { namespace gl
{
	inline void CheckGLError(const char * file, int line, const char * what)
	{
		GLenum error = glGetError();
		if (error == GL_NO_ERROR)
			return;

		text::StringOutputStream ss;
		ss << file << ":" << line << ": " << what << ": ";
		while(error != GL_NO_ERROR)
		{
			ss << "OpenGL error " << error << " ";

			error = glGetError();
		}
		throw std::runtime_error(ss.Get());
	}

#define TOOLKIT_GL_CALL(...) \
	do { __VA_ARGS__ ; TOOLKIT_NS :: gl :: CheckGLError( __FILE__, __LINE__, #__VA_ARGS__ ); } while(false)

}}

#endif
