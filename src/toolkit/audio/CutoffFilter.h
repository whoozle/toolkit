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

}}

#endif
