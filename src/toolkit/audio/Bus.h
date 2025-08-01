#ifndef TOOLKIT_AUDIO_BUS_H
#define TOOLKIT_AUDIO_BUS_H

#include <toolkit/audio/BroadcastSink.h>
#include <toolkit/audio/ISource.h>
#include <toolkit/audio/Mixer.h>
#include <cmath>

namespace TOOLKIT_NS { namespace audio
{

	class Bus : public ISource, public BroadcastSink
	{
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
		void Update()
		{
			if (_tempo <= 0)
				throw Exception("tempo must be positive, your music not necessarily");

			auto phase = _time / _timeToClick;
			_timeToClick = 60.0f / _tempo / _units;
			_time = phase * _timeToClick;
		}

	public:
		Bus(float tempo, int beats, int units):
			_tempo(tempo), _bar(0), _beats(beats), _units(units),
			_beat(0), _time(0), _timeToClick(1)
		{ Update(); }

		using BroadcastSink::Add;

		void Add(const ISourcePtr & source)
		{ _mixer.Add(source); }

		void SetTempo(float bpm)
		{ _tempo = bpm; Update(); }
		float GetTempo() const
		{ return _tempo; }

		void SetSignature(unsigned beats, unsigned units)
		{ _beats = beats; _units = units; Update(); }
		int GetBeats() const
		{ return _beats; }
		int GetBarUnits() const
		{ return _units; }

		void Get(float dt, FloatBuffer buffer) override
		{
			size_t offset = 0;
			while(offset < buffer.size())
			{
				size_t nextBytes = std::min<size_t>(std::ceil((_timeToClick - _time) / dt), buffer.size() - offset);
				_mixer.Get(dt, FloatBuffer(buffer, offset, nextBytes));
				_time += nextBytes * dt;
				offset += nextBytes;
				while (_time >= _timeToClick)
				{
					_time -= _timeToClick;
					HandleBeat(BeatEvent{_bar, _beat, _beats});
					_beat = (_beat + 1) % _beats;
					if (_beat == 0)
						++_bar;
				}
			}
		}
	};

}}

#endif
