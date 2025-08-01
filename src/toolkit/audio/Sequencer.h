#ifndef TOOLKIT_AUDIO_SEQUENCER_H
#define TOOLKIT_AUDIO_SEQUENCER_H

#include <toolkit/audio/ISink.h>
#include <functional>
#include <vector>

namespace TOOLKIT_NS { namespace audio
{

	class Sequencer : public ISink
	{
	public:
		using Callback = std::function<void (const BeatEvent &)>;

		Sequencer(Callback && callback) : _callback(std::move(callback))
		{ }

		void HandleBeat(const BeatEvent &beat) override
		{ _callback(beat); }

	private:
		Callback 		_callback;
	};

}}

#endif
