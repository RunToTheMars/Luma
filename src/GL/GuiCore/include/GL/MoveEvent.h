#include "Geometry/Vector.h"

namespace GL
{
class MoveEvent
{
public:
  MoveEvent (const Geom::Vec2I &pos) : mPos (pos) {}
  ~MoveEvent () = default;

  const Geom::Vec2I &pos () const { return mPos; }

private:
  Geom::Vec2I mPos;
};
}  // namespace GL
