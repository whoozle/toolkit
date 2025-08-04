#ifndef TOOLKIT_AUDIO_ARPEGGIATOR_H
#define TOOLKIT_AUDIO_ARPEGGIATOR_H

#include <toolkit/audio/Scale.h>
#include <toolkit/audio/ITuning.h>
#include <toolkit/audio/ISink.h>
#include <toolkit/log/Logger.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{

	struct IArpeggiatorMode
	{
		virtual ~IArpeggiatorMode() = default;
		virtual unsigned StepCount() const = 0;
		virtual Key GetStep(Key base, int idx) const = 0;
	};
	TOOLKIT_DECLARE_PTR(IArpeggiatorMode);

	enum ArpeggiatorSequenceCommand
	{
		Ignore,
		Play,
		PlayNoNext
	};

	struct IArpeggiatorSequenceGenerator
	{
		virtual ~IArpeggiatorSequenceGenerator() = default;
		virtual ArpeggiatorSequenceCommand GenerateEvent(const BeatEvent & beat, float &pressure) = 0;
	};
	TOOLKIT_DECLARE_PTR(IArpeggiatorSequenceGenerator);

	class BaseArpeggiatorMode : public IArpeggiatorMode
	{
	protected:
		ScalePtr _scale;
		std::vector<int> _perm;
	public:
		BaseArpeggiatorMode(ScalePtr scale);
		unsigned StepCount() const override;
		Key GetStep(Key base, int idx) const override;
	};

	struct ArpeggiatorModeUp : public BaseArpeggiatorMode
	{
		ArpeggiatorModeUp(ScalePtr scale);
	};
	struct ArpeggiatorModeDown : public BaseArpeggiatorMode
	{
		ArpeggiatorModeDown(ScalePtr scale);
	};
	struct ArpeggiatorModeConverge : public BaseArpeggiatorMode
	{
		ArpeggiatorModeConverge(ScalePtr scale);
	};
	struct ArpeggiatorModeDiverge : public BaseArpeggiatorMode
	{
		ArpeggiatorModeDiverge(ScalePtr scale);
	};
	struct ArpeggiatorModeRandom : public BaseArpeggiatorMode
	{
		ArpeggiatorModeRandom(ScalePtr scale);
	};

	struct Arpeggiator : public ISink
	{
		static log::Logger	Log;
		ISinkPtr			_sink;
		IArpeggiatorModePtr _mode;
		IArpeggiatorSequenceGeneratorPtr _generator;
		KeyEvent			_base;
		bool				_active = false;
		unsigned			_step = 0;

	public:
		Arpeggiator(ISinkPtr sink, IArpeggiatorModePtr mode, IArpeggiatorSequenceGeneratorPtr seqGen);
		~Arpeggiator();

		void SetMode(IArpeggiatorModePtr mode);
		void SetSink(ISinkPtr sink);

		void HandleBeat(const BeatEvent &beat) override;
		void HandlePress(const KeyEvent &key) override;
		void HandleRelease(const KeyEvent &key) override;
	};

}}

#endif
