#pragma once

#include "Luma/Geom/Rect_fwd.h"
#include "Luma/Geom/Vector.h"

namespace Luma::Geom
{
template <typename T>
struct Rect
{
  Vector<T, 2> leftBottom;
  Vector<T, 2> size;

  constexpr inline bool contains (const Vector<T, 2> &point) const;
  constexpr inline bool operator== (const Rect &rhs) const;

  constexpr inline Vector<T, 2> rightTop () const;

  constexpr inline Rect &unit (const Rect &);
  constexpr inline Rect united (const Rect &) const;
};

//-----------------------------------------------------------------

template<typename T>
constexpr inline bool Rect<T>::contains (const Vector<T, 2> &point) const
{
  return    leftBottom[0] <= point[0] &&  point[0] <= leftBottom[0] + size[0]
         && leftBottom[1] <= point[1] &&  point[1] <= leftBottom[1] + size[1];
}

template<typename T>
constexpr inline bool Rect<T>::operator== (const Rect<T> &rhs) const
{
  return size == rhs.size && leftBottom == rhs.leftBottom;
}

template<typename T>
constexpr inline Vector<T, 2> Rect<T>::rightTop () const
{
  return leftBottom + size;
}

template<typename T>
constexpr inline Rect<T> &Rect<T>::unit (const Rect<T> &other)
{
  leftBottom = min (leftBottom, other.leftBottom);
  Vector<T, 2> newRightTop = max (rightTop (), other.rightTop ());
  size = newRightTop - leftBottom;
  return *this;
}

template<typename T>
constexpr inline Rect<T> Rect<T>::united (const Rect<T> &other) const
{
  Vector<T, 2> newleftBottom = min (leftBottom, other.leftBottom);
  Vector<T, 2> newRightTop = max (rightTop (), other.rightTop ());
  return Rect<T> (newleftBottom, newRightTop - newleftBottom);
}
}  // namespace Luma::Geom
