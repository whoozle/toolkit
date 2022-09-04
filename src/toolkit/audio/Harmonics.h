#ifndef TOOLKIT_AUDIO_HARMONICS_H
#define TOOLKIT_AUDIO_HARMONICS_H

#include <toolkit/audio/ISource.h>
#include <toolkit/audio/BaseOscillator.h>
#include <toolkit/audio/Mixer.h>
#include <initializer_list>

namespace TOOLKIT_NS { namespace audio
{

	template<typename OscillatorType>
	class Harmonics final : public ISourceWithBaseFrequency
	{
		using OscillatorTypePtr = std::shared_ptr<OscillatorType>;

		Mixer								_mixer;
		std::vector<OscillatorTypePtr>		_sources;
		float								_freq;

	public:
		Harmonics(std::initializer_list<float> volumes, float freq, float volume = BaseOscillator::DefaultVolume)
		{
			int h = 1;
			_sources.reserve(volumes.size());
			for(float srcVolume : volumes)
			{
				auto source = std::make_shared<OscillatorType>(freq * h++, srcVolume * volume);
				_sources.push_back(source);
				_mixer.Add(source);
			}
		}

		void SetFrequency(float freq) override
		{
			_freq = freq;
			int h = 1;
			for(auto & source : _sources)
				source->SetFrequency(freq * h++);
		}

		float GetFrequency() const override
		{ return _freq; }

		void Get(float dt, FloatBuffer buffer) override
		{ return _mixer.Get(dt, buffer); }
	};

}}

#endif
