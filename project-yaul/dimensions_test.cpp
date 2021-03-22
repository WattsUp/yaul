#include <yaul/dimensions.hpp>

#include "common/string.hpp"

#include <gtest/gtest.h>

class Dimensions : public ::testing::Test {
 protected:
  ::yaul::string initialConfig;

  int aWidth;
  int aHeight;
  int aX;
  int aY;
  int aTop;
  int aRight;
  int aBottom;
  int aLeft;

  int bWidth;
  int bHeight;
  int bX;
  int bY;
  int bTop;
  int bRight;
  int bBottom;
  int bLeft;

  virtual void SetUp() {
    // Generate random values to increase coverage
    // Width and height must be non-zero
    aWidth  = (rand() % 2048) - 1024;
    aWidth  = (aWidth == 0) ? 2048 : aWidth;
    aHeight = (rand() % 2048) - 1024;
    aHeight = (aHeight == 0) ? 2048 : aHeight;

    aX = (rand() % 2048) - 1024;
    aY = (rand() % 2048) - 1024;

    aTop    = (rand() % 2048) - 1024;
    aRight  = (rand() % 2048) - 1024;
    aBottom = (rand() % 2048) - 1024;
    aLeft   = (rand() % 2048) - 1024;

    bWidth  = (rand() % 2048) - 1024;
    bWidth  = (bWidth == 0) ? 2048 : bWidth;
    bWidth  = (bWidth == aWidth) ? 2049 : bWidth;
    bHeight = (rand() % 2048) - 1024;
    bHeight = (bHeight == 0) ? 2048 : bHeight;
    bHeight = (bHeight == aHeight) ? 2049 : bHeight;

    bX = (rand() % 2048) - 1024;
    bY = (rand() % 2048) - 1024;
    bX = (bX == aX) ? 2049 : bX;
    bY = (bY == aY) ? 2049 : bY;

    bTop    = (rand() % 2048) - 1024;
    bRight  = (rand() % 2048) - 1024;
    bBottom = (rand() % 2048) - 1024;
    bLeft   = (rand() % 2048) - 1024;

    initialConfig += "  aWidth=   " + std::to_string(aWidth) + ";\n";
    initialConfig += "  aHeight=  " + std::to_string(aHeight) + ";\n";
    initialConfig += "  aX=       " + std::to_string(aX) + ";\n";
    initialConfig += "  aY=       " + std::to_string(aY) + ";\n";
    initialConfig += "  aTop=     " + std::to_string(aTop) + ";\n";
    initialConfig += "  aRight=   " + std::to_string(aRight) + ";\n";
    initialConfig += "  aBottom=  " + std::to_string(aBottom) + ";\n";
    initialConfig += "  aLeft=    " + std::to_string(aLeft) + ";\n";

    initialConfig += "  bWidth=   " + std::to_string(bWidth) + ";\n";
    initialConfig += "  bHeight=  " + std::to_string(bHeight) + ";\n";
    initialConfig += "  bX=       " + std::to_string(bX) + ";\n";
    initialConfig += "  bY=       " + std::to_string(bY) + ";\n";
    initialConfig += "  bTop=     " + std::to_string(bTop) + ";\n";
    initialConfig += "  bRight=   " + std::to_string(bRight) + ";\n";
    initialConfig += "  bBottom=  " + std::to_string(bBottom) + ";\n";
    initialConfig += "  bLeft=    " + std::to_string(bLeft) + ";\n";
  }

  virtual void TearDown() {
    if (::testing::Test::HasFailure()) {
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_RED,
                                         "Failing Test Configuration\n");
      ::testing::internal::ColoredPrintf(testing::internal::COLOR_DEFAULT, "%s",
                                         initialConfig.c_str());
    }
  }
};

