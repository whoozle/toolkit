#ifndef TOOLKIT_TOKEN_H
#define TOOLKIT_TOKEN_H

#include <toolkit/core/Counter.h>

namespace TOOLKIT_NS
{

	struct IToken : Counter<IToken>
	{
		virtual ~IToken() { }
	};
	DECLARE_COUNTER_PTR(IToken);

	class Token
	{
		ITokenPtr _token;

	public:
		Token() : _token() { }
		Token(const ITokenPtr & token) : _token(token) { }

		explicit operator bool () const
		{ return (bool)_token; }
	};

}

#endif
