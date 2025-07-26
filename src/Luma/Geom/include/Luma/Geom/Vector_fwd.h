#pragma once

#include <cstddef>

namespace Luma::Geom
{
template<typename T, std::size_t N>
struct Vector;

using Vec1I = Vector<int, 1>;
using Vec2I = Vector<int, 2>;
using Vec3I = Vector<int, 3>;
using Vec4I = Vector<int, 4>;

using Vec1UI = Vector<unsigned int, 1>;
using Vec2UI = Vector<unsigned int, 2>;
using Vec3UI = Vector<unsigned int, 3>;
using Vec4UI = Vector<unsigned int, 4>;

using Vec1F = Vector<float, 1>;
using Vec2F = Vector<float, 2>;
using Vec3F = Vector<float, 3>;
using Vec4F = Vector<float, 4>;

using Vec1D = Vector<double, 1>;
using Vec2D = Vector<double, 2>;
using Vec3D = Vector<double, 3>;
using Vec4D = Vector<double, 4>;
}


