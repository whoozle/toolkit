#ifndef TOOLKIT_AUDIO_ETTUNING_H
#define TOOLKIT_AUDIO_ETTUNING_H

#include <toolkit/audio/ITuning.h>
#include <cmath>

namespace TOOLKIT_NS { namespace audio
{

	class ETTuning final : public ITuning
	{
		int		_parts;
		float 	_standardPitch;
		Key		_standardPitchNote;

	public:
		ETTuning(int parts = 12, float standardPitch = 440.0f, Key standardPitchNote = { 4, 9 }):
			_parts(parts), _standardPitch(standardPitch), _standardPitchNote(standardPitchNote)
		{ }

		int GetIndex(Key key) const
		{
			return (key.Octave - _standardPitchNote.Octave) * _parts + key.Tone - _standardPitchNote.Tone;
		}

		float GetFrequency(Key key) const override
		{
			float index = GetIndex(key);
			return _standardPitch * std::pow(2, index / _parts);
		}
	};

}}

#endif
