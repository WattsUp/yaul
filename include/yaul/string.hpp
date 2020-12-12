#ifndef _YAUL_STRING_HPP_
#define _YAUL_STRING_HPP_

#ifndef YAUL_TINYUTF8_EXTERNAL
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4127)  // Conditional expression is constant
#endif

#include "bundled/tinyutf8.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#else /* YAUL_TINYUTF8_EXTERNAL */
#include "tinyutf8.h"
#endif /* YAUL_TINYUTF8_EXTERNAL */

namespace yaul {
using string = tiny_utf8::string;
}

#endif /* _YAUL_STRING_HPP_ */