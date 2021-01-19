#include "object_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

/******************************** Public Class ********************************/

Object::Object() noexcept : pImpl(ptr::Unique<Impl>::make()) {}

Object::~Object() noexcept = default;

Object::Object(Object&& o) noexcept : pImpl(std::move(o.pImpl)) {}

Object& Object::operator=(Object&& o) noexcept {
  if (this != &o) {
    pImpl = std::move(o.pImpl);
  }
  return *this;
}

Object::Object(const Object& o) noexcept
    : pImpl(ptr::Unique<Impl>::make(*o.pImpl)) {}

Object& Object::operator=(const Object& o) noexcept {
  if (this != &o) {
    pImpl = ptr::Unique<Impl>::make(*o.pImpl);
  }
  return *this;
}

Object::Object(ptr::Unique<Impl> p) noexcept : pImpl(std::move(p)) {}

}  // namespace yaul