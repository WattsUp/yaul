#include "object_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

/******************************** Public Class ********************************/

Object::Object() noexcept : pImpl(new Object::Impl()) {}

Object::~Object() noexcept = default;

Object::Object(Object&& o) noexcept {
  pImpl.swap(o.pImpl);
}

Object& Object::operator=(Object&& o) noexcept {
  if (this != &o) {
    pImpl.swap(o.pImpl);
  }
  return *this;
}

Object::Object(const Object& o) noexcept : pImpl(new Object::Impl(*o.pImpl)) {}

Object& Object::operator=(const Object& o) noexcept {
  if (this != &o) {
    // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
    pImpl.reset(new Object::Impl(*o.pImpl));
  }
  return *this;
}

Object::Object(Object::Impl& p) noexcept : pImpl(&p) {}

}  // namespace yaul