#ifndef _YAUL_LOG_H_
#define _YAUL_LOG_H_

#include "yaul.h"

namespace yaul {

void YAUL_API log(const char* s);

void YAUL_API configureLogging(const char* filename);

};  // namespace yaul

#endif /* _YAUL_LOG_H_ */