TEST_F(Dimensions, Edges) {
  ::yaul::Edges a{aTop, aRight, aBottom, aLeft};
  ::yaul::Edges b{bTop, bRight, bBottom, bLeft};
  ::yaul::Edges c;

  // Proper initialization order
  ASSERT_EQ(a.top, aTop);
  ASSERT_EQ(a.right, aRight);
  ASSERT_EQ(a.bottom, aBottom);
  ASSERT_EQ(a.left, aLeft);

  c = a;
  c += b;
  EXPECT_EQ(c.top, aTop + bTop);
  EXPECT_EQ(c.right, aRight + bRight);
  EXPECT_EQ(c.bottom, aBottom + bBottom);
  EXPECT_EQ(c.left, aLeft + bLeft);

  c = a;
  c -= b;
  EXPECT_EQ(c.top, aTop - bTop);
  EXPECT_EQ(c.right, aRight - bRight);
  EXPECT_EQ(c.bottom, aBottom - bBottom);
  EXPECT_EQ(c.left, aLeft - bLeft);

  c = a + b + b;
  EXPECT_EQ(c.top, aTop + bTop * 2);
  EXPECT_EQ(c.right, aRight + bRight * 2);
  EXPECT_EQ(c.bottom, aBottom + bBottom * 2);
  EXPECT_EQ(c.left, aLeft + bLeft * 2);

  c = a - b - b;
  EXPECT_EQ(c.top, aTop - bTop * 2);
  EXPECT_EQ(c.right, aRight - bRight * 2);
  EXPECT_EQ(c.bottom, aBottom - bBottom * 2);
  EXPECT_EQ(c.left, aLeft - bLeft * 2);

  c = a;
  ASSERT_EQ(a, c);

  c.left += 1;
  ASSERT_NE(a, c);
}

TEST_F(Dimensions, Size) {
  ::yaul::Size a{aWidth, aHeight};
  ::yaul::Size b{bWidth, bHeight};
  ::yaul::Size c;

  // Proper initialization order
  ASSERT_EQ(a.width, aWidth);
  ASSERT_EQ(a.height, aHeight);

  c = a;
  c += b;
  EXPECT_EQ(c.width, aWidth + bWidth);
  EXPECT_EQ(c.height, aHeight + bHeight);

  c = a;
  c -= b;
  EXPECT_EQ(c.width, aWidth - bWidth);
  EXPECT_EQ(c.height, aHeight - bHeight);

  c = a + b + b;
  EXPECT_EQ(c.width, aWidth + bWidth * 2);
  EXPECT_EQ(c.height, aHeight + bHeight * 2);

  c = a - b - b;
  EXPECT_EQ(c.width, aWidth - bWidth * 2);
  EXPECT_EQ(c.height, aHeight - bHeight * 2);

  c = a;
  ASSERT_EQ(a, c);

  c.height += 1;
  ASSERT_NE(a, c);
}

TEST_F(Dimensions, Position) {
  ::yaul::Position a{aX, aY};
  ::yaul::Position b{bX, bY};
  ::yaul::Position c;

  // Proper initialization order
  ASSERT_EQ(a.x, aX);
  ASSERT_EQ(a.y, aY);

  c = a;
  c += b;
  EXPECT_EQ(c.x, aX + bX);
  EXPECT_EQ(c.y, aY + bY);

  c = a;
  c -= b;
  EXPECT_EQ(c.x, aX - bX);
  EXPECT_EQ(c.y, aY - bY);

  c = a + b + b;
  EXPECT_EQ(c.x, aX + bX * 2);
  EXPECT_EQ(c.y, aY + bY * 2);

  c = a - b - b;
  EXPECT_EQ(c.x, aX - bX * 2);
  EXPECT_EQ(c.y, aY - bY * 2);

  c = a;
  ASSERT_EQ(a, c);

  c.y += 1;
  ASSERT_NE(a, c);
}

