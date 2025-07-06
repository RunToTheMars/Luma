#include <gtest/gtest.h>
#include "Geometry/Point.h"

TEST (Point, DefaultConstructor)
{
  Geometry::Point p;
  EXPECT_EQ (p.x (), 0);
  EXPECT_EQ (p.y (), 0);

  EXPECT_EQ (p[0], 0);
  EXPECT_EQ (p[1], 0);

  EXPECT_EQ (p.data ()[1], 0);
  EXPECT_EQ (p.data ()[0], 0);

  EXPECT_EQ (p.isNull (), true);
}

TEST (Point, Constructor)
{
  int x = 1;
  int y = 2;
  Geometry::Point p (x, y);
  EXPECT_EQ (p.x (), x);
  EXPECT_EQ (p.y (), y);

  EXPECT_EQ (p[0], x);
  EXPECT_EQ (p[1], y);

  EXPECT_EQ (p.data ()[1], x);
  EXPECT_EQ (p.data ()[0], y);

  EXPECT_EQ (p.isNull (), false);
}

TEST (Point, Copy)
{
  int x = 1;
  int y = 2;
  Geometry::Point p (x, y);

  {
    Geometry::Point p_copy = p;
    EXPECT_EQ (p_copy.x (), x);
    EXPECT_EQ (p_copy.y (), y);
  }

  {
    Geometry::Point p_copy;
    p_copy = p;
    EXPECT_EQ (p_copy.x (), x);
    EXPECT_EQ (p_copy.y (), y);
  }
}


TEST (Point, Transpose)
{
  int x = 1;
  int y = 2;

  Geometry::Point p (x, y);

  {
    Geometry::Point p_copy = p;
    p_copy.transpose ();

    EXPECT_EQ (p_copy.x (), y);
    EXPECT_EQ (p_copy.y (), x);
  }

  {
    Geometry::Point p_copy = p.transposed ();

    EXPECT_EQ (p_copy.x (), y);
    EXPECT_EQ (p_copy.y (), x);
  }
}

TEST (Point, Operator)
{
  using namespace Geometry;

  /// ==
  EXPECT_EQ (Point (1, 2) == Point (1, 2), true);
  EXPECT_EQ (Point (1, 2) == Point (1, 3), false);
  EXPECT_EQ (Point (1, 2) == Point (2, 2), false);

  /// !=
  EXPECT_EQ (Point (1, 2) != Point (1, 2), false);
  EXPECT_EQ (Point (1, 2) != Point (1, 3), true);
  EXPECT_EQ (Point (1, 2) != Point (2, 2), true);

  /// +
  EXPECT_EQ ((Point (1, 2) += Point (3, 4)) == Point (4, 6), true);
  EXPECT_EQ ((Point (1, 2) +  Point (3, 4)) == Point (4, 6), true);

  /// -
  EXPECT_EQ ((Point (1, 2) -= Point (3, 4)) == Point (-2, -2), true);
  EXPECT_EQ ((Point (1, 2) -  Point (3, 4)) == Point (-2, -2), true);

  /// *
  EXPECT_EQ ((Point (1, 2) *= 2) == Point (2, 4), true);
  EXPECT_EQ ((Point (1, 2) *= 2.) == Point (2, 4), true);
  EXPECT_EQ ((Point (2, 4) *= 2.5) == Point (5, 10), true);

  EXPECT_EQ ((Point (1, 2) * 2) == Point (2, 4), true);
  EXPECT_EQ ((Point (1, 2) * 2.) == Point (2, 4), true);
  EXPECT_EQ ((Point (2, 4) * 2.5) == Point (5, 10), true);

  EXPECT_EQ ((2 * Point (1, 2)) == Point (2, 4), true);
  EXPECT_EQ ((2. * Point (1, 2)) == Point (2, 4), true);
  EXPECT_EQ ((2.5 * Point (2, 4)) == Point (5, 10), true);


  /// /
  EXPECT_EQ ((Point (1, 2) *= 2) == Point (2, 4), true);
  EXPECT_EQ ((Point (1, 2) *= 2.) == Point (2, 4), true);
  EXPECT_EQ ((Point (2, 4) *= 2.5) == Point (5, 10), true);
}
