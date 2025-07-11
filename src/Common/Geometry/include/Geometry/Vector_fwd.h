#pragma once

#include <cstddef>

namespace Geom
{
template<typename T, std::size_t N>
struct Vector;

using Vec1I = Geom::Vector<int, 1>;
using Vec2I = Geom::Vector<int, 2>;
using Vec3I = Geom::Vector<int, 3>;
using Vec4I = Geom::Vector<int, 4>;

using Vec1UI = Geom::Vector<unsigned int, 1>;
using Vec2UI = Geom::Vector<unsigned int, 2>;
using Vec3UI = Geom::Vector<unsigned int, 3>;
using Vec4UI = Geom::Vector<unsigned int, 4>;

using Vec1F = Geom::Vector<float, 1>;
using Vec2F = Geom::Vector<float, 2>;
using Vec3F = Geom::Vector<float, 3>;
using Vec4F = Geom::Vector<float, 4>;

using Vec1D = Geom::Vector<double, 1>;
using Vec2D = Geom::Vector<double, 2>;
using Vec3D = Geom::Vector<double, 3>;
using Vec4D = Geom::Vector<double, 4>;
}


