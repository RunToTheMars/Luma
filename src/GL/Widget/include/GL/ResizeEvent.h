#pragma once

#include "Geometry/Size.h"

namespace GL
{
class ResizeEvent
{
public:
  ResizeEvent (const Geometry::Size &size) : m_size (size) {}
  ~ResizeEvent () = default;

  const Geometry::Size &size () const { return m_size; }

private:
  const Geometry::Size m_size;
};
}  // namespace GL
