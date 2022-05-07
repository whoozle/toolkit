#ifndef TOOLKIT_AUDIO_DELAY_H
#define TOOLKIT_AUDIO_DELAY_H

#include <toolkit/audio/IFilter.h>
#include <deque>

namespace TOOLKIT_NS { namespace audio
{

	class Delay final : public IFilter
	{
		float					_delay;
		float 					_volume;
		std::deque<float>		_buffer;

	public:
		Delay(float delay, float volume):
			_delay(delay), _volume(volume)
		{ }

		void Process(float dt, FloatBuffer dstBuffer, ConstFloatBuffer srcBuffer) override
		{
			size_t samples = _delay / dt;
			size_t n = dstBuffer.size();
			assert(n == srcBuffer.size());

			auto * dst = dstBuffer.data();
			auto * src = srcBuffer.data();
			while(n--)
			{
				auto sample = *src++;
				if (_buffer.size() < samples)
					_buffer.push_back(sample);

				if (_buffer.size() >= samples)
				{
					sample += _buffer.front() * _volume;
					_buffer.pop_front();
				}
				*dst++ = sample;
			}
		}
	};

}}

#endif
