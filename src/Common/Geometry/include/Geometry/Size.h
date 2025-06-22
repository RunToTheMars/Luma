#pragma once

namespace Geometry
{
class Size
{
public:
  constexpr inline Size ();
  constexpr inline Size (int width, int height);
  constexpr inline Size (const Size &);

  constexpr inline bool isEmpty () const;
  constexpr inline bool isValid () const;

  constexpr inline int &rwidth ();
  constexpr inline int &rheight ();

  constexpr inline int width () const;
  constexpr inline int height () const;

  constexpr inline void setWidth (int width);
  constexpr inline void setHeight (int height);

  constexpr inline Size transposed () const;
  constexpr inline void transpose ();

  constexpr inline Size &operator *= (int factor);
  constexpr inline Size &operator *= (double factor);

  constexpr inline Size &operator /= (int divisor);
  constexpr inline Size &operator /= (double divisor);

private:
  int m_width = 0;
  int m_height = 0;
};

constexpr inline Size operator* (const Size &size, int factor);
constexpr inline Size operator* (const Size &size, double factor);

constexpr inline Size operator* (int factor, const Size &size);
constexpr inline Size operator* (double factor, const Size &size);

constexpr inline Size operator/ (const Size &size, int divisor);
constexpr inline Size operator/ (const Size &size, double divisor);

//-----------------------------------------------------------------

constexpr inline Size::Size () : m_width (0), m_height (0) {}
constexpr inline Size::Size (int width, int height) : m_width (width), m_height (height) {}
constexpr inline Size::Size (const Size &size) : m_width (size.width ()), m_height (size.height ()) {}

constexpr inline bool Size::isEmpty () const { return m_width <= 0 && m_height <= 0; }
constexpr inline bool Size::isValid () const { return m_width >= 0 && m_height >= 0; }

constexpr inline int &Size::rwidth () { return m_width; }
constexpr inline int &Size::rheight () { return m_height; }

constexpr inline int Size::width () const { return m_width; }
constexpr inline int Size::height () const { return m_height; }

constexpr inline void Size::setWidth (int width) { m_width = width; }
constexpr inline void Size::setHeight (int height) { m_height = height; }

constexpr inline Size Size::transposed () const { return Size (m_height, m_width); }
constexpr inline void Size::transpose ()
{
  int tmp = m_width;
  m_width = m_height;
  m_height = tmp;
}

constexpr inline Size &Size::operator*= (int factor)
{
  m_width *= factor;
  m_height *= factor;
  return *this;
}

constexpr inline Size &Size::operator*= (double factor)
{
  m_width = static_cast<int> (static_cast<double> (m_width) * factor);
  m_height = static_cast<int> (static_cast<double> (m_height) * factor);
  return *this;
}

constexpr inline Size &Size::operator/= (int divisor)
{
  m_width /= divisor;
  m_height /= divisor;
  return *this;
}

constexpr inline Size &Size::operator/= (double divisor)
{
  m_width = static_cast<int> (static_cast<double> (m_width) / divisor);
  m_height = static_cast<int> (static_cast<double> (m_height) / divisor);
  return *this;
}

constexpr inline Size operator* (const Size &size, int factor) { return Size (size.width () * factor, size.height () * factor); }
constexpr inline Size operator* (const Size &size, double factor)
{
  return Size (static_cast<int> (static_cast<double> (size.width ()) * factor),
               static_cast<int> (static_cast<double> (size.height ()) * factor));
}

constexpr inline Size operator* (int factor, const Size &size) { return size * factor; }
constexpr inline Size operator* (double factor, const Size &size) { return size * factor; }

constexpr inline Size operator/ (const Size &size, int divisor) { return Size (size.width () / divisor, size.height () / divisor); }
constexpr inline Size operator/ (const Size &size, double divisor)
{
  return Size (static_cast<int> (static_cast<double> (size.width ()) / divisor),
               static_cast<int> (static_cast<double> (size.height ()) / divisor));
}
} // namespace Geometry
