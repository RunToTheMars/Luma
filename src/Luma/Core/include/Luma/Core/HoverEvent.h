#pragma once

#include "Luma/Geom/Vector.h"

namespace Luma::Core
{
using namespace Luma::Geom;

class HoverEvent
{
public:
  HoverEvent (const Vec2D &pos) : mPos (pos) {}
  ~HoverEvent () = default;

  const Vec2D &pos () const { return mPos; }

private:
  Vec2D mPos;
};
}  // namespace Luma::Core
