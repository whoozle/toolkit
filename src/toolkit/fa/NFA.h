#ifndef TOOLKIT_FA_NFA_H
#define TOOLKIT_FA_NFA_H

#include <toolkit/core/core.h>
#include <bitset>
#include <stddef.h>

namespace TOOLKIT_NS { namespace fa
{

	template<size_t Q, typename CharacterType = char>
	class NFA
	{
	public:
		static constexpr size_t Epsilon = 0;
		using SetType = std::bitset<Q + 1>;

		struct State
		{
			SetType Current;
		};

	private:
		SetType 	_transitions[Q];
		SetType		_accepting;

	public:
		NFA()
		{ }

		void Accept(size_t state)
		{
			if (state == Epsilon)
				throw std::runtime_error("invalid accepting state");

			_accepting.set(state);
		}

		State Initialize(size_t initialState)
		{
			if (initialState == Epsilon)
				throw std::runtime_error("invalid initial state");

			State state { };
			state.Current |= initialState;
			return state;
		}

		void Input(State & state, size_t ch)
		{
			auto & current = state.Current;

			SetType next;
			for(size_t state = 0; state < current.size(); ++state)
			{
				if (current[state])
				{
					next |= _transitions[state][ch];
					next |= _transitions[state][Epsilon];
				}
			}
			current = next;
		}
	};
}}

#endif
