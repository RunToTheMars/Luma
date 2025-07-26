#pragma once

#include "Luma/Geom/Rect.h"

namespace Luma::Core
{
using namespace Luma::Geom;

class RenderEvent
{
public:
  RenderEvent (const RectI &rect) : mRect (rect) {}
  ~RenderEvent () = default;

  const RectI &rect () const { return mRect; }

private:
  RectI mRect;
};
}  // namespace Luma::Core
