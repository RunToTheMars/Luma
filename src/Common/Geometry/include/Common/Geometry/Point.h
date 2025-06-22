#pragma once

namespace Common
{
namespace Geometry
{
class Point
{
public:
  constexpr inline Point ();
  constexpr inline Point (int x, int y);
  constexpr inline Point (const Point &);

  constexpr inline bool isNull () const;

  constexpr inline int &rx ();
  constexpr inline int &ry ();

  constexpr inline int x () const;
  constexpr inline int y () const;

  constexpr inline Point transposed () const;
  constexpr inline void transpose ();

  constexpr inline Point &operator*= (int factor);
  constexpr inline Point &operator*= (double factor);

  constexpr inline Point &operator/= (int divisor);
  constexpr inline Point &operator/= (double divisor);

  constexpr inline Point &operator+= (const Point &point);
  constexpr inline Point &operator-= (const Point &point);

private:
  int m_x = 0;
  int m_y = 0;
};

constexpr inline Point operator+ (const Point &lhs, const Point &rhs);
constexpr inline Point operator- (const Point &lhs, const Point &rhs);

constexpr inline Point operator* (const Point &point, int factor);
constexpr inline Point operator* (const Point &point, double factor);

constexpr inline Point operator* (int factor, const Point &point);
constexpr inline Point operator* (double factor, const Point &point);

constexpr inline Point operator/ (const Point &point, int divisor);
constexpr inline Point operator/ (const Point &point, double divisor);

//-----------------------------------------------------------------

constexpr inline Point::Point () : m_x (0), m_y (0) {}
constexpr inline Point::Point (int x, int y) : m_x (x), m_y (y) {}
constexpr inline Point::Point (const Point &point) : m_x (point.x ()), m_y (point.y ()) {}

constexpr inline bool Point::isNull () const { return m_x == 0 && m_y == 0; }

constexpr inline int &Point::rx () { return m_x; }
constexpr inline int &Point::ry () { return m_y; }

constexpr inline int Point::x () const { return m_x; }
constexpr inline int Point::y () const { return m_y; }

constexpr inline Point Point::transposed () const { return Point (m_y, m_x); }
constexpr inline void Point::transpose ()
{
  int tmp = m_x;
  m_x = m_y;
  m_y = tmp;
}

constexpr inline Point &Point::operator*= (int factor)
{
  m_x *= factor;
  m_y *= factor;
  return *this;
}

constexpr inline Point &Point::operator*= (double factor)
{
  m_x = static_cast<int> (static_cast<double> (m_x) * factor);
  m_y = static_cast<int> (static_cast<double> (m_y) * factor);
  return *this;
}

constexpr inline Point &Point::operator/= (int divisor)
{
  m_x /= divisor;
  m_y /= divisor;
  return *this;
}

constexpr inline Point &Point::operator/= (double divisor)
{
  m_x = static_cast<int> (static_cast<double> (m_x) / divisor);
  m_y = static_cast<int> (static_cast<double> (m_y) / divisor);
  return *this;
}

constexpr inline Point &Point::operator+= (const Point &point)
{
  m_x += point.m_x;
  m_y += point.m_y;
  return *this;
}

constexpr inline Point &Point::operator-= (const Point &point)
{
  m_x -= point.m_x;
  m_y -= point.m_y;
  return *this;
}

constexpr inline Point operator+ (const Point &lhs, const Point &rhs) { return Point (lhs.x () + rhs.x (), lhs.y () + rhs.y ()); }
constexpr inline Point operator- (const Point &lhs, const Point &rhs) { return Point (lhs.x () - rhs.x (), lhs.y () - rhs.y ()); }

constexpr inline Point operator* (const Point &point, int factor) { return Point (point.x () * factor, point.y () * factor); }
constexpr inline Point operator* (const Point &point, double factor)
{
  return Point (static_cast<int> (static_cast<double> (point.x ()) * factor),
                static_cast<int> (static_cast<double> (point.y ()) * factor));
}

constexpr inline Point operator* (int factor, const Point &point) { return point * factor; }
constexpr inline Point operator* (double factor, const Point &point) { return point * factor; }

constexpr inline Point operator/ (const Point &point, int divisor) { return Point (point.x () / divisor, point.y () / divisor); }
constexpr inline Point operator/ (const Point &point, double divisor)
{
  return Point (static_cast<int> (static_cast<double> (point.x ()) / divisor),
                static_cast<int> (static_cast<double> (point.y ()) / divisor));
}
}  // namespace Geometry
}  // namespace Common
