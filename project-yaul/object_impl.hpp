#ifndef _YAUL_OBJECT_IMPL_HPP_
#define _YAUL_OBJECT_IMPL_HPP_

#include <yaul/object.hpp>

namespace yaul {

class Object::Impl {
 public:
  /**
   * @brief Construct a new Object implementation object with default values
   *
   */
  Impl() noexcept = default;

  virtual ~Impl() noexcept = default;

 private:
};

}  // namespace yaul

#endif /* _YAUL_OBJECT_IMPL_HPP_ */