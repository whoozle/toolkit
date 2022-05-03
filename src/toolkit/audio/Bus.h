#ifndef TOOLKIT_AUDIO_BUS_H
#define TOOLKIT_AUDIO_BUS_H

#include <toolkit/audio/ISink.h>
#include <toolkit/audio/ISource.h>
#include <toolkit/audio/Mixer.h>
#include <vector>

namespace TOOLKIT_NS { namespace audio
{

	class Bus : public ISource
	{
	private:
		float		_tempo;
		int			_beats;
		int			_unit;
		int			_beat;
		float 		_time;
		float		_timeToClick;

		Mixer		_mixer;
		std::vector<ISinkPtr> _sinks;

	private:
		void Update()
		{
			if (_tempo <= 0)
				throw Exception("tempo must be positive, your music not necessarily");

			auto phase = _time / _timeToClick;
			_timeToClick = 60.0f / _tempo / _unit;
			_time = phase * _timeToClick;
		}

	public:
		Bus(float tempo, int beats, int unit):
			_tempo(tempo), _beats(beats), _unit(unit),
			_beat(0), _time(0), _timeToClick(1)
		{ Update(); }

		void Add(const ISourcePtr & source)
		{ _mixer.Add(source); }
		void Add(const ISinkPtr & sink)
		{ _sinks.push_back(sink); }

		void SetTempo(float bpm)
		{ _tempo = bpm; Update(); }
		float GetTempo() const
		{ return _tempo; }

		void SetSignature(int beats, int unit)
		{ _beats = beats; _unit = unit; Update(); }
		int GetBeats() const
		{ return _beats; }
		int GetBarUnit() const
		{ return _unit; }

		void Get(float dt, FloatBuffer buffer) override
		{
			size_t offset = 0;
			while(offset < buffer.size())
			{
				size_t nextBytes = std::min<size_t>(ceil((_timeToClick - _time) / dt), buffer.size() - offset);
				_mixer.Get(dt, FloatBuffer(buffer, offset, nextBytes));
				_time += nextBytes * dt;
				offset += nextBytes;
				while (_time >= _timeToClick)
				{
					_time -= _timeToClick;
					for(auto & sink : _sinks)
						sink->HandleBeat(_beat);
					_beat = (_beat + 1) % _beats;
				}
			}
		}
	};

}}

#endif
