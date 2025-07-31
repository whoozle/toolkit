#ifndef TOOLKIT_AUDIO_ETTUNING_H
#define TOOLKIT_AUDIO_ETTUNING_H

#include <toolkit/audio/ITuning.h>
#include <cmath>

namespace TOOLKIT_NS { namespace audio
{

	class ETTuning final : public ITuning
	{
		float _standardPitch;
		float _parts;

	public:
		ETTuning(float standardPitch = 440.0f, float parts = 12.0f):
			_standardPitch(standardPitch), _parts(parts)
		{ }

		float GetFrequency(Key key) const override
		{ return _standardPitch * std::pow(2, key.Octave + (key.Tone / _parts)); }
	};

}}

#endif
