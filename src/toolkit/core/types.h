#ifndef TOOLKIT_TYPES_H
#define TOOLKIT_TYPES_H

#include <toolkit/core/core.h>
#include <stdint.h>
#include <stddef.h>

namespace TOOLKIT_NS
{

	using uint	= unsigned int;
	using ulong	= unsigned long;

	using u8	= uint8_t;
	using u16	= uint16_t;
	using u32	= uint32_t;
	using u64	= uint64_t;

	using s8	= int8_t;
	using s16	= int16_t;
	using s32	= int32_t;
	using s64	= int64_t;

}

#define TOOLKIT_DECLARE_PTR(C) using C##Ptr = std::shared_ptr<C>
#define TOOLKIT_DECLARE_WEAK_PTR(C) using C##WeakPtr = std::weak_ptr<C>
#define TOOLKIT_DECLARE_CONST_PTR(C) using C##ConstPtr = std::shared_ptr<const C>
#define TOOLKIT_DECLARE_CONST_WEAK_PTR(C) using C##ConstWeakPtr = std::weak_ptr<const C>
#define TOOLKIT_DECLARE_UPTR(C) using C##UPtr = std::unique_ptr<C>

#define TOOLKIT_FORWARD_DECLARE(NS, TYPE, NAME, DECL) namespace TOOLKIT_NS { namespace NS { TYPE NAME ; DECL } }

#define TOOLKIT_FORWARD_DECLARE_CLASS(NS, NAME) TOOLKIT_FORWARD_DECLARE(NS, class, NAME, /**/ )
#define TOOLKIT_FORWARD_DECLARE_CLASS_PTR(NS, NAME) TOOLKIT_FORWARD_DECLARE(NS, class, NAME, TOOLKIT_DECLARE_PTR(NAME) ; )
#define TOOLKIT_FORWARD_DECLARE_CLASS_WEAK_PTR(NS, NAME) TOOLKIT_FORWARD_DECLARE(NS, class, NAME, TOOLKIT_DECLARE_PTR(NAME) ; TOOLKIT_DECLARE_WEAK_PTR(NAME) ;)
#define TOOLKIT_FORWARD_DECLARE_CLASS_CPTR(NS, NAME) TOOLKIT_FORWARD_DECLARE(NS, class, NAME, TOOLKIT_DECLARE_PTR(NAME) ; TOOLKIT_DECLARE_CONST_PTR(NAME) ; )
#define TOOLKIT_FORWARD_DECLARE_INTERFACE(NS, NAME) TOOLKIT_FORWARD_DECLARE(NS, struct, NAME, /**/ )
#define TOOLKIT_FORWARD_DECLARE_INTERFACE_PTR(NS, NAME) TOOLKIT_FORWARD_DECLARE(NS, struct, NAME, TOOLKIT_DECLARE_PTR(NAME) ; TOOLKIT_DECLARE_PTR(NAME) ; )
#define TOOLKIT_FORWARD_DECLARE_INTERFACE_CONST_PTR(NS, NAME) TOOLKIT_FORWARD_DECLARE(NS, struct, NAME, TOOLKIT_DECLARE_PTR(NAME) ; TOOLKIT_DECLARE_CONST_PTR(NAME) ; )

#ifdef __EXCEPTIONS
#	define TOOLKIT_EXCEPTIONS_ENABLED
#	define THROW(EXCEPTION, ...) throw EXCEPTION(__VA_ARGS__)
#	define ASSERT(EXPR, EXCEPTION, ...) if (!(EXPR)) THROW(EXCEPTIONS, __VA_ARGS__)
#	define TRY try
#	define CATCH(SPEC, ...) catch ( SPEC ) __VA_ARGS__
#else
#	include <assert.h>
#	include <stdlib.h>
#	define THROW(EXCEPTION, ...) abort()
#	define ASSERT(EXPR, EXCEPTION, ...) assert((EXPR))
#	define TRY /**/
#	define CATCH(SPEC, ...) /**/
#endif

#ifdef __GXX_RTTI
#	define TOOLKIT_RTTI_ENABLED
#endif

#endif
