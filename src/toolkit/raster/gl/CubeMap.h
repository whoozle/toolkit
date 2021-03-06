#ifndef TOOLKIT_RASTER_GL_CUBEMAP_H
#define TOOLKIT_RASTER_GL_CUBEMAP_H

#include <toolkit/raster/Size.h>
#include <toolkit/raster/gl/Utils.h>
#include <toolkit/raster/gl/Texture.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/Buffer.h>

namespace TOOLKIT_NS { namespace gl
{
	class CubeMap : public Noncopyable
	{
		gl::Texture 		_texture;
		raster::Size 		_size;

	public:
		CubeMap(raster::Size size);
		~CubeMap();

		gl::Texture & GetTexture() { return _texture; }
		const gl::Texture & GetTexture() const { return _texture; }

		static void Setup(GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);
		void Bind();
		void Load(GLenum target, GLint level, GLint internalformat, GLenum format, GLenum type, ConstBuffer data);
	};
	TOOLKIT_DECLARE_PTR(CubeMap);

}}

#endif
