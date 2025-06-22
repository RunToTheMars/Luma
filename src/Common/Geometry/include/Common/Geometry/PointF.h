#pragma once

namespace Common
{
namespace Geometry
{
class PointF
{
public:
  constexpr inline PointF () noexcept;
  constexpr inline PointF (double x, double y) noexcept;
  constexpr inline PointF (const PointF &) noexcept;

  constexpr inline double x () const noexcept;
  constexpr inline double y () const noexcept;

  constexpr inline double &rx () const noexcept;
  constexpr inline double &ry () const noexcept;

  constexpr inline bool isNull () const noexcept;

  constexpr inline Point toPoint () const;

  constexpr inline PointF transposed () const noexcept;

  constexpr static inline double dotProduct (const PointF &, const PointF &);

  constexpr inline PointF &operator*= (double factor);
  constexpr inline PointF &operator/= (double factor);

private:
  double m_x;
  double m_y;
};

constexpr inline PointF::PointF () noexcept : m_x (0.0), m_y (0.0) {}
constexpr inline PointF::PointF (double x, double y) noexcept : m_x (x), m_y (y) {}
constexpr inline PointF::PointF (const PointF & point) noexcept : m_x (point.x ()), m_y (point.y ()) {}

constexpr inline double PointF::x () const noexcept { return m_x; }
constexpr inline double PointF::y () const noexcept { return m_y; }

constexpr inline double &PointF::rx () const noexcept { return m_x; }
constexpr inline double &PointF::ry () const noexcept { return m_y; }

constexpr inline bool PointF::isNull () const noexcept { return m_x == 0 && m_y == 0; }

constexpr inline Point PointF::toPoint () const
{
  return Point(static_cast<int> (m_x), static_cast<int> (m_y));
}

constexpr inline PointF transposed () const noexcept { return PointF (m_y, m_x); }

constexpr inline double PointF::dotProduct (const PointF & p1, const PointF & p2)
{
  return p1.m_x * p2.m_x + p1.m_y * p2.m_y;
}
} // namespace Geometry
} // namespace Common
