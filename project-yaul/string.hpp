#ifndef _YAUL_STRING_HPP_
#define _YAUL_STRING_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4127)  // Conditional expression is constant
#endif

#include <tinyutf8.h>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace yaul {

using string = tiny_utf8::utf8_string;

}

#endif /* _YAUL_STRING_HPP_ */