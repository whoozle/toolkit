#ifndef TOOLKIT_GLES_FRAMEBUFFER_H
#define TOOLKIT_GLES_FRAMEBUFFER_H

#include <toolkit/raster/gl/Utils.h>
#include <toolkit/core/Noncopyable.h>

namespace TOOLKIT_NS { namespace gl
{
	class FrameBuffer : public Noncopyable
	{
		GLuint		_fbo;

	public:
		FrameBuffer();
		~FrameBuffer();

		void Bind(GLenum target);
		static void Bind(GLenum target, GLuint fb);
	};
	TOOLKIT_DECLARE_PTR(FrameBuffer);

}}

#endif