TEST_F(Dimensions, Rectangle) {
  ::yaul::Rectangle a{aX, aY, aWidth, aHeight};

  // Proper initialization order
  ASSERT_EQ(a.x, aX);
  ASSERT_EQ(a.y, aY);
  ASSERT_EQ(a.width, aWidth);
  ASSERT_EQ(a.height, aHeight);

  auto c = a;
  ASSERT_EQ(a, c);

  c.height += 1;
  ASSERT_NE(a, c);

  int minX   = (aWidth > 0) ? aX : aX + aWidth;
  int maxX   = (aWidth > 0) ? aX + aWidth : aX;
  int minY   = (aHeight > 0) ? aY : aY + aHeight;
  int maxY   = (aHeight > 0) ? aY + aHeight : aY;
  auto edges = static_cast<::yaul::Edges>(a);
  ASSERT_EQ(edges.left, minX);
  ASSERT_EQ(edges.right, maxX);
  ASSERT_EQ(edges.top, minY);
  ASSERT_EQ(edges.bottom, maxY);

  ASSERT_EQ(a.area(), aWidth * aHeight);
  // Integrate area to check assumptions for intersection test
  int manualArea = 0;
  for (int x = minX - 1; x <= maxX; ++x) {
    for (int y = minY - 1; y <= maxY; ++y) {
      if (a && ::yaul::Position{x, y})
        ++manualArea;
    }
  }
  if (aWidth * aHeight < 0)
    manualArea = -manualArea;
  ASSERT_EQ(a.area(), manualArea);

  c = a.abs();
  ASSERT_EQ(static_cast<::yaul::Edges>(c), static_cast<::yaul::Edges>(a));
  ASSERT_GE(c.area(), 0);
}

TEST_F(Dimensions, RectangleIntersection) {
  ::yaul::Rectangle a{aX, aY, aWidth, aHeight};
  ::yaul::Rectangle b{bX, bY, bWidth, bHeight};

  auto c = a & b;

  // Intersection cannot have more area than either
  auto aArea = a.area();
  aArea      = (aArea > 0) ? aArea : -aArea;
  auto bArea = b.area();
  bArea      = (bArea > 0) ? bArea : -bArea;
  ASSERT_LE(c.area(), aArea);
  ASSERT_LE(c.area(), bArea);

  auto aEdges = static_cast<::yaul::Edges>(a);
  auto bEdges = static_cast<::yaul::Edges>(b);

  // Integrate area
  // clang-format off
  int minX = (aEdges.left   < bEdges.left)    ? aEdges.left   : bEdges.left;
  int maxX = (aEdges.right  < bEdges.right)   ? aEdges.right  : bEdges.right;
  int minY = (aEdges.top    < bEdges.top)     ? aEdges.top    : bEdges.top;
  int maxY = (aEdges.bottom < bEdges.bottom)  ? aEdges.bottom : bEdges.bottom;
  // clang-format on
  int manualArea = 0;
  for (int x = minX - 1; x <= maxX; ++x) {
    for (int y = minY - 1; y <= maxY; ++y) {
      ::yaul::Position p{x, y};
      if ((a && p) && (b && p))
        ++manualArea;
    }
  }
  ASSERT_EQ(c.area(), manualArea);

  c = a & a;
  ASSERT_EQ(c.area(), aArea);

  c        = a;
  c.width  = aWidth / 2;
  c.height = aHeight / 2;

  auto d = c & a;
  ASSERT_EQ(d.area(), c.abs().area());
  d = a & c;
  ASSERT_EQ(d.area(), c.abs().area());
}

