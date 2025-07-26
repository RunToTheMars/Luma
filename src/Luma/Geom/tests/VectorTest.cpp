#include <gtest/gtest.h>
#include "Luma/Geom/Vector.h"

using namespace Luma::Geom;

TEST (Vec2I, Constructor)
{
  int x = 1;
  int y = 2;
  Vec2I p {x, y};
  EXPECT_EQ (p[0], x);
  EXPECT_EQ (p[1], y);

  EXPECT_EQ (p.data ()[0], x);
  EXPECT_EQ (p.data ()[1], y);
}

TEST (Vec2I, Copy)
{
  int x = 1;
  int y = 2;
  Vec2I p {x, y};

  {
    Vec2I p_copy = p;
    EXPECT_EQ (p_copy[0], x);
    EXPECT_EQ (p_copy[1], y);
  }

  {
    Vec2I p_copy;
    p_copy = p;
    EXPECT_EQ (p_copy[0], x);
    EXPECT_EQ (p_copy[1], y);
  }
}

TEST (Vec2I, Operator)
{
  bool ok = Vec2I {1, 2} == Vec2I {1, 2};
  /// ==
  EXPECT_EQ ((Vec2I {1, 2} == Vec2I {1, 2}), true);
  EXPECT_EQ ((Vec2I {1, 2} == Vec2I {1, 3}), false);
  EXPECT_EQ ((Vec2I {1, 2} == Vec2I {2, 2}), false);

  /// !=
  EXPECT_EQ ((Vec2I {1, 2} != Vec2I {1, 2}), false);
  EXPECT_EQ ((Vec2I {1, 2} != Vec2I {1, 3}), true);
  EXPECT_EQ ((Vec2I {1, 2} != Vec2I {2, 2}), true);

  /// +
  EXPECT_EQ (((Vec2I {1, 2} += Vec2I {3, 4}) == Vec2I {4, 6}), true);
  EXPECT_EQ (((Vec2I {1, 2} +  Vec2I {3, 4}) == Vec2I {4, 6}), true);

  /// -
  EXPECT_EQ (((Vec2I {1, 2} -= Vec2I {3, 4}) == Vec2I {-2, -2}), true);
  EXPECT_EQ (((Vec2I {1, 2} -  Vec2I {3, 4}) == Vec2I {-2, -2}), true);

  /// *
  EXPECT_EQ (((Vec2I {1, 2} *=   2) == Vec2I {2, 4 }), true);
  EXPECT_EQ (((Vec2I {1, 2} *= 2. ) == Vec2I {2, 4 }), true);
  EXPECT_EQ (((Vec2I {2, 4} *= 2.5) == Vec2I {5, 10}), true);

  EXPECT_EQ (((Vec2I {1, 2} * 2  ) == Vec2I {2, 4 }), true);
  EXPECT_EQ (((Vec2I {1, 2} * 2. ) == Vec2I {2, 4 }), true);
  EXPECT_EQ (((Vec2I {2, 4} * 2.5) == Vec2I {5, 10}), true);

  EXPECT_EQ (((2   * Vec2I {1, 2}) == Vec2I {2,  4}), true);
  EXPECT_EQ (((2.  * Vec2I {1, 2}) == Vec2I {2, 4 }), true);
  EXPECT_EQ (((2.5 * Vec2I {2, 4}) == Vec2I {5, 10}), true);

  ///
  EXPECT_EQ (((Vec2I {1, 2} *= 2  ) == Vec2I {2, 4 }), true);
  EXPECT_EQ (((Vec2I {1, 2} *= 2. ) == Vec2I {2, 4 }), true);
  EXPECT_EQ (((Vec2I {2, 4} *= 2.5) == Vec2I {5, 10}), true);
}
