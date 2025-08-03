#pragma once

#include "Luma/Geom/UiRect_fwd.h"
#include "Luma/Geom/Vector.h"

namespace Luma::Geom
{
template <typename T>
struct UiRect
{
  Vector<T, 2> leftTop;
  Vector<T, 2> size;

  constexpr inline bool contains (const Vector<T, 2> &point) const;
  constexpr inline bool operator== (const UiRect &rhs) const;

  constexpr inline Vector<T, 2> rightBottom () const;

  constexpr inline UiRect &unit (const UiRect &);
  constexpr inline UiRect united (const UiRect &) const;
};

//-----------------------------------------------------------------

template<typename T>
constexpr inline bool UiRect<T>::contains (const Vector<T, 2> &point) const
{
  return    leftTop[0] <= point[0] &&  point[0] <= leftTop[0] + size[0]
         && leftTop[1] <= point[1] &&  point[1] <= leftTop[1] + size[1];
}

template<typename T>
constexpr inline bool UiRect<T>::operator== (const UiRect<T> &rhs) const
{
  return size == rhs.size && leftTop == rhs.leftTop;
}

template<typename T>
constexpr inline Vector<T, 2> UiRect<T>::rightBottom () const
{
  return leftTop + size;
}

template<typename T>
constexpr inline UiRect<T> &UiRect<T>::unit (const UiRect<T> &other)
{
  leftTop = min (leftTop, other.leftTop);
  Vector<T, 2> newRightBottom = max (rightBottom (), other.rightBottom ());
  size = newRightBottom - leftTop;
  return *this;
}

template<typename T>
constexpr inline UiRect<T> UiRect<T>::united (const UiRect<T> &other) const
{
  Vector<T, 2> newleftTop = min (leftTop, other.leftTop);
  Vector<T, 2> newRightBottom = max (rightBottom (), other.rightBottom ());
  return UiRect<T> (newleftTop, newRightBottom - newleftTop);
}
}  // namespace Luma::Geom
