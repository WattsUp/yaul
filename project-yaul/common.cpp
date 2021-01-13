#include <yaul/common.hpp>

#include "common/version.h"

namespace yaul {

YAUL_API const char* getVersionString() {
  return VERSION_STRING;
}

YAUL_API const char* getVersionStringFull() {
  return VERSION_STRING_FULL;
}

}  // namespace yaul