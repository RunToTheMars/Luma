#pragma once

#include "Luma/Geom/Vector_fwd.h"
#include <cstring>

namespace Luma::Geom
{

template<typename T, std::size_t N>
struct Vector
{
  constexpr inline T* data () noexcept;
  constexpr inline const T* data () const noexcept;

  constexpr inline std::size_t size () const noexcept;

  constexpr inline bool operator== (const Vector<T, N> &rhs) const noexcept;

  constexpr inline T operator[] (std::size_t i) const noexcept;
  constexpr inline T &operator[] (std::size_t i) noexcept;

  constexpr inline T at (std::size_t i) const noexcept;
  constexpr inline T &at (std::size_t i) noexcept;

  constexpr inline Vector<T, N> &operator+= (const Vector<T, N> &rhs) noexcept;
  constexpr inline Vector<T, N> &operator-= (const Vector<T, N> &rhs) noexcept;
  constexpr inline Vector<T, N> &operator*= (const Vector<T, N> &rhs) noexcept;
  constexpr inline Vector<T, N> &operator/= (const Vector<T, N> &rhs) noexcept;

  template <typename ScalarT>
  constexpr inline Vector<T, N> &operator*= (ScalarT rhs) noexcept;

  template <typename ScalarT>
  constexpr inline Vector<T, N> &operator/= (ScalarT rhs) noexcept;

  T __mPrivateData[N];
};

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator+ (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept;

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator- (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept;

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator* (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept;

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator/ (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept;

template<typename T, std::size_t N, typename ScalarT>
constexpr inline const Vector<T, N> operator* (const Vector<T, N> &lhs, ScalarT rhs) noexcept;

template<typename T, std::size_t N, typename ScalarT>
constexpr inline const Vector<T, N> operator* (ScalarT lhs, const Vector<T, N> &rhs) noexcept;

template<typename T, std::size_t N, typename ScalarT>
constexpr inline const Vector<T, N> operator/ (const Vector<T, N> &lhs, ScalarT rhs) noexcept;

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> min (const Vector<T, N> &vec1, const Vector<T, N> &vec2) noexcept;

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> max (const Vector<T, N> &vec1, const Vector<T, N> &vec2) noexcept;

//-----------------------------------------------------------------

template<typename T, std::size_t N>
constexpr inline T* Vector<T, N>::data () noexcept
{
  return __mPrivateData;
}

template<typename T, std::size_t N>
constexpr inline const T* Vector<T, N>::data () const noexcept
{
  return __mPrivateData;
}

template<typename T, std::size_t N>
constexpr inline std::size_t Vector<T, N>::size () const noexcept
{
  return N;
}

template<typename T, std::size_t N>
constexpr inline bool Vector<T, N>::operator== (const Vector<T, N> &rhs) const noexcept
{
  return std::memcmp (__mPrivateData, rhs.__mPrivateData, sizeof (__mPrivateData)) == 0;
}

template<typename T, std::size_t N>
constexpr inline T Vector<T, N>::operator[] (std::size_t i) const noexcept
{
  return __mPrivateData[i];
}

template<typename T, std::size_t N>
constexpr inline T &Vector<T, N>::operator[] (std::size_t i) noexcept
{
  return __mPrivateData[i];
}

template<typename T, std::size_t N>
constexpr inline T Vector<T, N>::at (std::size_t i) const noexcept
{
  return __mPrivateData[i];
}

template<typename T, std::size_t N>
constexpr inline T &Vector<T, N>::at (std::size_t i) noexcept
{
  return __mPrivateData[i];
}

template<typename T, std::size_t N>
constexpr inline Vector<T, N> &Vector<T, N>::operator+= (const Vector<T, N> &rhs) noexcept
{
  for (std::size_t i = 0; i < N; i++)
    __mPrivateData[i] += rhs.__mPrivateData[i];
  return *this;
}

template<typename T, std::size_t N>
constexpr inline Vector<T, N> &Vector<T, N>::operator-= (const Vector<T, N> &rhs) noexcept
{
  for (std::size_t i = 0; i < N; i++)
    __mPrivateData[i] -= rhs.__mPrivateData[i];
  return *this;
}

template<typename T, std::size_t N>
constexpr inline Vector<T, N> &Vector<T, N>::operator*= (const Vector<T, N> &rhs) noexcept
{
  for (std::size_t i = 0; i < N; i++)
    __mPrivateData[i] *= rhs.__mPrivateData[i];
  return *this;
}

template<typename T, std::size_t N>
constexpr inline Vector<T, N> &Vector<T, N>::operator/= (const Vector<T, N> &rhs) noexcept
{
  for (std::size_t i = 0; i < N; i++)
    __mPrivateData[i] /= rhs.__mPrivateData[i];
  return *this;
}


template<typename T, std::size_t N>
template<typename ScalarT>
constexpr inline Vector<T, N> &Vector<T, N>::operator*= (ScalarT rhs) noexcept
{
  for (std::size_t i = 0; i < N; i++)
    __mPrivateData[i] *= rhs;
  return *this;
}

template<typename T, std::size_t N>
template<typename ScalarT>
constexpr inline Vector<T, N> &Vector<T, N>::operator/= (ScalarT rhs) noexcept
{
  for (std::size_t i = 0; i < N; i++)
    __mPrivateData[i] /= rhs;
  return *this;
}

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator+ (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = lhs.__mPrivateData[i] + rhs.__mPrivateData[i];
  return res;
}

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator- (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = lhs.__mPrivateData[i] - rhs.__mPrivateData[i];
  return res;
}

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator* (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = lhs.__mPrivateData[i] * rhs.__mPrivateData[i];
  return res;
}

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> operator/ (const Vector<T, N> &lhs, const Vector<T, N> &rhs) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = lhs.__mPrivateData[i] / rhs.__mPrivateData[i];
  return res;
}

template<typename T, std::size_t N, typename ScalarT>
constexpr inline const Vector<T, N> operator* (const Vector<T, N> &lhs, ScalarT rhs) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = lhs.__mPrivateData[i] * rhs;
  return res;
}

template<typename T, std::size_t N, typename ScalarT>
constexpr inline const Vector<T, N> operator* (ScalarT lhs, const Vector<T, N> &rhs) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = lhs * rhs.__mPrivateData[i];
  return res;
}

template<typename T, std::size_t N, typename ScalarT>
constexpr inline const Vector<T, N> operator/ (const Vector<T, N> &lhs, ScalarT rhs) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = lhs.__mPrivateData[i] / rhs;
  return res;
}

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> min (const Vector<T, N> &vec1, const Vector<T, N> &vec2) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = vec1[i] < vec2[i] ? vec1[i] : vec2[i];
  return res;
}

template<typename T, std::size_t N>
constexpr inline const Vector<T, N> max (const Vector<T, N> &vec1, const Vector<T, N> &vec2) noexcept
{
  Vector<T, N> res;
  for (std::size_t i = 0; i < N; i++)
    res.__mPrivateData[i] = vec1[i] < vec2[i] ? vec2[i] : vec1[i];
  return res;
}

}
