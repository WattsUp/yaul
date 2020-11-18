#ifndef _YAUL_H_
#define _YAUL_H_

#ifdef _MSC_VER
#if YAUL_CREATE_SHARED_LIBRARY
#define YAUL_API __declspec(dllexport)
#elif YAUL_LINKED_AS_SHARED_LIBRARY
#define YAUL_API __declspec(dllimport)
#endif
#elif __GNUC__ >= 4 || defined(__clang__)
#define YAUL_API __attribute__((visibility("default")))
#endif /* _MSC_VER */

#ifndef YAUL_API
#define YAUL_API
#endif /* YAUL_API */

/* stdint.h is not available on older MSVC */
#if defined(_MSC_VER) && (_MSC_VER < 1600) && (!defined(_STDINT)) && \
    (!defined(_STDINT_H))
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
#else
#include <stdint.h>
#endif

/**
 * @brief Add two numbers using yaul
 *
 * @param a
 * @param b
 * @return uint32_t a + b
 */
uint32_t YAUL_API yaulAdd(uint32_t a, uint32_t b);

#endif /* _YAUL_H_ */