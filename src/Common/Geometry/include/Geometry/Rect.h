#pragma once

#include "Geometry/Point.h"
#include "Geometry/Size.h"

namespace Geometry
{
class Rect
{
public:
  constexpr inline Rect ();
  constexpr inline Rect (const Point &leftBottom, const Size &size);
  constexpr inline Rect (const Rect &);

  constexpr inline const Point &leftBottom () const;
  constexpr inline Point &rleftBottom ();

  constexpr inline const Size &size () const;
  constexpr inline Size &rsize ();

private:
  Point m_leftBottom;
  Size m_size;
};

//-----------------------------------------------------------------

constexpr inline Rect::Rect () = default;
constexpr inline Rect::Rect (const Point &leftBottom, const Size &size) : m_leftBottom (leftBottom), m_size (size) {}
constexpr inline Rect::Rect (const Rect &rect) : m_leftBottom (rect.m_leftBottom), m_size (rect.m_size) {}

constexpr inline const Point &Rect::leftBottom () const { return m_leftBottom; }
constexpr inline Point &Rect::rleftBottom () { return m_leftBottom; }

constexpr inline const Size &Rect::size () const { return m_size; }
constexpr inline Size &Rect::rsize () { return m_size; }

}  // namespace Geometry
