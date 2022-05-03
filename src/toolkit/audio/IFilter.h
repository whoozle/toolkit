#ifndef TOOLKIT_AUDIO_IFILTER_H
#define TOOLKIT_AUDIO_IFILTER_H

#include <toolkit/audio/ISource.h>
#include <memory>

namespace TOOLKIT_NS { namespace audio
{

	struct IFilter
	{
		virtual ~IFilter() = default;

		virtual void Process(float dt, FloatBuffer dst, ConstFloatBuffer src) = 0;
	};
	TOOLKIT_DECLARE_PTR(IFilter);

	class Filter : public ISource
	{
	private:
		IFilterPtr		_filter;
		ISourcePtr		_source;

	public:
		Filter(const IFilterPtr &filter, const ISourcePtr & source):
			_filter(filter), _source(source)
		{ }

		void Get(float dt, FloatBuffer buffer) override
		{
			_source->Get(dt, buffer);
			_filter->Process(dt, buffer, buffer);
		}
	};
	TOOLKIT_DECLARE_PTR(Filter);

	template<typename FilterType, typename ... FilterArgs>
	FilterPtr MakeFilter(const ISourcePtr & src, FilterArgs ... args)
	{ return std::make_shared<Filter>(std::make_shared<FilterType>(std::forward<FilterArgs>(args)...), src); }

}}

#endif
