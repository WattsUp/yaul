#ifndef _YAUL_OBJECT_HPP_
#define _YAUL_OBJECT_HPP_

#include <memory>
#include <yaul/common.hpp>

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

  /**
   * @brief Access the implementation object as a derived object
   *
   * @tparam T derived Impl typename
   * @return T* pointer to implementation (dynamically casted to derived object)
   */
  template <typename T>
  [[nodiscard]] inline T* impl() const noexcept {
    return dynamic_cast<T*>(pImpl.get());
  }

 private:
  std::unique_ptr<Impl> pImpl;
};

class YAUL_API SharedObject {
 public:
  /**
   * @brief Construct a new base pImpl shared object
   *
   */
  SharedObject() noexcept;

  YAUL_DEFINE_DESTRUCT_MOVE_COPY(SharedObject)

  class Impl;

 protected:
  YAUL_DEFINE_INHERIT_SHARED(SharedObject);

  /**
   * @brief Set the implementation object to a new one
   *
   * @param p new implementation to set
   */
  void setImpl(std::shared_ptr<Impl> p) noexcept;

  /**
   * @brief Access the implementation object as a derived object
   *
   * @tparam T derived Impl typename
   * @return T* pointer to implementation (dynamically casted to derived object)
   */
  template <typename T>
  [[nodiscard]] inline T* impl() const noexcept {
    return dynamic_cast<T*>(pImpl.get());
  }

 private:
  std::shared_ptr<Impl> pImpl;
};

}  // namespace yaul

#endif /* _YAUL_OBJECT_HPP_ */