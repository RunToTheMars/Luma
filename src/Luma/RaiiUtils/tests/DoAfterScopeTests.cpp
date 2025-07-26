#include "Luma/RaiiUtils/DoAfterScope.h"
#include <gtest/gtest.h>

TEST (DoAfterScope, MACRO)
{
  int x = 0;

  {
    DO_AFTER_SCOPE (x++);
  }
  EXPECT_EQ (x, 1);

  {
    DO_AFTER_SCOPE (x++);
    EXPECT_EQ (x, 1);
  }
}
