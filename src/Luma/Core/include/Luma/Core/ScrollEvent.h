#pragma once

#include "Luma/Geom/Vector.h"

namespace Luma::Core
{
using namespace Luma::Geom;

class ScrollEvent
{
public:
  ScrollEvent (const Vec2D &offset) : mOffset (offset) {}
  ~ScrollEvent () = default;

  const Vec2D &offset () const { return mOffset; }

private:
  Vec2D mOffset;
};
}  // namespace Luma::Core
