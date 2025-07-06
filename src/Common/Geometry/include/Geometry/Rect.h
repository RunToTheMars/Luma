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

  constexpr inline bool contains (const Point &point) const;

  constexpr inline bool operator== (const Rect &) const;

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

constexpr inline bool Rect::contains (const Point &point) const
{
  return    m_leftBottom.x () <= point.x () &&  point.x () <= m_leftBottom.x () + m_size.width ()
         && m_leftBottom.y () <= point.y () &&  point.y () <= m_leftBottom.y () + m_size.height ();
}

constexpr inline bool Rect::operator== (const Rect &rect) const
{
  return m_size == rect.m_size && m_leftBottom == rect.m_leftBottom;
}

}  // namespace Geometry
