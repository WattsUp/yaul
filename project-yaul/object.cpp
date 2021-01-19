#include "object_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

/******************************** Public Class ********************************/

Object::Object() noexcept : pImpl(std::make_unique<Impl>()) {}

Object::~Object() noexcept = default;

Object::Object(Object&& o) noexcept : pImpl(std::move(o.pImpl)) {}

Object& Object::operator=(Object&& o) noexcept {
  if (this != &o) {
    pImpl = std::move(o.pImpl);
  }
  return *this;
}

Object::Object(const Object& o) noexcept
    : pImpl(std::make_unique<Impl>(*o.pImpl)) {}

Object& Object::operator=(const Object& o) noexcept {
  if (this != &o) {
    pImpl = std::make_unique<Impl>(*o.pImpl);
  }
  return *this;
}

Object::Object(std::unique_ptr<Impl> p) noexcept : pImpl(std::move(p)) {}

SharedObject::SharedObject() noexcept : pImpl(std::make_shared<Impl>()) {}

SharedObject::~SharedObject() noexcept = default;

SharedObject::SharedObject(SharedObject&& o) noexcept
    : pImpl(std::move(o.pImpl)) {}

SharedObject& SharedObject::operator=(SharedObject&& o) noexcept {
  if (this != &o) {
    pImpl = std::move(o.pImpl);
  }
  return *this;
}

// Copy pointer so shared_ptr has another owner
SharedObject::SharedObject(const SharedObject& o) noexcept = default;

// Copy pointer so shared_ptr has another owner
SharedObject& SharedObject::operator=(const SharedObject& o) noexcept = default;

void SharedObject::setImpl(std::shared_ptr<Impl> p) noexcept {
  pImpl = std::move(p);
}

SharedObject::SharedObject(std::shared_ptr<Impl> p) noexcept
    : pImpl(std::move(p)) {}

}  // namespace yaul