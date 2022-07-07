#ifndef TOOLKIT_CORE_H
#define TOOLKIT_CORE_H

#undef unix
#include <toolkit/core/config.h>

#ifdef __EXCEPTIONS
#	define TOOLKIT_EXCEPTIONS_ENABLED
#	define THROW(EXCEPTION, ...) throw EXCEPTION(__VA_ARGS__)
#	define TRY try
#	define CATCH(SPEC, ...) catch ( SPEC ) __VA_ARGS__
#	define THROW_ON_FAILURE(EXPR, EXCEPTION, ...) ASSERT(EXPR, EXCEPTION, __VA_ARGS__)
#else
#	include <stdlib.h>
#	define THROW(EXCEPTION, ...) { printf("%s\n", #__VA_ARGS__); abort(); }
#	define TRY /**/
#	define CATCH(SPEC, ...) /**/
#	define THROW_ON_FAILURE(EXPR, EXCEPTION, ...) (void)(EXPR)
#endif
#define ASSERT(EXPR, EXCEPTION, ...) if (!(EXPR)) THROW(EXCEPTION, __VA_ARGS__)

#ifdef __GXX_RTTI
#	define TOOLKIT_RTTI_ENABLED
#endif

#endif
