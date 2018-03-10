#ifndef TOOLKIT_CORE_H
#define TOOLKIT_CORE_H

#include <toolkit/core/config.h>

#define TOOLKIT_NS_BEGIN namespace TOOLKIT_NS {
#define TOOLKIT_NS_END }

#define TOOLKIT_SUBNS_BEGIN(NAME) namespace TOOLKIT_NS { namespace NAME {
#define TOOLKIT_SUBNS_END }}

#define TOOLKIT_SUBNS2_BEGIN(NAME, NAME2) namespace TOOLKIT_NS { namespace NAME { namespace NAME2 {
#define TOOLKIT_SUBNS2_END }}}

#endif
