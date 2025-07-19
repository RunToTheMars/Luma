#pragma once

#include "Luma/Core/Vector.h"

namespace Luma::Core
{
class MoveEvent
{
public:
  MoveEvent (const Vec2I &pos) : mPos (pos) {}
  ~MoveEvent () = default;

  const Vec2I &pos () const { return mPos; }

private:
  Vec2I mPos;
};
}  // namespace Luma::Core
