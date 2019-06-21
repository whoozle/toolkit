#ifndef TOOLKIT_SOFTWARERENDERERWITHCONTEXT_H
#define TOOLKIT_SOFTWARERENDERERWITHCONTEXT_H

#include <toolkit/raster/software/Renderer.h>

namespace TOOLKIT_NS { namespace raster { namespace software
{

	template<typename ContextType>
    class RendererWithContext : public Renderer
    {
    protected:
        using super = Renderer;
        ContextType & _context;

    public:
	    RendererWithContext(ContextType &ctx): _context(ctx) { }

		ContextType & GetContext()
		{ return _context; }

		const ContextType & GetContext() const
		{ return _context; }

		void Flip(raster::Rect rect)
		{ Blit(_context.GetFrontBuffer(), rect, rect.TopLeft(), _context.GetBackBuffer(), rect); }
    };

}}}

#endif
