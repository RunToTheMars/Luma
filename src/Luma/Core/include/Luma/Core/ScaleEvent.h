#pragma once

#include "Luma/Geom/Vector.h"

namespace Luma::Core
{
using namespace Luma::Geom;

class ScaleEvent
{
public:
  ScaleEvent (const Vec2F &scale) : mScale (scale) {}
  ~ScaleEvent () = default;

  const Vec2F &scale () const { return mScale; }

private:
  Vec2F mScale;
};
}  // namespace Luma::Core
