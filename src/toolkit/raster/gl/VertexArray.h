#ifndef TOOLKIT_GLES_VERTEXARRAY_H
#define TOOLKIT_GLES_VERTEXARRAY_H

#include <toolkit/raster/gl/Utils.h>
#include <toolkit/core/Noncopyable.h>

namespace TOOLKIT_NS { namespace gl
{
	class VertexArray : public Noncopyable
	{
		GLuint		_array;

	public:
		VertexArray();
		~VertexArray();

		void Bind();
	};
	TOOLKIT_DECLARE_PTR(VertexArray);

}}

#endif
