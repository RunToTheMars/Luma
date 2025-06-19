#include <gtest/gtest.h>
#include "Point.h"

TEST (PointDefaultCunstruct, Nullify)
{
  GL::Point p;
  EXPECT_EQ (p.x (), 0);
  EXPECT_EQ (p.y (), 0);
}

TEST (PointCunstruct, Correct)
{
  int x = 1;
  int y = 2;
  GL::Point p (x, y);
  EXPECT_EQ (p.x (), x);
  EXPECT_EQ (p.y (), y);
}
