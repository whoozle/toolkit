#include <toolkit/raster/gl/Program.h>
#include <toolkit/raster/gl/Utils.h>
#include <toolkit/raster/gl/Shader.h>
#include <vector>

namespace TOOLKIT_NS { namespace gl
{
	Program::Program() : _program(glCreateProgram())
	{
		CheckGLError();
		if (!_program)
			throw std::runtime_error("glCreateProgram failed");
	}

	void Program::Attach(Shader &shader)
	{
		glAttachShader(_program, shader._shader);
		CheckGLError();
	}

	void Program::Detach(Shader &shader)
	{
		glDetachShader(_program, shader._shader);
		CheckGLError();
	}

	void Program::Link()
	{
		glLinkProgram(_program);
		GLint status;
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if (!status)
			throw std::runtime_error("link: " + GetLog());
	}

	std::string Program::GetLog() const
	{
		std::vector<char> log;
		GLint log_len;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &log_len);
		if(log_len > 1)
		{
			GLsizei written;
			log.resize(log_len);
			glGetProgramInfoLog(_program, log_len, &written, log.data());
		}
		return std::string(log.begin(), log.end());
	}

	void Program::Use()
	{
		glUseProgram(_program);
		CheckGLError();
	}

	void Program::BindAttributeLocation(int index, const std::string &name)
	{
		glBindAttribLocation(_program, index, name.c_str());
		CheckGLError();
	}

	int Program::GetAttributeLocation(const std::string &name) const
	{
		int loc = glGetAttribLocation(_program, name.c_str());
		CheckGLError();
		if (loc == -1)
			throw std::runtime_error("glGetAttribLocation " + name + " failed");
		return loc;
	}

	int Program::GetUniformLocation(const std::string &name) const
	{
		int loc = glGetUniformLocation(_program, name.c_str());
		CheckGLError();
		if (loc == -1)
			throw std::runtime_error("glGetUniformLocation " + name + " failed");
		return loc;
	}

	Program::~Program()
	{
		glDeleteProgram(_program);
	}
}}


