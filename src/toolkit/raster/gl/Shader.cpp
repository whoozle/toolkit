#include <toolkit/raster/gl/Shader.h>
#include <toolkit/raster/gl/Utils.h>
#include <vector>

namespace TOOLKIT_NS { namespace gl
{
	Shader::Shader(GLenum type)
	{
		TOOLKIT_GL_CALL(_shader = glCreateShader(type));
		if (!_shader)
			throw std::runtime_error("shader creation failed");
	}

	void Shader::SetSource(const std::string &source)
	{
		const char *s = source.c_str();
		GLint n = source.size();
		TOOLKIT_GL_CALL(glShaderSource(_shader, 1, &s, &n));
	}

	std::string Shader::GetSource() const
	{ return std::string(); }

	void Shader::Compile()
	{
		glCompileShader(_shader);
		GLint status;
		glGetShaderiv(_shader, GL_COMPILE_STATUS, &status);
		if (!status)
			throw std::runtime_error("compile: " + GetLog());
	}

	std::string Shader::GetLog() const
	{
		std::vector<char> log;
		GLint log_len;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &log_len);
		if(log_len > 1)
		{
			GLsizei written;
			log.resize(log_len);
			glGetShaderInfoLog(_shader, log_len, &written, log.data());
		}
		return std::string(log.begin(), log.end());
	}

	Shader::~Shader() {
		glDeleteShader(_shader);
	}
}}

