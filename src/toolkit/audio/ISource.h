#ifndef TOOLKIT_AUDIO_ISOURCE_H
#define TOOLKIT_AUDIO_ISOURCE_H

#include <toolkit/audio/Buffer.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{

	struct ISource
	{
		virtual ~ISource() = default;

		virtual void Get(float dt, FloatBuffer buffer) = 0;
	};
	TOOLKIT_DECLARE_PTR(ISource);

}}

#endif
