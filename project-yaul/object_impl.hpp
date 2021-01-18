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
  Impl(Impl&& o) noexcept  = default;
  Impl& operator=(Impl&& o) noexcept = default;
  Impl(const Impl& o) noexcept       = default;
  Impl& operator=(const Impl& o) noexcept = default;

 private:
};

}  // namespace yaul

#endif /* _YAUL_OBJECT_IMPL_HPP_ */