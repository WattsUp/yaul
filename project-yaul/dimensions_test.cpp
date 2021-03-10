#include <yaul/dimensions.hpp>

#include <gtest/gtest.h>

class Dimensions : public ::testing::Test {
 protected:
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

  virtual void SetUp() {
    srand((unsigned)time(NULL));

    // Generate random values to increase coverage
    // Width and height must be non-zero
    aWidth  = (rand() % 2048) - 1024;
    aWidth  = (aWidth == 0) ? 2048 : aWidth;
    aHeight = (rand() % 2048) - 1024;
    aHeight = (aHeight == 0) ? 2048 : aHeight;

    aX      = (rand() % 2048) - 1024;
    aY      = (rand() % 2048) - 1024;
    aTop    = (rand() % 2048) - 1024;
    aRight  = (rand() % 2048) - 1024;
    aBottom = (rand() % 2048) - 1024;
    aLeft   = (rand() % 2048) - 1024;

    bWidth  = (rand() % 2048) - 1024;
    bWidth  = (bWidth == 0) ? 2048 : bWidth;
    bHeight = (rand() % 2048) - 1024;
    bHeight = (bHeight == 0) ? 2048 : bHeight;

    bX = (rand() % 2048) - 1024;
    bY = (rand() % 2048) - 1024;
  }

  virtual void TearDown() {}
};

TEST_F(Dimensions, Edges) {
  ::yaul::Edges a{aTop, aRight, aBottom, aLeft};

  // Proper initialization order
  ASSERT_EQ(a.top, aTop);
  ASSERT_EQ(a.right, aRight);
  ASSERT_EQ(a.bottom, aBottom);
  ASSERT_EQ(a.left, aLeft);
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
}

TEST_F(Dimensions, Rectangle) {
  ::yaul::Rectangle a{aX, aY, aWidth, aHeight};

  // Proper initialization order
  ASSERT_EQ(a.x, aX);
  ASSERT_EQ(a.y, aY);
  ASSERT_EQ(a.width, aWidth);
  ASSERT_EQ(a.height, aHeight);
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
  ::yaul::Position lieOutsideB{bX - bWidth, bY - bHeight};
  ASSERT_TRUE(b && liesWithinB);
  ASSERT_TRUE(liesWithinB && b);
  ASSERT_FALSE(b && lieOutsideB);
  ASSERT_FALSE(lieOutsideB && b);

  // Edge cases (heh pun but also corners)
  const int xAdjustment = ((bWidth > 0) ? 1 : -1);
  const int yAdjustment = ((bHeight > 0) ? 1 : -1);

  liesWithinB = ::yaul::Position{bX, bY};
  lieOutsideB = liesWithinB - ::yaul::Position{xAdjustment, yAdjustment};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  liesWithinB = ::yaul::Position{bX + bWidth, bY + bHeight};
  lieOutsideB = liesWithinB + ::yaul::Position{xAdjustment, yAdjustment};
  EXPECT_TRUE(b && liesWithinB);
  EXPECT_FALSE(b && lieOutsideB);

  lieOutsideB = ::yaul::Position{bX + bWidth / 2, bY - yAdjustment};
  EXPECT_FALSE(b && lieOutsideB);
  lieOutsideB = ::yaul::Position{bX + bWidth / 2, bY + bHeight + yAdjustment};
  EXPECT_FALSE(b && lieOutsideB);

  lieOutsideB = ::yaul::Position{bX - xAdjustment, bY + bHeight / 2};
  EXPECT_FALSE(b && lieOutsideB);
  lieOutsideB = ::yaul::Position{bX + bWidth + xAdjustment, bY + bHeight / 2};
  EXPECT_FALSE(b && lieOutsideB);

  bWidth  = (bWidth > 0) ? bWidth : -bWidth;
  bHeight = (bHeight > 0) ? bHeight : -bHeight;
  ::yaul::Rectangle positiveWidthB{bX, bY, bWidth, bHeight};
  liesWithinB = ::yaul::Position{bX, bY};
  lieOutsideB = liesWithinB - ::yaul::Position{1, 1};
  EXPECT_TRUE(positiveWidthB && liesWithinB);
  EXPECT_FALSE(positiveWidthB && lieOutsideB);

  liesWithinB = ::yaul::Position{bX + bWidth, bY + bHeight};
  lieOutsideB = liesWithinB + ::yaul::Position{1, 1};
  EXPECT_TRUE(positiveWidthB && liesWithinB);
  EXPECT_FALSE(positiveWidthB && lieOutsideB);

  bWidth  = -bWidth;
  bHeight = -bHeight;
  ::yaul::Rectangle negativeWidthB{bX, bY, bWidth, bHeight};
  liesWithinB = ::yaul::Position{bX, bY};
  lieOutsideB = liesWithinB + ::yaul::Position{1, 1};
  EXPECT_TRUE(negativeWidthB && liesWithinB);
  EXPECT_FALSE(negativeWidthB && lieOutsideB);

  liesWithinB = ::yaul::Position{bX + bWidth, bY + bHeight};
  lieOutsideB = liesWithinB - ::yaul::Position{1, 1};
  EXPECT_TRUE(negativeWidthB && liesWithinB);
  EXPECT_FALSE(negativeWidthB && lieOutsideB);
}