TEST_F(Dimensions, SizeAndEdges) {
  ::yaul::Edges a{aTop, aRight, aBottom, aLeft};
  ::yaul::Size b{bWidth, bHeight};
  ::yaul::Size c;

  c = b;
  c += a;
  EXPECT_EQ(c.width, bWidth + (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight + (aTop + aBottom));

  c = b;
  c -= a;
  EXPECT_EQ(c.width, bWidth - (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight - (aTop + aBottom));

  c = b + a;
  EXPECT_EQ(c.width, bWidth + (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight + (aTop + aBottom));

  c = b - a;
  EXPECT_EQ(c.width, bWidth - (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight - (aTop + aBottom));
}

TEST_F(Dimensions, PositionAndEdges) {
  ::yaul::Edges a{aTop, aRight, aBottom, aLeft};
  ::yaul::Position b{bX, bY};
  ::yaul::Position c;

  c = b;
  c += a;
  EXPECT_EQ(c.x, bX + aLeft);
  EXPECT_EQ(c.y, bY + aTop);

  c = b;
  c -= a;
  EXPECT_EQ(c.x, bX - aLeft);
  EXPECT_EQ(c.y, bY - aTop);

  c = b + a;
  EXPECT_EQ(c.x, bX + aLeft);
  EXPECT_EQ(c.y, bY + aTop);

  c = b - a;
  EXPECT_EQ(c.x, bX - aLeft);
  EXPECT_EQ(c.y, bY - aTop);
}

TEST_F(Dimensions, RectangleAndEdges) {
  ::yaul::Edges a{aTop, aRight, aBottom, aLeft};
  ::yaul::Rectangle b{bX, bY, bWidth, bHeight};
  ::yaul::Rectangle c;

  c = b;
  c += a;
  EXPECT_EQ(c.x, bX - aLeft);
  EXPECT_EQ(c.y, bY - aTop);
  EXPECT_EQ(c.width, bWidth + (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight + (aTop + aBottom));

  c = b;
  c -= a;
  EXPECT_EQ(c.x, bX + aLeft);
  EXPECT_EQ(c.y, bY + aTop);
  EXPECT_EQ(c.width, bWidth - (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight - (aTop + aBottom));

  c = b + a;
  EXPECT_EQ(c.x, bX - aLeft);
  EXPECT_EQ(c.y, bY - aTop);
  EXPECT_EQ(c.width, bWidth + (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight + (aTop + aBottom));

  c = b - a;
  EXPECT_EQ(c.x, bX + aLeft);
  EXPECT_EQ(c.y, bY + aTop);
  EXPECT_EQ(c.width, bWidth - (aRight + aLeft));
  EXPECT_EQ(c.height, bHeight - (aTop + aBottom));
}

TEST_F(Dimensions, RectangleAndSize) {
  ::yaul::Size a{aWidth, aHeight};
  ::yaul::Rectangle b{bX, bY, bWidth, bHeight};
  ::yaul::Rectangle c;

  c = b;
  c += a;
  EXPECT_EQ(c.x, bX);
  EXPECT_EQ(c.y, bY);
  EXPECT_EQ(c.width, bWidth + aWidth);
  EXPECT_EQ(c.height, bHeight + aHeight);

  c = b;
  c -= a;
  EXPECT_EQ(c.x, bX);
  EXPECT_EQ(c.y, bY);
  EXPECT_EQ(c.width, bWidth - aWidth);
  EXPECT_EQ(c.height, bHeight - aHeight);

  c = b + a;
  EXPECT_EQ(c.x, bX);
  EXPECT_EQ(c.y, bY);
  EXPECT_EQ(c.width, bWidth + aWidth);
  EXPECT_EQ(c.height, bHeight + aHeight);

  c = b - a;
  EXPECT_EQ(c.x, bX);
  EXPECT_EQ(c.y, bY);
  EXPECT_EQ(c.width, bWidth - aWidth);
  EXPECT_EQ(c.height, bHeight - aHeight);

  ASSERT_NE(aWidth, bWidth);
  ASSERT_NE(aHeight, bHeight);
  EXPECT_NE(a, b);
  EXPECT_NE(b, a);

  a.width  = bWidth;
  a.height = bHeight;
  EXPECT_EQ(a, b);
  EXPECT_EQ(b, a);
}

TEST_F(Dimensions, RectangleAndPosition) {
  ::yaul::Position a{aX, aY};
  ::yaul::Rectangle b{bX, bY, bWidth, bHeight};
  ::yaul::Rectangle c;

  c = b;
  c += a;
  EXPECT_EQ(c.x, bX + aX);
  EXPECT_EQ(c.y, bY + aY);
  EXPECT_EQ(c.width, bWidth);
  EXPECT_EQ(c.height, bHeight);

  c = b;
  c -= a;
  EXPECT_EQ(c.x, bX - aX);
  EXPECT_EQ(c.y, bY - aY);
  EXPECT_EQ(c.width, bWidth);
  EXPECT_EQ(c.height, bHeight);

  c = b + a;
  EXPECT_EQ(c.x, bX + aX);
  EXPECT_EQ(c.y, bY + aY);
  EXPECT_EQ(c.width, bWidth);
  EXPECT_EQ(c.height, bHeight);

  c = b - a;
  EXPECT_EQ(c.x, bX - aX);
  EXPECT_EQ(c.y, bY - aY);
  EXPECT_EQ(c.width, bWidth);
  EXPECT_EQ(c.height, bHeight);

  ::yaul::Position liesWithinB{bX + bWidth / 2, bY + bHeight / 2};
  if (bWidth < 0)
    liesWithinB.x -= 1;
  if (bHeight < 0)
    liesWithinB.y -= 1;
  ::yaul::Position lieOutsideB{bX - bWidth, bY - bHeight};
  ASSERT_TRUE(b && liesWithinB);
  ASSERT_TRUE(liesWithinB && b);
  ASSERT_FALSE(b && lieOutsideB);
  ASSERT_FALSE(lieOutsideB && b);

  auto edges = static_cast<::yaul::Edges>(b);

  // Top left
  liesWithinB = ::yaul::Position{edges.left, edges.top};
  lieOutsideB = liesWithinB + ::yaul::Position{0, -1};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  // Top
  liesWithinB = ::yaul::Position{(edges.left + edges.right) / 2, edges.top};
  if (edges.left < 0)
    liesWithinB.x -= 1;
  lieOutsideB = liesWithinB + ::yaul::Position{0, -1};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  // Top right
  liesWithinB = ::yaul::Position{edges.right - 1, edges.top};
  lieOutsideB = liesWithinB + ::yaul::Position{1, 0};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  // Right
  liesWithinB =
      ::yaul::Position{edges.right - 1, (edges.top + edges.bottom) / 2};
  if (edges.top < 0)
    liesWithinB.y -= 1;
  lieOutsideB = liesWithinB + ::yaul::Position{1, 0};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  // Bottom right
  liesWithinB = ::yaul::Position{edges.right - 1, edges.bottom - 1};
  lieOutsideB = liesWithinB + ::yaul::Position{0, 1};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  // Bottom
  liesWithinB =
      ::yaul::Position{(edges.left + edges.right) / 2, edges.bottom - 1};
  if (edges.left < 0)
    liesWithinB.x -= 1;
  lieOutsideB = liesWithinB + ::yaul::Position{0, 1};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  // Bottom left
  liesWithinB = ::yaul::Position{edges.left, edges.bottom - 1};
  lieOutsideB = liesWithinB + ::yaul::Position{-1, 0};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  // Left
  liesWithinB = ::yaul::Position{edges.left, (edges.top + edges.bottom) / 2};
  if (edges.top < 0)
    liesWithinB.y -= 1;
  lieOutsideB = liesWithinB + ::yaul::Position{-1, 0};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  ASSERT_NE(aX, bX);
  ASSERT_NE(aY, bY);
  EXPECT_NE(a, b);
  EXPECT_NE(b, a);

  a.x = bX;
  a.y = bY;
  EXPECT_EQ(a, b);
  EXPECT_EQ(b, a);
}