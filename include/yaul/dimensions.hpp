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

  /**
   * @brief Add two edges together by adding their dimensions
   *
   * @param a
   * @param b
   * @return Edges
   */
  friend inline Edges operator+(const Edges& a, const Edges& b) {
    return {a.top + b.top, a.right + b.right, a.bottom + b.bottom,
            a.left + b.left};
  }
  inline Edges& operator+=(const Edges& rhs) noexcept {
    return *this = *this + rhs;
  }

  /**
   * @brief Subtract two edges together by subtracting their dimensions
   *
   * @param a
   * @param b
   * @return Edges
   */
  friend inline Edges operator-(const Edges& a, const Edges& b) {
    return {a.top - b.top, a.right - b.right, a.bottom - b.bottom,
            a.left - b.left};
  }
  inline Edges& operator-=(const Edges& rhs) noexcept {
    return *this = *this - rhs;
  }

  /**
   * @brief Equality operator, compares each member variable
   *
   * @param rhs
   * @return true if all member variables are equal, false otherwise
   */
  inline bool operator==(const Edges& rhs) const noexcept {
    return (top == rhs.top) && (right == rhs.right) && (bottom == rhs.bottom) &&
           (left == rhs.left);
  }

  /**
   * @brief Inequality operator, compares each member variable
   *
   * @param rhs
   * @return true if any member variables are not equal, false otherwise
   */
  inline bool operator!=(const Edges& rhs) const noexcept {
    return (top != rhs.top) || (right != rhs.right) || (bottom != rhs.bottom) ||
           (left != rhs.left);
  }
};

/**
 * @brief Size object containing a width and a height. List initialization order
 * {width, height}
 *
 * When applicable, uses screen coordinates: {0, 0} is top left corner (with
 * positive width/height), +x is right, +y is down
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

  /**
   * @brief Equality operator, compares each member variable
   *
   * @param rhs
   * @return true if all member variables are equal, false otherwise
   */
  inline bool operator==(const Size& rhs) const noexcept {
    return (width == rhs.width) && (height == rhs.height);
  }

  /**
   * @brief Inequality operator, compares each member variable
   *
   * @param rhs
   * @return true if any member variables are not equal, false otherwise
   */
  inline bool operator!=(const Size& rhs) const noexcept {
    return (width != rhs.width) || (height != rhs.height);
  }
};

/**
 * @brief Position object containing x and y offsets. List initialization order
 * {x, y}
 *
 * When applicable, uses screen coordinates: {0, 0} is top left corner (with
 * positive width/height), +x is right, +y is down
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

  /**
   * @brief Equality operator, compares each member variable
   *
   * @param rhs
   * @return true if all member variables are equal, false otherwise
   */
  inline bool operator==(const Position& rhs) const noexcept {
    return (x == rhs.x) && (y == rhs.y);
  }

  /**
   * @brief Inequality operator, compares each member variable
   *
   * @param rhs
   * @return true if any member variables are not equal, false otherwise
   */
  inline bool operator!=(const Position& rhs) const noexcept {
    return (x != rhs.x) || (y != rhs.y);
  }
};

