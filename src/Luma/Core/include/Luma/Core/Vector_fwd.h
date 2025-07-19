#pragma once

#include <cstddef>

namespace Luma::Core
{
template<typename T, std::size_t N>
struct Vector;

using Vec1I = Luma::Core::Vector<int, 1>;
using Vec2I = Luma::Core::Vector<int, 2>;
using Vec3I = Luma::Core::Vector<int, 3>;
using Vec4I = Luma::Core::Vector<int, 4>;

using Vec1UI = Luma::Core::Vector<unsigned int, 1>;
using Vec2UI = Luma::Core::Vector<unsigned int, 2>;
using Vec3UI = Luma::Core::Vector<unsigned int, 3>;
using Vec4UI = Luma::Core::Vector<unsigned int, 4>;

using Vec1F = Luma::Core::Vector<float, 1>;
using Vec2F = Luma::Core::Vector<float, 2>;
using Vec3F = Luma::Core::Vector<float, 3>;
using Vec4F = Luma::Core::Vector<float, 4>;

using Vec1D = Luma::Core::Vector<double, 1>;
using Vec2D = Luma::Core::Vector<double, 2>;
using Vec3D = Luma::Core::Vector<double, 3>;
using Vec4D = Luma::Core::Vector<double, 4>;
}


