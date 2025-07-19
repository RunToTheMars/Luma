#include "Geometry/Rect.h"

namespace GL
{
class RenderEvent
{
public:
  RenderEvent (const Geom::RectI &rect) : mRect (rect) {}
  ~RenderEvent () = default;

  const Geom::RectI &rect () const { return mRect; }

private:
  Geom::RectI mRect;
};
}
