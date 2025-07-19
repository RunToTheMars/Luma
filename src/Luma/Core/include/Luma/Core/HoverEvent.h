#pragma once

#include "Luma/Core/Vector.h"

namespace Luma::Core
{
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
