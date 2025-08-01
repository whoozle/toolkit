#ifndef TOOLKIT_AUDIO_MDCT_H
#define TOOLKIT_AUDIO_MDCT_H

#include <toolkit/audio/FFT.h>
#include <array>
#include <cmath>
#include <ccomplex>
#include <functional>

namespace TOOLKIT_NS { namespace audio
{

	namespace window
	{
		template<typename Type>
		struct Vorbis
		{
			Type operator()(unsigned x, unsigned n) const
			{
				Type s = std::sin(Type(M_PI) * (x + Type(0.5)) / n);
				return std::sin(Type(M_PI_2) * s * s);
			}
		};
	}

	template<typename Type, unsigned N>
	class MDCT
	{
		static_assert(N > 4, "N must be a power of two and greater than 4");
		static constexpr unsigned N2 = N / 2;
		static constexpr unsigned N4 = N / 4;

		using ComplexType = std::complex<Type>;
		using FFTType = FFT<Type, N4>;

		std::array<Type, N> 			_windowFuncCache;
		std::array<ComplexType, N4> 	_angle;
		Type 							_sqrtN;

	public:
		MDCT(std::function<Type(unsigned, unsigned)> windowFunc):
			_sqrtN(std::sqrt(Type(N)))
		{
			for(unsigned i = 0; i != N4; ++i)
				_angle[i] = std::polar<Type>(1, 2 * Type(M_PI) * (i + Type(0.125)) / N);
			for(unsigned i = 0; i != N; ++i)
				_windowFuncCache[i] = windowFunc(i, N);
		}

		void Forward(Type *data) const
		{
			std::array<ComplexType, N4> fftData;

			std::array<Type, N> rotate; //FIXME : remove me
			for(unsigned t = 0; t < N4; ++t) {
				rotate[t] = -data[t + 3 * N4];
			}
			for(unsigned t = N4; t < N; ++t) {
				rotate[t] = data[t - N4];
			}

			for(unsigned t = 0; t < N4; ++t) {
				Type re = (rotate[t * 2] - rotate[N - 1 - t * 2]) / 2;
				Type im = (rotate[N2 + t * 2] - rotate[N2 - 1 - t * 2]) / -2;
				auto & a = _angle[t];
				fftData[t] = ComplexType(re * a.real() + im * a.imag(), -re * a.imag() + im * a.real());
			}

			FFTType::Forward(fftData.data());

			for(unsigned t = 0; t < N4; ++t) {
				auto & a = _angle[t];
				auto &f = fftData[t];
				f = ComplexType(
					2 / _sqrtN * (f.real() * a.real() + f.imag() * a.imag()),
					2 / _sqrtN * (-f.real() * a.imag() + f.imag() * a.real())
				);
			}

			for(unsigned t = 0; t < N4; ++t) {
				data[2 * t] = fftData[t].real();
				data[N2 - 2 * t - 1] = -fftData[t].imag();
			}
		}

		void Inverse(Type *data) const
		{
			std::array<ComplexType, N4> fftData;

			for(unsigned t = 0; t < N4; ++t) {
				Type re = data[t * 2] / 2, im = data[N2 - 1 - t * 2] / 2;
				auto & a = _angle[t];
				fftData[t] = ComplexType(re * a.real() + im * a.imag(), - re * a.imag() + im * a.real());
			}

			FFTType::Forward(fftData.data());

			for(unsigned t = 0; t < N4; ++t) {
				auto & a = _angle[t];
				auto & f = fftData[t];
				fftData[t] = ComplexType(
					8 / _sqrtN * (f.real() * a.real() + f.imag() * a.imag()),
					8 / _sqrtN * (-f.real() * a.imag() + f.imag() * a.real())
				);
			}

			std::array<Type, N> rotate; //FIXME : remove me
			for(unsigned t = 0; t < N4; ++t) {
				const auto &f = fftData[t];
				rotate[2 * t] = f.real();
				rotate[N2 + 2 * t] = f.imag();
			}
			for(unsigned t = 1; t < N; t += 2) {
				rotate[t] = - rotate[N - t - 1];
			}

			//shift
			for(unsigned t = 0; t < 3 * N4; ++t) {
				data[t] = rotate[t + N4];
			}
			for(unsigned t = 3 * N4; t < N; ++t) {
				data[t] = -rotate[t - 3 * N4];
			}
		}

		void ApplyWindow(Type *data) const
		{
			for(int i = 0; i < N; ++i) {
				data[i] *= _windowFuncCache[i];
			}
		}
	};
}}

#endif
