#ifndef TOOLKIT_AUDIO_IFILTER_H
#define TOOLKIT_AUDIO_IFILTER_H

#include <toolkit/audio/Buffer.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{

	struct IFilter
	{
		virtual ~IFilter() = default;

		virtual void Process(float dt, FloatBuffer dst, ConstFloatBuffer src) = 0;
	};
	TOOLKIT_DECLARE_PTR(IFilter);

}}

#endif
