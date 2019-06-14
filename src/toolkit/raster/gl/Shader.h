#ifndef GHOST_BACKEND_GL_SHADER_H
#define	GHOST_BACKEND_GL_SHADER_H

#include <toolkit/raster/gl/Utils.h>
#include <toolkit/core/Noncopyable.h>
#include <string>

namespace TOOLKIT_NS { namespace gl
{
	class Shader : public Noncopyable
	{
	private:
		GLuint			_shader;
		friend class Program;

	public:
		Shader(GLenum type);

		void SetSource(const std::string &source);
		std::string GetSource() const;
		void Compile();
		std::string GetLog() const;

		~Shader();
	};
}}

#endif
