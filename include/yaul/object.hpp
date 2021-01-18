#ifndef _YAUL_OBJECT_HPP_
#define _YAUL_OBJECT_HPP_

#include <yaul/common.hpp>
#include <yaul/pointers.hpp>

namespace yaul {

class YAUL_API Object {
 public:
  /**
   * @brief Construct a new base pImpl object
   *
   */
  Object() noexcept;

  YAUL_DEFINE_DESTRUCT_MOVE_COPY(Object)

  class Impl;

 protected:
  YAUL_DEFINE_INHERIT(Object);

  template <typename T>
  [[nodiscard]] inline T* impl() const noexcept {
    return dynamic_cast<T*>(pImpl.get());
  }

 private:
  ptr::Scoped<Impl> pImpl;
};

}  // namespace yaul

#endif /* _YAUL_OBJECT_HPP_ */