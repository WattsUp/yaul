#ifndef YAUL_VG_DIMENSIONS_HPP
#define YAUL_VG_DIMENSIONS_HPP

namespace yaul {

/**
 * @brief Size object containing a width and a height. List initialization order
 * {width, height}
 *
 */
struct Size {
  int width  = 0;
  int height = 0;
};

/**
 * @brief Position object containing x and y offsets. List initialization order
 * {x, y}
 *
 */
struct Position {
  int x = 0;
  int y = 0;

  inline Position& operator+=(const Position& translation) noexcept {
    x += translation.x;
    y += translation.y;
    return *this;
  }

  inline Position& operator-=(const Position& translation) noexcept {
    x -= translation.x;
    y -= translation.y;
    return *this;
  }
};

/**
 * @brief Translate a point by adding a translation
 *
 * @param point
 * @param translation
 * @return Position
 */
inline Position operator+(const Position& point, const Position& translation) {
  return {point.x + translation.x, point.y + translation.y};
}

/**
 * @brief Translate a point by subtracting a translation
 *
 * @param point
 * @param translation
 * @return Position
 */
inline Position operator-(const Position& point, const Position& translation) {
  return {point.x - translation.x, point.y - translation.y};
}

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

  inline Rectangle& operator+=(const Position& translation) noexcept {
    x += translation.x;
    y += translation.y;
    return *this;
  }
};

/**
 * @brief Translate a rectangle by adding a translation
 *
 * @param rect
 * @param point
 * @return Rectangle
 */
inline Rectangle operator+(const Rectangle& rect, const Position& translation) {
  return {rect.x + translation.x, rect.y + translation.y, rect.width,
          rect.height};
}

/**
 * @brief Check if point is within rectangle (including edges)
 *
 * @param point
 * @param rect
 * @return true when point lies within the rectangle, edges inclusive. false
 * otherwise
 */
inline bool operator&&(const Position& point, const Rectangle& rect) {
  if (point.x < rect.x)
    return false;
  if (point.x > (rect.x + rect.width))
    return false;
  if (point.y < rect.y)
    return false;
  if (point.y > (rect.y + rect.height))
    return false;
  return true;
}
inline bool operator&&(const Rectangle& rect, const Position& point) {
  return point && rect;
}

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

}  // namespace yaul

#endif /* _YAUL_VG_DIMENSIONS_HPP_ */