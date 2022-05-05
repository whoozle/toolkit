#ifndef TOOLKIT_AUDIO_POLYNOMIAL_OSCILLATOR_H
#define TOOLKIT_AUDIO_POLYNOMIAL_OSCILLATOR_H

#include <toolkit/audio/BaseOscillator.h>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class PolynomialOscillator final : public BaseOscillator
	{
	public:
		using Points = std::vector<std::pair<float, float>>;

	private:
		Points 					_points;

	private:
		void Sort()
		{ std::sort(_points.begin(), _points.end()); }

	public:
		PolynomialOscillator(float freq, float volume = DefaultVolume, const Points & points = Points()):
			BaseOscillator(freq, volume), _points(points)
		{ Sort(); }

		PolynomialOscillator(float freq, float volume, const Points && points = Points()):
			BaseOscillator(freq, volume), _points(std::move(points))
		{ Sort(); }

		void SetPoints(Points && points) {
			_points = std::move(points);
			Sort();
		}

		void SetPoints(const Points & points) {
			_points = points;
			Sort();
		}

		float Lagrange(float t, size_t j) const
		{
			float prod = 1;
			float xj = _points[j].first;
			for(size_t i = 0, n = _points.size(); i != n; ++i)
			{
				float p = _points[i].first;
				if (i != j)
					prod *= (t - p) / (xj - p);
			}
			return prod;
		}

		float Next(float dt) override
		{
			float sum = 0;
			float t = Tick(dt, 1.0f);
			for(size_t i = 0, n = _points.size(); i != n; ++i)
				sum += Lagrange(t, i) * _points[i].second;
			return sum * _volume;
		}

	};

}}

#endif
