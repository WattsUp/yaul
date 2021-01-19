#ifndef _YAUL_POINTERS_HPP_
#define _YAUL_POINTERS_HPP_

#include <memory>
#include <yaul/common.hpp>

namespace yaul::ptr {

/**
 * @class Simple lightweight pointer wrapper to ensure delete gets called when
 * pointer goes out of scope. Not moveable and not copyable.
 *
 * @tparam T object type
 */
template <class T>
class Scoped {
 private:
  T* p;

  /**
   * @brief Create a scoped pointer by taking ownership of a raw pointer
   *
   */
  constexpr explicit Scoped(T* p) noexcept : p(p) {}

 public:
  /**
   * @brief Make a scoped pointer by calling new T(args...)
   *
   * @tparam Args types to pass to constructor
   * @param args to pass to constructor
   * @return Scoped pointer to new object
   */
  template <class... Args>
  static Scoped make(Args&&... args) {
    return Scoped(new T(std::forward<Args>(args)...));
  }

  /**
   * @brief Destroy the scoped pointer by calling delete
   *
   */
  ~Scoped() noexcept { delete p; }

  YAUL_NO_COPY(Scoped);
  YAUL_NO_MOVE(Scoped);

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
  inline void swap(Scoped& rhs) noexcept { std::swap(p, rhs.p); }
};

/**
 * @class Simple lightweight array pointer wrapper to ensure delete[] gets
 * called when pointer goes out of scope. Not moveable and not copyable.
 *
 * @tparam T object type
 */
template <class T>
class Scoped<T[]> {  // NOLINT (modernize-avoid-c-arrays)
 private:
  T* p;

  /**
   * @brief Create a scoped pointer by taking ownership of a raw pointer
   *
   */
  constexpr explicit Scoped(T* p) noexcept : p(p) {}

 public:
  /**
   * @brief Make a scoped pointer array by calling new T[len]
   *
   * @param len length of array to create
   * @return Scoped pointer to new object
   */
  static Scoped make(size_t len) { return Scoped(new T[len]()); }

  /**
   * @brief Destroy the scoped pointer array by calling delete[]
   *
   */
  ~Scoped() noexcept { delete[] p; }

  YAUL_NO_COPY(Scoped);
  YAUL_NO_MOVE(Scoped);

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
  inline void swap(Scoped& rhs) noexcept { std::swap(p, rhs.p); }
};

/**
 * @class Default deleter that calls delete on a pointer
 *
 * @tparam T type to delete
 */
template <typename T>
struct DefaultDeleter {
  constexpr DefaultDeleter() noexcept = default;

  /**
   * @brief Copy construct an existing deleter if types are compatible
   *
   * @tparam U type of deleter to copy
   */
  template <class _T, std::enable_if_t<std::is_convertible_v<_T*, T*>, int> = 0>
  explicit constexpr DefaultDeleter(
      const DefaultDeleter<_T>& /* unused */) noexcept {}

  /**
   * @brief Delete a pointer
   *
   * @param p pointer to delete
   */
  void operator()(T* p) const noexcept {
    // NOLINTNEXTLINE (bugprone-sizeof-expression)
    static_assert(0 < sizeof(T), "Can't delete an incomplete type");
    delete p;  // NOLINT (cppcoreguidelines-owning-memory)
  }
};

/**
 * @class Default array deleter that calls delete[] on a pointer
 *
 * @tparam T type to delete
 */
template <typename T>
class DefaultDeleter<T[]> {  // NOLINT (modernize-avoid-c-arrays)
 public:
  constexpr DefaultDeleter() noexcept = default;

  /**
   * @brief Copy construct an existing deleter if types are compatible
   *
   * @tparam U type of deleter to copy
   */
  template <class _T,
            // NOLINTNEXTLINE (modernize-avoid-c-arrays)
            std::enable_if_t<std::is_convertible_v<_T (*)[], T (*)[]>, int> = 0>
  explicit constexpr DefaultDeleter(
      // NOLINTNEXTLINE (modernize-avoid-c-arrays)
      const DefaultDeleter<_T[]>& /* unused */) noexcept {}

  /**
   * @brief Delete a pointer to an array
   *
   * @param p array pointer to delete[]
   */
  template <class _T,
            // NOLINTNEXTLINE (modernize-avoid-c-arrays)
            std::enable_if_t<std::is_convertible_v<_T (*)[], T (*)[]>, int> = 0>
  void operator()(_T* p) const noexcept {
    // NOLINTNEXTLINE (bugprone-sizeof-expression)
    static_assert(0 < sizeof(T), "Can't delete an incomplete type");
    delete[] p;  // NOLINT (cppcoreguidelines-owning-memory)
  }
};

/**
 * @class Unique pointer wrapper to ensure delete gets called when pointer goes
 * out of scope. Not copyable.
 *
 * @tparam T object type
 * @tparam D deleter for T
 */
template <class T, class D = DefaultDeleter<T>>
class Unique {
 private:
  T* p;
  D d;

  /**
   * @brief Create a scoped pointer by taking ownership of a raw pointer
   *
   */
  constexpr explicit Unique(T* p) noexcept : p(p), d() {}

 public:
  /**
   * @brief Create an empty unique pointer
   *
   */
  constexpr Unique() noexcept : p(nullptr), d() {}

