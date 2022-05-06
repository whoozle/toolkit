#ifndef TOOLKIT_AUDIO_CUTOFF_FILTER_H
#define TOOLKIT_AUDIO_CUTOFF_FILTER_H

#include <toolkit/audio/IFilter.h>
#include <assert.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class CutoffFilter : public IFilterWithCutoffFrequency
	{
	private:
		float _cutoff;

	protected:
		float _rc;

	public:
		CutoffFilter(float cutoff)
		{ SetCutoff(cutoff); }

		float GetCutoff() const
		{ return _cutoff; }

		void SetCutoff(float cutoff)
		{
			_cutoff = cutoff;
			_rc = 1 / (M_PI * 2 * cutoff);
		}
	};

	class LowPassFilter final : public CutoffFilter
	{
	private:
		float _prev = 0;

	public:
		using CutoffFilter::CutoffFilter;

		void Process(float dt, FloatBuffer dstBuffer, ConstFloatBuffer srcBuffer) override
		{
			float a = dt / (_rc + dt);
			size_t n = dstBuffer.size();
			assert(n == srcBuffer.size());
			auto * src = srcBuffer.data();
			auto * dst = dstBuffer.data();
			while(n--)
			{
				_prev += a * (*src++ - _prev);
				*dst++ = _prev;
			}
		}
	};

	class HighPassFilter final : public CutoffFilter
	{
	private:
		float _prevOutput = 0;
		float _prevInput = 0;

	public:
		using CutoffFilter::CutoffFilter;

		void Process(float dt, FloatBuffer dstBuffer, ConstFloatBuffer srcBuffer) override
		{
			float a = _rc / (_rc + dt);
			size_t n = dstBuffer.size();
			assert(n == srcBuffer.size());
			auto * src = srcBuffer.data();
			auto * dst = dstBuffer.data();
			while(n--)
			{
				auto input = *src++;
				*dst++ = _prevOutput = a * (_prevOutput + input - _prevInput);
				_prevInput = input;
			}
		}
	};

}}

#endif
