#include <toolkit/raster/gl/VertexArray.h>
#include <toolkit/raster/gl/Utils.h>

namespace TOOLKIT_NS { namespace gl
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &_array);
		CheckGLError();
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_array);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(_array);
		CheckGLError();
	}

}}
