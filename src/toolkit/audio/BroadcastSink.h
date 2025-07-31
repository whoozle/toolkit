#ifndef TOOLKIT_AUDIO_BROADCASTSINK_H
#define TOOLKIT_AUDIO_BROADCASTSINK_H

#include <toolkit/audio/ISink.h>
#include <vector>

namespace TOOLKIT_NS { namespace audio
{

	class BroadcastSink : public ISink
	{
		std::vector<ISinkPtr> _sinks;

	public:
		void Add(ISinkPtr sink)
		{ _sinks.push_back(std::move(sink)); }

		void HandleBeat(int beat) override
		{
			for(auto &sink : _sinks)
				sink->HandleBeat(beat);
		}

		void HandlePress(const KeyEvent &key) override
		{
			for(auto &sink : _sinks)
				sink->HandlePress(key);
		}

		void HandleRelease(const KeyEvent &key) override
		{
			for(auto &sink : _sinks)
				sink->HandlePress(key);
		}
	};
	TOOLKIT_DECLARE_PTR(BroadcastSink);

}}

#endif
