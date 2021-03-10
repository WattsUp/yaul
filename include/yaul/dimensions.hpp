#ifndef YAUL_VG_DIMENSIONS_HPP
#define YAUL_VG_DIMENSIONS_HPP

namespace yaul {

namespace units {

constexpr float mmPerInch = 25.4F;

};

/**
 * @brief Edges object containing top, right, bottom, and left dimensions. List
 * initialization order {top, right, bottom, left}
 *
 */
struct Edges {
  int top    = 0;
  int right  = 0;
  int bottom = 0;
  int left   = 0;
};

/**
 * @brief Size object containing a width and a height. List initialization order
 * {width, height}
 *
 */
struct Size {
  int width  = 0;
  int height = 0;

  /**
   * @brief Add two sizes together by adding their dimensions
   *
   * @param a
   * @param b
   * @return Size
   */
  friend inline Size operator+(const Size& a, const Size& b) {
    return {a.width + b.width, a.height + b.height};
  }
  inline Size& operator+=(const Size& rhs) noexcept {
    return *this = *this + rhs;
  }

  /**
   * @brief Subtract two sizes together by subtracting their dimensions
   *
   * @param a
   * @param b
   * @return Size
   */
  friend inline Size operator-(const Size& a, const Size& b) {
    return {a.width - b.width, a.height - b.height};
  }
  inline Size& operator-=(const Size& rhs) noexcept {
    return *this = *this - rhs;
  }

  /**
   * @brief Grow a size object by an edges object
   *
   * @param size
   * @param edges
   * @return Size
   */
  friend inline Size operator+(const Size& size, const Edges& edges) {
    return {size.width + (edges.left + edges.right),
            size.height + (edges.top + edges.bottom)};
  }
  inline Size& operator+=(const Edges& edges) noexcept {
    return *this = *this + edges;
  }

  /**
   * @brief Shrink a size object by an edges object
   *
   * @param size
   * @param edges
   * @return Size
   */
  friend inline Size operator-(const Size& size, const Edges& edges) {
    return {size.width - (edges.left + edges.right),
            size.height - (edges.top + edges.bottom)};
  }
  inline Size& operator-=(const Edges& edges) noexcept {
    return *this = *this - edges;
  }
};

/**
 * @brief Position object containing x and y offsets. List initialization order
 * {x, y}
 *
 */
struct Position {
  int x = 0;
  int y = 0;

  /**
   * @brief Translate a point by adding a translation
   *
   * @param point
   * @param translation
   * @return Position
   */
  friend inline Position operator+(const Position& point,
                                   const Position& translation) {
    return {point.x + translation.x, point.y + translation.y};
  }
  inline Position& operator+=(const Position& translation) noexcept {
    return *this = *this + translation;
  }

  /**
   * @brief Translate a point by subtracting a translation
   *
   * @param point
   * @param translation
   * @return Position
   */
  friend inline Position operator-(const Position& point,
                                   const Position& translation) {
    return {point.x - translation.x, point.y - translation.y};
  }
  inline Position& operator-=(const Position& translation) noexcept {
    return *this = *this - translation;
  }

  /**
   * @brief Translate a position object by adding an edges object (left and top
   * edges translate x and y)
   *
   * @param point
   * @param edges
   * @return Position
   */
  friend inline Position operator+(const Position& point, const Edges& edges) {
    return {point.x + edges.left, point.y + edges.top};
  }
  inline Position& operator+=(const Edges& edges) noexcept {
    return *this = *this + edges;
  }

  /**
   * @brief Translate a position object by subtracting an edges object (left and
   * top edges translate x and y)
   *
   * @param point
   * @param edges
   * @return Position
   */
  friend inline Position operator-(const Position& point, const Edges& edges) {
    return {point.x - edges.left, point.y - edges.top};
  }
  inline Position& operator-=(const Edges& edges) noexcept {
    return *this = *this - edges;
  }
};

/**
 * @brief Rectangle object containing x and y offsets and a width
 * and a height. List initialization order {x, y, width, height}
 *
 */
struct Rectangle {
  int x      = 0;
  int y      = 0;
  int width  = 0;
  int height = 0;

