#ifndef TOOLKIT_AUDIO_SCALE_H
#define TOOLKIT_AUDIO_SCALE_H

#include <toolkit/core/types.h>
#include <memory>
#include <vector>

namespace TOOLKIT_NS { namespace audio
{

	struct Scale
	{
		std::vector<int> Pitches;
	};
	TOOLKIT_DECLARE_PTR(Scale);

}}

#endif
