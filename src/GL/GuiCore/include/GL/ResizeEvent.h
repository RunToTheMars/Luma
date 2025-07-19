#include "Geometry/Vector.h"

namespace GL
{
class ResizeEvent
{
public:
  ResizeEvent (const Geom::Vec2I &size) : mSize (size) {}
  ~ResizeEvent () = default;

  const Geom::Vec2I &size () const { return mSize; }

private:
  Geom::Vec2I mSize;
};
}