/**
 * @brief Rectangle object containing x and y offsets and a width
 * and a height. List initialization order {x, y, width, height}
 *
 * When applicable, uses screen coordinates: {0, 0} is top left corner (with
 * positive width/height), +x is right, +y is down
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

  /**
   * @brief Equality operator, compares each member variable
   *
   * @param rhs
   * @return true if all member variables are equal, false otherwise
   */
  inline bool operator==(const Rectangle& rhs) const noexcept {
    return (x == rhs.x) && (y == rhs.y) && (width == rhs.width) &&
           (height == rhs.height);
  }

  /**
   * @brief Inequality operator, compares each member variable
   *
   * @param rhs
   * @return true if any member variables are not equal, false otherwise
   */
  inline bool operator!=(const Rectangle& rhs) const noexcept {
    return (x != rhs.x) || (y != rhs.y) || (width != rhs.width) ||
           (height != rhs.height);
  }

  /**
   * @brief Cast a rectangle to edges, ensuring left < right and bottom > top
   *
   * @return Edges
   */
  explicit inline operator Edges() const noexcept {
    Edges edges;
    if (width > 0) {
      edges.left  = x;
      edges.right = x + width;
    } else {
      edges.left  = x + width;
      edges.right = x;
    }
    if (height > 0) {
      edges.top    = y;
      edges.bottom = y + height;
    } else {
      edges.top    = y + height;
      edges.bottom = y;
    }
    return edges;
  }

  /**
   * @brief Computethe area
   *
   * @return int
   */
  [[nodiscard]] inline int area() const noexcept { return width * height; }

  /**
   * @brief Compute a rectangle with positive dimensions
   *
   * @return Rectangle
   */
  [[nodiscard]] inline Rectangle abs() const noexcept {
    if (width > 0 && height > 0)
      return *this;
    Rectangle copy(*this);
    if (width < 0) {
      copy.x += width;
      copy.width = -width;
    }
    if (height < 0) {
      copy.y += height;
      copy.height = -height;
    }
    return copy;
  }

  /**
   * @brief Boolean interection operator, returns the area of intersection
   *
   * @param rhs
   * @return Rectangle of intersection area
   */
  Rectangle operator&(const Rectangle& rhs) const noexcept {
    auto edges    = static_cast<Edges>(*this);
    auto rhsEdges = static_cast<Edges>(rhs);
    // Fails if not overlapping
    // clang-format off
    edges.top      = (edges.top     > rhsEdges.top)     ? edges.top     : rhsEdges.top;
    edges.right    = (edges.right   < rhsEdges.right)   ? edges.right   : rhsEdges.right;
    edges.bottom   = (edges.bottom  < rhsEdges.bottom)  ? edges.bottom  : rhsEdges.bottom;
    edges.left     = (edges.left    > rhsEdges.left)    ? edges.left    : rhsEdges.left;
    // clang-format on
    if (edges.right < edges.left || edges.bottom < edges.top)
      return Rectangle{0, 0, 0, 0};  // No overlap
    return Rectangle{edges.left, edges.top, edges.right - edges.left,
                     edges.bottom - edges.top};
  }
};

/**
 * @brief Check if point is within rectangle
 * Point is checked as if center of pixel, i.e. +{0.5, 0.5}. Top and left edges
 * return true, bottom and right edges return false. The number of unique points
 * that return true equals the rect's area.
 *
 * @param point
 * @param rect
 * @return true when point lies within the rectangle. false otherwise
 */
inline bool operator&&(const Position& point, const Rectangle& rect) noexcept {
  auto edges = static_cast<Edges>(rect);
  if (point.x < edges.left || point.x >= edges.right)
    return false;
  if (point.y < edges.top || point.y >= edges.bottom)
    return false;
  return true;
}
inline bool operator&&(const Rectangle& rect, const Position& point) noexcept {
  return point && rect;
}

/**
 * @brief Equality operator, compares rectangle is located at point
 *
 * @param rhs
 * @return true if rectangle.x/y == point.x/y, false otherwise
 */
inline bool operator==(const Rectangle& rect, const Position& point) noexcept {
  return (rect.x == point.x) && (rect.y == point.y);
}
inline bool operator==(const Position& point, const Rectangle& rect) noexcept {
  return rect == point;
}

/**
 * @brief Inequality operator, compares rectangle is not located at point
 *
 * @param rhs
 * @return true if rectangle.x/y != point.x/y, false otherwise
 */
inline bool operator!=(const Rectangle& rect, const Position& point) noexcept {
  return (rect.x != point.x) || (rect.y != point.y);
}
inline bool operator!=(const Position& point, const Rectangle& rect) noexcept {
  return rect != point;
}

/**
 * @brief Equality operator, compares rectangle is of same size
 *
 * @param rhs
 * @return true if rectangle.width/height == point.width/height, false
 * otherwise
 */
inline bool operator==(const Rectangle& rect, const Size& size) noexcept {
  return (rect.width == size.width) && (rect.height == size.height);
}
inline bool operator==(const Size& size, const Rectangle& rect) noexcept {
  return rect == size;
}

/**
 * @brief Inequality operator, compares rectangle is not of same size
 *
 * @param rhs
 * @return true if rectangle.width/height != point.width/height, false
 * otherwise
 */
inline bool operator!=(const Rectangle& rect, const Size& size) noexcept {
  return (rect.width != size.width) || (rect.height != size.height);
}
inline bool operator!=(const Size& size, const Rectangle& rect) noexcept {
  return rect != size;
}

}  // namespace yaul

#endif /* YAUL_VG_DIMENSIONS_HPP */