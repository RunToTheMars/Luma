#pragma once

#include "Luma/Geom/Rect_fwd.h"
#include "Luma/Geom/Vector.h"

namespace Luma::Geom
{
template <typename T>
struct Rect
{
  Vector<T, 2> leftTop;
  Vector<T, 2> size;

  constexpr inline bool contains (const Vector<T, 2> &point) const;
  constexpr inline bool operator== (const Rect &rhs) const;

  constexpr inline Vector<T, 2> rightBottom () const;

  constexpr inline Rect &unit (const Rect &);
  constexpr inline Rect united (const Rect &) const;
};

//-----------------------------------------------------------------

template<typename T>
constexpr inline bool Rect<T>::contains (const Vector<T, 2> &point) const
{
  return    leftTop[0] <= point[0] &&  point[0] <= leftTop[0] + size[0]
         && leftTop[1] <= point[1] &&  point[1] <= leftTop[1] + size[1];
}

template<typename T>
constexpr inline bool Rect<T>::operator== (const Rect<T> &rhs) const
{
  return size == rhs.size && leftTop == rhs.leftTop;
}

template<typename T>
constexpr inline Vector<T, 2> Rect<T>::rightBottom () const
{
  return leftTop + size;
}

template<typename T>
constexpr inline Rect<T> &Rect<T>::unit (const Rect<T> &other)
{
  leftTop = min (leftTop, other.leftTop);
  Vector<T, 2> newRightBottom = max (rightBottom (), other.rightBottom ());
  size = newRightBottom - leftTop;
  return *this;
}

template<typename T>
constexpr inline Rect<T> Rect<T>::united (const Rect<T> &other) const
{
  Vector<T, 2> newleftTop = min (leftTop, other.leftTop);
  Vector<T, 2> newRightBottom = max (rightBottom (), other.rightBottom ());
  return Rect<T> (newleftTop, newRightBottom - newleftTop);
}
}  // namespace Luma::Geom