  /**
   * @brief Make a unique pointer by calling new T(args...)
   *
   * @tparam Args types to pass to constructor
   * @param args to pass to constructor
   * @return Unique pointer to new object
   */
  template <class... Args>
  static Unique make(Args&&... args) {
    return Unique(new T(std::forward<Args>(args)...));
  }

  /**
   * @brief Destroy the unique pointer by calling the deleter
   *
   */
  ~Unique() noexcept { d(p); }

  YAUL_NO_COPY(Unique);

  /**
   * @brief Move construct a unique pointer
   *
   * @param rhs to move into this
   */
  Unique(Unique&& rhs) noexcept
      : p(rhs.release()), d(std::forward<D>(rhs.getDeleter())) {}

  /**
   * @brief Move construct a unique pointer, casting derived types
   *
   * @tparam _T derived class of T
   * @tparam _D deleter for _T
   * @param rhs to move into this
   */
  template <class _T,
            class _D,
            std::enable_if_t<std::conjunction_v<std::is_convertible<_T*, T*>,
                                                std::is_move_constructible<_D>>,
                             int> = 0>
  Unique(Unique<_T, _D>&& rhs) noexcept  // NOLINT (google-explicit-constructor)
      : p(rhs.release()), d(std::forward<_D>(rhs.getDeleter())) {}

  /**
   * @brief Move assign a unique pointer
   *
   * @param rhs to move into this
   * @return Unique& *this
   */
  Unique& operator=(Unique&& rhs) noexcept {
    swap(rhs);
    return *this;
  }

  /**
   * @brief Move assign a unique pointer, casting derived types
   *
   * @tparam _T derived class of T
   * @tparam _D deleter for _T
   * @param rhs to move into this
   * @return Unique& *this
   */
  template <class _T,
            class _D,
            std::enable_if_t<std::conjunction_v<std::is_convertible<_T*, T*>,
                                                std::is_assignable<D&, _D>>,
                             int> = 0>
  Unique& operator=(Unique<_T, _D>&& rhs) noexcept {
    swap(rhs);
    return *this;
  }

  /**
   * @brief Get the deleter for the pointer
   *
   * @return D& deleter
   */
  inline D& getDeleter() noexcept { return d; }

  /**
   * @brief Get the deleter for the pointer
   *
   * @return const D& deleter
   */
  [[nodiscard]] inline const D& getDeleter() const noexcept { return d; }

  /**
   * @brief Release the raw pointer held, transfers ownership to recipient.
   *
   * @return T* raw pointer
   */
  inline T* release() noexcept { return std::exchange(p, nullptr); }

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
   * @brief Exchange pointers between two Unique objects
   *
   * @param rhs to swap
   */
  inline void swap(Unique& rhs) noexcept {
    std::swap(p, rhs.p);
    std::swap(d, rhs.d);
  }
};

/**
 * @class Unique array pointer wrapper to ensure delete gets called when pointer
 * goes out of scope. Not copyable.
 *
 * @tparam T object type
 * @tparam D deleter for T
 */
template <class T, class D>
class Unique<T[], D> {  // NOLINT (modernize-avoid-c-arrays)
 private:
  T* p;
  D d;

  /**
   * @brief Create a scoped pointer by taking ownership of a raw pointer
   *
   */
  constexpr explicit Unique(T* p) noexcept : p(p), d() {}

 public:
  /**
   * @brief Create an empty unique pointer
   *
   */
  constexpr Unique() noexcept : p(nullptr), d() {}

  /**
   * @brief Make a unique pointer array by calling new T[len]
   *
   * @param len length of array to create
   * @return Unique pointer to new object
   */
  static Unique make(size_t len) { return Unique(new T[len]()); }

  /**
   * @brief Destroy the unique pointer by calling the deleter
   *
   */
  ~Unique() noexcept { d(p); }

  YAUL_NO_COPY(Unique);

  /**
   * @brief Move construct a unique pointer
   *
   * @param rhs to move into this
   */
  Unique(Unique&& rhs) noexcept : p(rhs.release()), d(std::forward<D>(rhs.d)) {}

  /**
   * @brief Move assign a unique pointer
   *
   * @param rhs to move into this
   * @return Unique& *this
   */
  Unique& operator=(Unique&& rhs) noexcept {
    swap(rhs);
    return *this;
  }

  /**
   * @brief Get the deleter for the pointer
   *
   * @return D& deleter
   */
  inline D& getDeleter() noexcept { return d; }

  /**
   * @brief Get the deleter for the pointer
   *
   * @return const D& deleter
   */
  [[nodiscard]] inline const D& getDeleter() const noexcept { return d; }

  /**
   * @brief Release the raw pointer held, transfers ownership to recipient.
   *
   * @return T* raw pointer
   */
  inline T* release() noexcept { return std::exchange(p, nullptr); }

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
   * @brief Exchange pointers between two Unique objects
   *
   * @param rhs to swap
   */
  inline void swap(Unique& rhs) noexcept {
    std::swap(p, rhs.p);
    std::swap(d, rhs.d);
  }
};

class Shared {};

class Weak {};

}  // namespace yaul::ptr

#endif /* _YAUL_POINTERS_HPP_ */