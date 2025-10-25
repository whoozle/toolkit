#ifndef TOOLKIT_AUDIO_FFT_H
#define TOOLKIT_AUDIO_FFT_H

#include <cmath>
#include <complex>

namespace TOOLKIT_NS { namespace audio
{

	namespace impl
	{
		template<typename Type, unsigned N>
		class FFT
		{
			using Next = FFT<Type, N / 2>;

			static_assert(N != 0, "N must be greater than zero");
			static_assert(((N - 1) & N) == 0, "N must be a power of two");

			static constexpr Type Angle = Type(-2 * M_PI / N);
			static constexpr Type Angle2 = Type(-1 * M_PI / N);

		public:
			using ComplexType = std::complex<Type>;

			template<int Sign>
			static void Apply(ComplexType* data)
			{
				Next::template Apply<Sign>(data);
				Next::template Apply<Sign>(data + N / 2);
				auto sinA = std::sin(Angle);
				auto sinA2 = -std::sin(Angle2);
				ComplexType wp(-2 * sinA2 * sinA2, sinA * Sign);
				ComplexType w(1, 0);

				for (unsigned i = 0, j = N / 2; i < N / 2 ; ++i, ++j) {
					ComplexType temp = data[j] * w;

					data[j] = data[i] - temp;
					data[i] += temp;

					w += w * wp;
				}
			}
		};

		template<typename Type>
		class FFT<Type, 1u>
		{
		public:
			using ComplexType = std::complex<Type>;

			template<int Sign>
			static void Apply(ComplexType* data)
			{ }
		};

		template<typename Type>
		class FFT<Type, 2u>
		{
		public:
			using ComplexType = std::complex<Type>;

			template<int Sign>
			static void Apply(ComplexType* data)
			{
				ComplexType temp = data[1];

				data[1] = data[0] - temp;
				data[0] += temp;
			}
		};
	}

	template <typename Type, unsigned N>
	struct FFT
	{
		using ComplexType = std::complex<Type>;

		static void Forward(ComplexType * data)
		{
			Scramble(data);
			impl::FFT<Type, N>::template Apply<1>(data);
		}

		static void Inverse(ComplexType * data)
		{
			Scramble(data);
			impl::FFT<Type, N>::template Apply<-1>(data);
			for(unsigned i = 0; i < N; ++i)
				data[i] /= N;
		}

		static void Scramble(ComplexType *data)
		{
			unsigned j = 0;
			for(unsigned i = 0; i < N; ++i)
			{
				if (i > j)
					std::swap(data[i], data[j]);
				unsigned m = N / 2;
				while(j >= m && m >= 2)
				{
					j -= m;
					m >>= 1;
				}
				j += m;
			}
		}
	};
}}

#endif
