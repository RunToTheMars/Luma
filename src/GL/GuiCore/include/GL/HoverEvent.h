#include "Geometry/Vector.h"

namespace Luma::Core
{
class HoverEvent
{
public:
  HoverEvent (const Geom::Vec2D &pos) : mPos (pos) {}
  ~HoverEvent () = default;

  const Geom::Vec2D &pos () const { return mPos; }

private:
  Geom::Vec2D mPos;
};
}  // namespace Luma::Core
