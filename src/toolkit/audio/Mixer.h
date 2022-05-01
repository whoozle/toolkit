#ifndef TOOLKIT_AUDIO_MIXER_H
#define TOOLKIT_AUDIO_MIXER_H

#include <toolkit/audio/ISource.h>
#include <memory>
#include <vector>
#include <math.h>

namespace TOOLKIT_NS { namespace audio
{

	class Mixer final : public ISource
	{
		std::vector<ISourcePtr> _sources;
		std::vector<float>		_buffer;

	public:
		void Add(const ISourcePtr & source)
		{
			_sources.push_back(source);
		}

		void Get(float dt, FloatBuffer dst) override
		{
			size_t n = dst.size();
			if (_buffer.size() != n)
				_buffer.resize(n);
			for(size_t i = 0; i != _sources.size(); ++i)
			{
				if (i != 0)
				{
					_sources[i]->Get(dt, _buffer);
					for(size_t i = 0; i < n; ++i) {
						dst[i] += _buffer[i];
					}
				}
				else
					_sources[i]->Get(dt, dst);
			}

			float norm = 1.0f / (_sources.size());
			for(auto & sample : dst)
				sample *= norm;
		}
	};
	TOOLKIT_DECLARE_PTR(Mixer);

}}

#endif
