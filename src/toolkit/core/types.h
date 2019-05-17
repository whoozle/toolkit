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

#define DECLARE_PTR(C) using C##Ptr = std::shared_ptr<C>
#define DECLARE_WEAK_PTR(C) using C##WeakPtr = std::weak_ptr<C>
#define DECLARE_CONST_PTR(C) using C##ConstPtr = std::shared_ptr<const C>
#define DECLARE_CONST_WEAK_PTR(C) using C##ConstWeakPtr = std::weak_ptr<const C>
#define DECLARE_UPTR(C) using C##UPtr = std::unique_ptr<C>

#define _FORWARD_DECLARE(NS, TYPE, NAME, DECL) namespace TOOLKIT_NS { namespace NS { TYPE NAME ; DECL } }

#define FORWARD_DECLARE_CLASS(NS, NAME) _FORWARD_DECLARE(NS, class, NAME, /**/ )
#define FORWARD_DECLARE_CLASS_PTR(NS, NAME) _FORWARD_DECLARE(NS, class, NAME, DECLARE_PTR(NAME) ; )
#define FORWARD_DECLARE_CLASS_WPTR(NS, NAME) _FORWARD_DECLARE(NS, class, NAME, DECLARE_PTR(NAME) ; DECLARE_WEAK_PTR(NAME) ;)
#define FORWARD_DECLARE_CLASS_CPTR(NS, NAME) _FORWARD_DECLARE(NS, class, NAME, DECLARE_PTR(NAME) ; DECLARE_CONST_PTR(NAME) ; )
#define FORWARD_DECLARE_INTERFACE(NS, NAME) _FORWARD_DECLARE(NS, struct, NAME, DECLARE_PTR(NAME) ; )
#define FORWARD_DECLARE_INTERFACE_CPTR(NS, NAME) _FORWARD_DECLARE(NS, struct, NAME, DECLARE_PTR(NAME) ; DECLARE_CONST_PTR(NAME) ; )


#endif
