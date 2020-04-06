#ifndef TOOLKIT_RASTER_GL_PROGRAM_H
#define TOOLKIT_RASTER_GL_PROGRAM_H

#include <toolkit/raster/gl/Utils.h>
#include <toolkit/core/Noncopyable.h>
#include <string>

namespace TOOLKIT_NS { namespace gl
{
	class Shader;
	class Program : public Noncopyable
	{
	public:
		Program();
		~Program();

		void Attach(Shader &shader);
		void Detach(Shader &shader);

		void BindAttributeLocation(int index, const std::string &name);
		int GetAttributeLocation(const std::string &name) const;

		int GetUniformLocation(const std::string &name) const;

		void Link();
		std::string GetLog() const;

		void Use();

	private:
		GLuint _program;
	};
	TOOLKIT_DECLARE_PTR(Program);

}}

#endif

