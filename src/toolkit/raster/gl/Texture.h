#ifndef TOOLKIT_RASTER_GL_TEXTURE_H
#define TOOLKIT_RASTER_GL_TEXTURE_H

#include <toolkit/raster/gl/Utils.h>
#include <toolkit/core/Noncopyable.h>

namespace TOOLKIT_NS { namespace gl
{

	class Texture : public Noncopyable
	{
	private:
		std::vector<GLuint> _textures;

	public:
		Texture(GLsizei n = 1);
		~Texture();

		GLuint Get(size_t idx = 0) const
		{ return _textures.at(idx); }

		void Bind(GLenum target, size_t idx = 0);
	};

}}

#endif
