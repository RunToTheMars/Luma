#pragma once

#include "Luma/Core/Vector.h"

namespace Luma::Core
{
class ResizeEvent
{
public:
  ResizeEvent (const Vec2I &size) : mSize (size) {}
  ~ResizeEvent () = default;

  const Vec2I &size () const { return mSize; }

private:
  Vec2I mSize;
};
}  // namespace Luma::Core
