#include <toolkit/audio/Arpeggiator.h>
#include <numeric>
#include <random>

namespace TOOLKIT_NS { namespace audio
{
	BaseArpeggiatorMode::BaseArpeggiatorMode(ScalePtr scale):
		_scale(std::move(scale))
	{ _perm.resize(_scale->Pitches.size()); }

	unsigned BaseArpeggiatorMode::StepCount() const
	{ return _scale->Pitches.size(); }

	Key BaseArpeggiatorMode::GetStep(Key base, int idx) const
	{
		int n = _scale->Pitches.size();
		idx %= n;
		if (idx < 0)
			idx += n;
		base.Tone += _scale->Pitches.at(_perm.at(idx));
		return base;
	}

	ArpeggiatorModeUp::ArpeggiatorModeUp(ScalePtr scale): BaseArpeggiatorMode(std::move(scale))
	{
		std::iota(_perm.begin(), _perm.end(), 0);
	}

	ArpeggiatorModeDown::ArpeggiatorModeDown(ScalePtr scale): BaseArpeggiatorMode(std::move(scale))
	{
		for(size_t i = 0, n = _perm.size(); i != n; ++i)
			_perm[i] = n - 1 - i;
	}

	ArpeggiatorModeConverge::ArpeggiatorModeConverge(ScalePtr scale): BaseArpeggiatorMode(std::move(scale))
	{
		// 0, n - 1, 1, n - 2, 2 ...
		int lo = 0;
		int hi = _perm.size() - 1;
		for(size_t i = 0, n = _perm.size(); i != n; )
		{
			_perm[i++] = lo++;
			if (i == n)
				break;
			_perm[i++] = hi--;
		}
	}

	ArpeggiatorModeDiverge::ArpeggiatorModeDiverge(ScalePtr scale): BaseArpeggiatorMode(std::move(scale))
	{
		// reverse of converge
		int lo = 0;
		int hi = _perm.size() - 1;
		for(size_t i = 0, n = _perm.size(); i != n; )
		{
			_perm[n - 1 - i++] = lo++;
			if (i == n)
				break;
			_perm[n - 1 - i++] = hi--;
		}
	}

	ArpeggiatorModeRandom::ArpeggiatorModeRandom(ScalePtr scale): BaseArpeggiatorMode(std::move(scale))
	{
		std::minstd_rand rand;
		std::iota(_perm.begin(), _perm.end(), 0);
		size_t n = _perm.size();
		if (n < 2)
			return;

		for(size_t i = 0; i <= n - 2; ++i)
		{
			std::uniform_int_distribution<size_t> uni(i, n - 1);
			auto j = uni(rand);
			std::swap(_perm[i], _perm[j]);
		}
	}

	log::Logger Arpeggiator::Log("arpeggiator");

	Arpeggiator::Arpeggiator(ISinkPtr sink, IArpeggiatorModePtr mode):
		_sink(std::move(sink)),
		_mode(std::move(mode))
	{ }

	Arpeggiator::~Arpeggiator()
	{ }

	void Arpeggiator::SetSink(ISinkPtr sink)
	{ _sink = sink; }

	void Arpeggiator::SetMode(IArpeggiatorModePtr mode)
	{ _mode = mode; }

	void Arpeggiator::HandleBeat(int beat)
	{
		if (_active)
		{
			if (beat == 0)
			{
				KeyEvent event = {_mode->GetStep(_base.Key, _step++), _base.Pressure};
				Log.Debug() << event;
				_sink->HandlePress(std::move(event));
			}
		}
	}

	void Arpeggiator::HandlePress(const KeyEvent &key)
	{
		_base = key;
		_active = true;
	}

	void Arpeggiator::HandleRelease(const KeyEvent &key)
	{ _active = false; }

}}
