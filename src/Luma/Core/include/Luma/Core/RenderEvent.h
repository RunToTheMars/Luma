#pragma once

#include "Luma/Core/Rect.h"

namespace Luma::Core
{
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
