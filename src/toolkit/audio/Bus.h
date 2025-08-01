#ifndef TOOLKIT_AUDIO_BUS_H
#define TOOLKIT_AUDIO_BUS_H

#include <toolkit/audio/BroadcastSink.h>
#include <toolkit/audio/ISource.h>
#include <toolkit/audio/Mixer.h>
#include <toolkit/log/Logger.h>

namespace TOOLKIT_NS { namespace audio
{

	class Bus : public ISource, public BroadcastSink
	{
		static log::Logger Log;

	private:
		float		_tempo;
		unsigned	_bar;
		unsigned	_beats;
		unsigned	_units;
		unsigned	_beat;
		float 		_time;
		float		_timeToClick;

		Mixer		_mixer;

	private:
		void Update();

	public:
		Bus(float tempo, int beats, int units);

		using BroadcastSink::Add;
		void Add(const ISourcePtr & source);

		void SetTempo(float bpm);
		float GetTempo() const;

		void SetSignature(unsigned beats, unsigned units);
		int GetBeats() const;
		int GetBarUnits() const;

		void Get(float dt, FloatBuffer buffer) override;
	};

}}

#endif
