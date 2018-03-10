#ifndef TOOLKIT_TYPES_H
#define TOOLKIT_TYPES_H

#include <stdint.h>
#include <toolkit/core/core.h>

TOOLKIT_NS_BEGIN

	typedef uint8_t		u8;
	typedef uint16_t	u16;
	typedef uint32_t	u32;
	typedef uint64_t	u64;

	typedef int8_t		s8;
	typedef int16_t		s16;
	typedef int32_t		s32;
	typedef int64_t		s64;

TOOLKIT_NS_END

#define DECLARE_PTR(C) using C##Ptr = std::shared_ptr<C>
#define DECLARE_CONST_PTR(C) using C##ConstPtr = std::shared_ptr<const C>
#define DECLARE_UPTR(C) using C##UPtr = std::unique_ptr<C>

#define _FORWARD_DECLARE(NS, TYPE, NAME, DECL) TOOLKIT_NS_BEGIN namespace NS { TYPE NAME ; DECL } TOOLKIT_NS_END

#define FORWARD_DECLARE_CLASS(NS, NAME) _FORWARD_DECLARE(NS, class, NAME, /**/ )
#define FORWARD_DECLARE_CLASS_PTR(NS, NAME) _FORWARD_DECLARE(NS, class, NAME, DECLARE_PTR(NAME) ; )
#define FORWARD_DECLARE_CLASS_CPTR(NS, NAME) _FORWARD_DECLARE(NS, class, NAME, DECLARE_PTR(NAME) ; DECLARE_CONST_PTR(NAME) ; )
#define FORWARD_DECLARE_INTERFACE(NS, NAME) _FORWARD_DECLARE(NS, struct, NAME, DECLARE_PTR(NAME) ; )
#define FORWARD_DECLARE_INTERFACE_CPTR(NS, NAME) _FORWARD_DECLARE(NS, struct, NAME, DECLARE_PTR(NAME) ; DECLARE_CONST_PTR(NAME) ; )


#endif
