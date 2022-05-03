#ifndef TOOLKIT_AUDIO_NOISE_H
#define TOOLKIT_AUDIO_NOISE_H

#include <toolkit/audio/ISource.h>
#include <random>

namespace TOOLKIT_NS { namespace audio
{

	class Noise final : public ISource
	{
		std::default_random_engine	_engine;
		std::uniform_real_distribution<float> _dist;
		float 						_volume;

	public:
		Noise(float volume = 0.5f): _volume(volume)
		{ }

		void Get(float dt, FloatBuffer buffer) override
		{
			size_t n = buffer.size();
			auto * dst = buffer.data();
			while(n--)
				*dst++ = _volume * _dist(_engine);
		}
	};

}}

#endif