  /**
   * @brief Add a size to a rectangle by adding their dimensions
   *
   * @param rectangle
   * @param size
   * @return Rectangle
   */
  friend inline Rectangle operator+(const Rectangle& rectangle,
                                    const Size& size) {
    return {rectangle.x, rectangle.y, rectangle.width + size.width,
            rectangle.height + size.height};
  }
  inline Rectangle& operator+=(const Size& size) noexcept {
    return *this = *this + size;
  }

  /**
   * @brief Subtract a size from a rectangle by subtracting their dimensions
   *
   * @param rectangle
   * @param size
   * @return Rectangle
   */
  friend inline Rectangle operator-(const Rectangle& rectangle,
                                    const Size& size) {
    return {rectangle.x, rectangle.y, rectangle.width - size.width,
            rectangle.height - size.height};
  }
  inline Rectangle& operator-=(const Size& size) noexcept {
    return *this = *this - size;
  }

  /**
   * @brief Translate a rectangle
   *
   * @param rectangle
   * @param translation
   * @return Rectangle
   */
  friend inline Rectangle operator+(const Rectangle& rectangle,
                                    const Position& translation) {
    return {rectangle.x + translation.x, rectangle.y + translation.y,
            rectangle.width, rectangle.height};
  }
  inline Rectangle& operator+=(const Position& translation) noexcept {
    return *this = *this + translation;
  }

  /**
   * @brief Translate a rectangle
   *
   * @param rectangle
   * @param translation
   * @return Rectangle
   */
  friend inline Rectangle operator-(const Rectangle& rectangle,
                                    const Position& translation) {
    return {rectangle.x - translation.x, rectangle.y - translation.y,
            rectangle.width, rectangle.height};
  }
  inline Rectangle& operator-=(const Position& translation) noexcept {
    return *this = *this - translation;
  }

  /**
   * @brief Grow a rectangle by edges without moving existing rectangle
   *
   * @param rectangle
   * @param edges
   * @return Rectangle
   */
  friend inline Rectangle operator+(const Rectangle& rectangle,
                                    const Edges& edges) {
    return {rectangle.x - edges.left, rectangle.y - edges.top,
            rectangle.width + (edges.left + edges.right),
            rectangle.height + (edges.top + edges.bottom)};
  }
  inline Rectangle& operator+=(const Edges& edges) noexcept {
    return *this = *this + edges;
  }

  /**
   * @brief Shrink a rectangle by edges without moving existing rectangle
   *
   * @param rectangle
   * @param edges
   * @return Rectangle
   */
  friend inline Rectangle operator-(const Rectangle& rectangle,
                                    const Edges& edges) {
    return {rectangle.x + edges.left, rectangle.y + edges.top,
            rectangle.width - (edges.left + edges.right),
            rectangle.height - (edges.top + edges.bottom)};
  }
  inline Rectangle& operator-=(const Edges& edges) noexcept {
    return *this = *this - edges;
  }
};

/**
 * @brief Check if point is within rectangle (including edges)
 *
 * @param point
 * @param rect
 * @return true when point lies within the rectangle, edges inclusive. false
 * otherwise
 */
inline bool operator&&(const Position& point, const Rectangle& rect) {
  if (point.x < rect.x && point.x < (rect.x + rect.width))
    return false;
  if (point.x > rect.x && point.x > (rect.x + rect.width))
    return false;
  if (point.y < rect.y && point.y < (rect.y + rect.height))
    return false;
  if (point.y > rect.y && point.y > (rect.y + rect.height))
    return false;
  return true;
}
inline bool operator&&(const Rectangle& rect, const Position& point) {
  return point && rect;
}

}  // namespace yaul

#endif /* YAUL_VG_DIMENSIONS_HPP */