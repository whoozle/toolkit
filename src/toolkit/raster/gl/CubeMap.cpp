#include <toolkit/raster/gl/CubeMap.h>

namespace TOOLKIT_NS { namespace gl
{

	CubeMap::CubeMap(raster::Size size): _size(size)
	{
	}

	CubeMap::~CubeMap()
	{}

	void CubeMap::Setup(GLint minFilter, GLint magFilter)
	{
		TOOLKIT_GL_CALL(glEnable(GL_TEXTURE_CUBE_MAP));
		TOOLKIT_GL_CALL(glActiveTexture(GL_TEXTURE0));
		TOOLKIT_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter));
		TOOLKIT_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter));
		TOOLKIT_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		TOOLKIT_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		TOOLKIT_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	}

	void CubeMap::Bind()
	{ TOOLKIT_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, _texture.Get())); }

	void CubeMap::Load(GLenum target, GLint level, GLint internalformat, GLenum format, GLenum type, ConstBuffer data)
	{
		if (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
			throw Exception("invalid target specified to Load");
		TOOLKIT_GL_CALL(glTexImage2D(target, level, internalformat, _size.Width, _size.Height, 0, format, type, data.data()));
	}

}}
