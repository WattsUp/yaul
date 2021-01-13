#ifndef _YAUL_VG_DIMENSIONS_HPP_
#define _YAUL_VG_DIMENSIONS_HPP_

namespace yaul {

/**
 * @brief Size object containing a width and a height
 *
 */
struct Size {
  int width  = 0;
  int height = 0;
};

/**
 * @brief Position object containing top (y) and left (x) offsets
 *
 */
struct Position {
  int top  = 0;
  int left = 0;
};

/**
 * @brief Edges object containing top, right, bottom, and left dimensions
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