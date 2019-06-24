#include <toolkit/raster/gl/VertexArray.h>
#include <toolkit/raster/gl/Utils.h>

namespace TOOLKIT_NS { namespace gl
{
	VertexArray::VertexArray()
	{
		TOOLKIT_GL_CALL(glGenVertexArrays(1, &_array));
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_array);
	}

	void VertexArray::Bind()
	{
		TOOLKIT_GL_CALL(glBindVertexArray(_array));
	}

}}
