#include <toolkit/raster/gl/Texture.h>

namespace TOOLKIT_NS { namespace gl
{
	Texture::Texture(GLsizei n): _textures(n)
	{
		TOOLKIT_GL_CALL(glGenTextures(n, _textures.data()));
	}

	void Texture::Bind(GLenum target, size_t idx)
	{
		TOOLKIT_GL_CALL(glBindTexture(target, Get(idx)));
	}

	Texture::~Texture()
	{ glDeleteTextures(_textures.size(), _textures.data()); }

}}
