#ifndef TOOLKIT_AUDIO_SOURCECONTROLSINK_H
#define TOOLKIT_AUDIO_SOURCECONTROLSINK_H

#include <toolkit/audio/ISink.h>
#include <toolkit/audio/ISource.h>
#include <toolkit/audio/IOscillator.h>
#include <toolkit/audio/ITuning.h>

namespace TOOLKIT_NS { namespace audio
{

	class SourceControlSink : public ISink
	{
		IOscillatorPtr _oscillator;
		ISourceWithBaseFrequencyPtr _source;
		ITuningPtr _tuning;

	public:
		SourceControlSink(ISourceWithBaseFrequencyPtr source, ITuningPtr tuning):
			_oscillator(std::dynamic_pointer_cast<IOscillator>(source)), _source(std::move(source)), _tuning(std::move(tuning))
		{ }

		void HandleBeat(int beat) override
		{ }

		void HandlePress(const KeyEvent &key) override
		{
			auto freq = _tuning->GetFrequency(key.Key);
			_source->SetFrequency(freq);
			if (_oscillator)
				_oscillator->SetVolume(key.Pressure);
		}

		void HandleRelease(const KeyEvent &key) override
		{ }
	};
	TOOLKIT_DECLARE_PTR(SourceControlSink);

}}

#endif
