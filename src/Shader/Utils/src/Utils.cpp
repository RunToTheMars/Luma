#pragma once

#include <math.h>

namespace Shader::Utils
{
bool almostEqual (float a, float b, float eps)
{
  return std::fabs (a - b) < eps;
}
}
