#ifndef _YAUL_POINTERS_HPP_
#define _YAUL_POINTERS_HPP_

#include <yaul/common.hpp>

namespace yaul::ptr {

/**
 * @class Simple lightweight pointer wrapper to ensure delete gets called. Not
 * moveable and not copyable.
 *
 */
template <typename T>
class YAUL_API Scoped final {
 private:
  T* p;

 public:
  /**
   * @brief Create a scoped pointer. Commonly used Scoped<T>(new T(args...))
   *
   * @param o pointer to take ownership of
   */
  constexpr explicit Scoped(T* o = nullptr) noexcept : p(o) {}

  ~Scoped() noexcept { delete p; }

  YAUL_NO_COPY(Scoped);
  YAUL_NO_MOVE(Scoped);

  /**
   * @brief Reset the held pointer to a new one, old one is deleted
   *
   * @param o pointer to take ownership of
   */
  inline void reset(T* o = nullptr) noexcept {
    delete p;
    p = o;
  };

  /**
   * @brief Dereference pointer to gain held object, does not check for nullptr
   *
   * @return T& held object
   */
  inline T& operator*() const noexcept { return *p; }

  /**
   * @brief Normal pointer access held object, does not check for nullptr
   *
   * @return T* held object
   */
  inline T* operator->() const noexcept { return p; }

  /**
   * @brief Get the raw pointer held by the object
   *
   * @return T* raw pointer
   */
  [[nodiscard]] inline T* get() const noexcept { return p; }

  /**
   * @brief Check for non-nullptr content
   *
   * @return raw pointer != nullptr
   */
  inline explicit operator bool() const noexcept { return p != nullptr; }

  /**
   * @brief Exchange pointers between two Scoped objects
   *
   * @param rhs to swap
   */
  inline void swap(Scoped& rhs) noexcept {
    T* temp = p;
    p       = rhs.p;
    rhs.p   = temp;
  }
};

/**
 * @class Simple lightweight pointer wrapper to ensure delete[] gets called. Not
 * moveable and not copyable.
 *
 */
template <typename T>
class YAUL_API ScopedArray final {
 private:
  T* p;

 public:
  /**
   * @brief Create a scoped pointer to an array. Commonly used Scoped<T>(new
   * T[len])
   *
   * @param o pointer to take ownership of
   */
  constexpr explicit ScopedArray(T* o = nullptr) noexcept : p(o) {}

  /**
   * @brief Create a scoped pointer to a new array
   *
   * @param len of array to declare
   */
  constexpr explicit ScopedArray(size_t len) noexcept : p(new T[len]) {}

  ~ScopedArray() noexcept { delete[] p; }

  YAUL_NO_COPY(ScopedArray);
  YAUL_NO_MOVE(ScopedArray);

  /**
   * @brief Reset the held pointer to a new one, old one is deleted
   *
   * @param o pointer to take ownership of
   */
  inline void reset(T* o = nullptr) noexcept {
    delete p;
    p = o;
  };

  /**
   * @brief Reset the held pointer to a new array
   *
   * @param len of array to declare
   */
  inline void reset(size_t len) noexcept {
    delete p;
    p = new T[len];  // NOLINT (cppcoreguidelines-owning-memory)
  };

  /**
   * @brief Array element accessor to get an element of the held array. No
   * bounds checking performed
   *
   * @param i index
   * @return T& element at index i of array
   */
  inline T& operator[](std::ptrdiff_t i) const noexcept { return p[i]; }

  /**
   * @brief Get the raw pointer held by the object
   *
   * @return T* raw pointer
   */
  [[nodiscard]] inline T* get() const noexcept { return p; }

  /**
   * @brief Check for non-nullptr content
   *
   * @return raw pointer != nullptr
   */
  inline explicit operator bool() const noexcept { return p != nullptr; }

  /**
   * @brief Exchange pointers between two Scoped objects
   *
   * @param rhs to swap
   */
  inline void swap(ScopedArray& rhs) noexcept {
    T* temp = p;
    p       = rhs.p;
    rhs.p   = temp;
  }
};

class YAUL_API Unique {};

class YAUL_API Shared {};

class YAUL_API Weak {};

}  // namespace yaul::ptr

#endif /* _YAUL_POINTERS_HPP_ */