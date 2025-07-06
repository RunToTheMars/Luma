#pragma once

namespace Geometry
{
class Point
{
public:
  constexpr inline Point () noexcept;
  constexpr inline Point (int x, int y) noexcept;
  constexpr inline Point (const Point &) noexcept;

  constexpr inline bool isNull () const noexcept;

  constexpr const int *data () const noexcept;
  constexpr int *data () noexcept;

  constexpr inline int operator[] (int i) const noexcept;
  constexpr inline int &operator[] (int i) noexcept;

  constexpr static int size () noexcept;

  constexpr inline int &rx () noexcept;
  constexpr inline int &ry () noexcept;

  constexpr inline int x () const noexcept;
  constexpr inline int y () const noexcept;

  constexpr inline Point transposed () const noexcept;
  constexpr inline void transpose () noexcept;

  constexpr inline bool operator== (const Point &) const noexcept;

  constexpr inline Point &operator= (const Point &) noexcept;

  constexpr inline Point &operator*= (int factor) noexcept;
  constexpr inline Point &operator*= (double factor) noexcept;

  constexpr inline Point &operator/= (int divisor) noexcept;
  constexpr inline Point &operator/= (double divisor) noexcept;

  constexpr inline Point &operator+= (const Point &point) noexcept;
  constexpr inline Point &operator-= (const Point &point) noexcept;

private:
  int mData[2] = {0, 0};
};

constexpr inline Point operator+ (const Point &lhs, const Point &rhs) noexcept;
constexpr inline Point operator- (const Point &lhs, const Point &rhs) noexcept;

constexpr inline Point operator* (const Point &point, int factor) noexcept;
constexpr inline Point operator* (const Point &point, double factor) noexcept;

constexpr inline Point operator* (int factor, const Point &point) noexcept;
constexpr inline Point operator* (double factor, const Point &point) noexcept;

constexpr inline Point operator/ (const Point &point, int divisor) noexcept;
constexpr inline Point operator/ (const Point &point, double divisor) noexcept;

//-----------------------------------------------------------------

constexpr inline Point::Point () noexcept : mData {0, 0}
{
}

constexpr inline Point::Point (int x, int y) noexcept : mData {x, y}
{
}

constexpr inline Point::Point (const Point &point) noexcept : mData {mData[0], mData[1]}
{
}

constexpr inline bool Point::isNull () const noexcept
{
  return mData[0] == 0 && mData[1] == 0;
}

constexpr const int *Point::data () const noexcept
{
  return mData;
}

constexpr int *Point::data () noexcept
{
  return mData;
}

constexpr inline int Point::operator[] (int i) const noexcept
{
  return mData[i];
}

constexpr inline int &Point::operator[] (int i) noexcept
{
  return mData[i];
}

constexpr int Point::size () noexcept
{
  return 2;
}

constexpr inline int &Point::rx () noexcept
{
  return mData[0];
}

constexpr inline int &Point::ry () noexcept
{
  return mData[1];
}

constexpr inline int Point::x () const noexcept
{
  return mData[0];
}

constexpr inline int Point::y () const noexcept
{
  return mData[1];
}

constexpr inline Point Point::transposed () const noexcept
{
  return Point (mData[1], mData[0]);
}

constexpr inline void Point::transpose () noexcept
{
  int tmp = mData[0];
  mData[0] = mData[1];
  mData[1] = tmp;
}

constexpr inline bool Point::operator== (const Point &rhs) const noexcept
{
  return mData[0] == rhs.mData[0] && mData[1] == rhs.mData[1];
}

constexpr inline Point &Point::operator= (const Point &rhs) noexcept
{
  mData[0] = rhs.mData[0];
  mData[1] = rhs.mData[1];
  return *this;
}

constexpr inline Point &Point::operator*= (int factor) noexcept
{
  mData[0] *= factor;
  mData[1] *= factor;
  return *this;
}

constexpr inline Point &Point::operator*= (double factor) noexcept
{
  mData[0] = static_cast<int> (static_cast<double> (mData[0]) * factor);
  mData[1] = static_cast<int> (static_cast<double> (mData[1]) * factor);
  return *this;
}

constexpr inline Point &Point::operator/= (int divisor) noexcept
{
  mData[0] /= divisor;
  mData[1] /= divisor;
  return *this;
}

constexpr inline Point &Point::operator/= (double divisor) noexcept
{
  mData[0] = static_cast<int> (static_cast<double> (mData[0]) / divisor);
  mData[1] = static_cast<int> (static_cast<double> (mData[1]) / divisor);
  return *this;
}

constexpr inline Point &Point::operator+= (const Point &point) noexcept
{
  mData[0] += point.mData[0];
  mData[1] += point.mData[1];
  return *this;
}

constexpr inline Point &Point::operator-= (const Point &point) noexcept
{
  mData[0] -= point.mData[0];
  mData[1] -= point.mData[1];
  return *this;
}

constexpr inline Point operator+ (const Point &lhs, const Point &rhs) noexcept
{
  return Point (lhs.x () + rhs.x (), lhs.y () + rhs.y ());
}

constexpr inline Point operator- (const Point &lhs, const Point &rhs) noexcept
{
  return Point (lhs.x () - rhs.x (), lhs.y () - rhs.y ());
}

constexpr inline Point operator* (const Point &point, int factor) noexcept
{
  return Point (point.x () * factor, point.y () * factor);
}

constexpr inline Point operator* (const Point &point, double factor) noexcept
{
  return Point (static_cast<int> (static_cast<double> (point.x ()) * factor),
                static_cast<int> (static_cast<double> (point.y ()) * factor));
}

constexpr inline Point operator* (int factor, const Point &point) noexcept
{
  return point * factor;
}

constexpr inline Point operator* (double factor, const Point &point) noexcept
{
  return point * factor;
}

constexpr inline Point operator/ (const Point &point, int divisor) noexcept
{
  return Point (point.x () / divisor, point.y () / divisor);
}

constexpr inline Point operator/ (const Point &point, double divisor) noexcept
{
  return Point (static_cast<int> (static_cast<double> (point.x ()) / divisor),
                static_cast<int> (static_cast<double> (point.y ()) / divisor));
}
}  // namespace Geometry
