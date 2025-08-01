#include <toolkit/audio/Bus.h>
#include <cmath>

namespace TOOLKIT_NS { namespace audio
{
	log::Logger Bus::Log("audio-bus");

	Bus::Bus(float tempo, int beats, int units):
		_tempo(tempo), _bar(0), _beats(beats), _units(units),
		_beat(0), _time(0), _timeToClick(1)
	{ Update(); }

	void Bus::Add(const ISourcePtr & source)
	{ _mixer.Add(source); }

	void Bus::SetTempo(float bpm)
	{ _tempo = bpm; Update(); }
	float Bus::GetTempo() const
	{ return _tempo; }

	void Bus::SetSignature(unsigned beats, unsigned units)
	{ _beats = beats; _units = units; Update(); }
	int Bus::GetBeats() const
	{ return _beats; }
	int Bus::GetBarUnits() const
	{ return _units; }

	void Bus::Update()
	{
		Log.Debug() << "setting signature to " << _beats << "/" << _units << ", tempo: " << _tempo;
		if (_tempo <= 0)
			throw Exception("tempo must be positive, your music not necessarily");

		auto phase = _time / _timeToClick;
		_timeToClick = 60.0f / _tempo / _units;
		_time = phase * _timeToClick;
	}

	void Bus::Get(float dt, FloatBuffer buffer)
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

}}
