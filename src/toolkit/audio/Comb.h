#ifndef TOOLKIT_AUDIO_COMB_FILTER_H
#define TOOLKIT_AUDIO_COMB_FILTER_H

#include <toolkit/audio/IFilter.h>
#include <deque>
#include <assert.h>

namespace TOOLKIT_NS { namespace audio
{

	class Comb final : public IFilter
	{
		float _delay;
		float _gain;
		std::deque<float> _buffer;

	public:
		Comb(float delay, float gain = 0.5f):
			_delay(delay), _gain(gain)
		{ }

		void Process(float dt, FloatBuffer dstBuffer, ConstFloatBuffer srcBuffer)
		{
			auto n = srcBuffer.size();
			assert(dst.size() == n);
			auto * dst = dstBuffer.data();
			auto * src = srcBuffer.data();
			size_t delay = _delay / dt;

			while(n--)
			{
				float v = *src++;
				if (_buffer.size() >= delay)
				{
					float prev = _buffer.front();
					_buffer.pop_front();
					v += _gain * prev;
				}
				_buffer.push_back(v);
				*dst++ = v;
			}
		}
	};

}}

#endif
