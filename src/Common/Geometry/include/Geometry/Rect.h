#pragma once

#include "Geometry/Rect_fwd.h"
#include "Geometry/Vector.h"

namespace Geom
{
template <typename T>
struct Rect
{
  Geom::Vector<T, 2> leftTop;
  Geom::Vector<T, 2> size;

  constexpr inline bool contains (const Geom::Vector<T, 2> &point) const;
  constexpr inline bool operator== (const Rect &rhs) const;
};

//-----------------------------------------------------------------

template<typename T>
constexpr inline bool Rect<T>::contains (const Geom::Vector<T, 2> &point) const
{
  return    leftTop[0] <= point[0] &&  point[0] <= leftTop[0] + size[0]
         && leftTop[1] <= point[1] &&  point[1] <= leftTop[1] + size[1];
}

template<typename T>
constexpr inline bool Rect<T>::operator== (const Rect<T> &rhs) const
{
  return size == rhs.size && leftTop == rhs.leftTop;
}

}  // namespace Geometry
