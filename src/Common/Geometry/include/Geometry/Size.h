#pragma once

namespace Geometry
{
class Size
{
public:
  constexpr inline Size ();
  constexpr inline Size (int width, int height);
  constexpr inline Size (const Size &);

  constexpr inline Size &operator= (const Size &);

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

  constexpr inline bool operator== (const Size &) const;

  constexpr inline Size &operator *= (int factor);
  constexpr inline Size &operator *= (double factor);

  constexpr inline Size &operator /= (int divisor);
  constexpr inline Size &operator /= (double divisor);

private:
  int mWidth = 0;
  int mHeight = 0;
};

constexpr inline Size operator* (const Size &size, int factor);
constexpr inline Size operator* (const Size &size, double factor);

constexpr inline Size operator* (int factor, const Size &size);
constexpr inline Size operator* (double factor, const Size &size);

constexpr inline Size operator/ (const Size &size, int divisor);
constexpr inline Size operator/ (const Size &size, double divisor);

//-----------------------------------------------------------------

constexpr inline Size::Size () : mWidth (0), mHeight (0) {}
constexpr inline Size::Size (int width, int height) : mWidth (width), mHeight (height) {}
constexpr inline Size::Size (const Size &size) : mWidth (size.width ()), mHeight (size.height ()) {}

constexpr inline Size &Size::operator= (const Size &rhs)
{
  mWidth = rhs.mWidth;
  mHeight = rhs.mHeight;
  return *this;
}

constexpr inline bool Size::isEmpty () const { return mWidth <= 0 && mHeight <= 0; }
constexpr inline bool Size::isValid () const { return mWidth >= 0 && mHeight >= 0; }

constexpr inline int &Size::rwidth () { return mWidth; }
constexpr inline int &Size::rheight () { return mHeight; }

constexpr inline int Size::width () const { return mWidth; }
constexpr inline int Size::height () const { return mHeight; }

constexpr inline void Size::setWidth (int width) { mWidth = width; }
constexpr inline void Size::setHeight (int height) { mHeight = height; }

constexpr inline Size Size::transposed () const { return Size (mHeight, mWidth); }
constexpr inline void Size::transpose ()
{
  int tmp = mWidth;
  mWidth = mHeight;
  mHeight = tmp;
}

constexpr inline bool Size::operator== (const Size &size) const
{
  return mWidth == size.mWidth && mHeight == size.mHeight;
}

constexpr inline Size &Size::operator*= (int factor)
{
  mWidth *= factor;
  mHeight *= factor;
  return *this;
}

constexpr inline Size &Size::operator*= (double factor)
{
  mWidth = static_cast<int> (static_cast<double> (mWidth) * factor);
  mHeight = static_cast<int> (static_cast<double> (mHeight) * factor);
  return *this;
}

constexpr inline Size &Size::operator/= (int divisor)
{
  mWidth /= divisor;
  mHeight /= divisor;
  return *this;
}

constexpr inline Size &Size::operator/= (double divisor)
{
  mWidth = static_cast<int> (static_cast<double> (mWidth) / divisor);
  mHeight = static_cast<int> (static_cast<double> (mHeight) / divisor);
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
