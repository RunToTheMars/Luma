#pragma once

#include "Geometry/Size.h"

namespace GL
{
class ResizeEvent
{
public:
  ResizeEvent (const Geometry::Size &size) : mSize (size) {}
  ~ResizeEvent () = default;

  const Geometry::Size &size () const { return mSize; }

private:
  const Geometry::Size mSize;
};
}  // namespace GL
