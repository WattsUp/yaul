#ifndef _YAUL_OBJECT_HPP_
#define _YAUL_OBJECT_HPP_

#include <yaul/common.hpp>

#define YAUL_DEFINE_DESTRUCT(x) virtual ~x() noexcept;
#define YAUL_DEFINE_MOVE(x) \
  x(x&& o) noexcept;        \
  x& operator=(x&& o) noexcept;
#define YAUL_DEFINE_COPY(x) \
  x(const x& o) noexcept;   \
  x& operator=(const x& o) noexcept;
#define YAUL_DEFINE_INHERIT(x) x(Impl& p) noexcept;

#define YAUL_DEFINE_DESTRUCT_MOVE_COPY(x) \
  YAUL_DEFINE_DESTRUCT(x) YAUL_DEFINE_MOVE(x) YAUL_DEFINE_COPY(x)

#define YAUL_IMPL_DESTRUCT(x) \
  x::~x() noexcept {}
#define YAUL_IMPL_MOVE(x, base)                \
  x::x(x&& o) noexcept : base(std::move(o)) {} \
  x& x::operator=(x&& o) noexcept {            \
    base::operator=(std::move(o));             \
    return *this;                              \
  }
#define YAUL_IMPL_COPY(x, base)                                \
  x::x(const x& o) noexcept                                    \
      : base(*new x::Impl(*static_cast<x::Impl*>(o.pImpl))) {} \
  x& x::operator=(const x& o) noexcept {                       \
    base::operator=(o);                                        \
    return *this;                                              \
  }
#define YAUL_IMPL_INHERIT(x) \
  x::x(x::Impl& p) noexcept : pImpl(&p) {}

#define YAUL_IMPL_DESTRUCT_MOVE_COPY(x, base) \
  YAUL_IMPL_DESTRUCT(x) YAUL_IMPL_MOVE(x, base) YAUL_IMPL_COPY(x, base)

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

  Impl* pImpl = nullptr;
};

}  // namespace yaul

#endif /* _YAUL_OBJECT_HPP_ */