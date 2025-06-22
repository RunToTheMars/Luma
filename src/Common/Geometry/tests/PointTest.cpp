#include <gtest/gtest.h>
#include "Geometry/Point.h"

TEST (PointDefaultCunstruct, Nullify)
{
  Geometry::Point p;
  EXPECT_EQ (p.x (), 0);
  EXPECT_EQ (p.y (), 0);
}

TEST (PointCunstruct, Correct)
{
  int x = 1;
  int y = 2;
  Geometry::Point p (x, y);
  EXPECT_EQ (p.x (), x);
  EXPECT_EQ (p.y (), y);
}
