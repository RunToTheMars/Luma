#pragma once

#include <math.h>

inline bool almostEqual (float a, float b, float eps = 1e-6)
{
  return std::fabs (a - b) < eps;
}
