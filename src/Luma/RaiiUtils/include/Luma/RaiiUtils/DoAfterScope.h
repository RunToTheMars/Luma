#pragma once

#include <utility>

namespace Luma::RaiiUtils
{
template<typename FUNC>
class DoAfterScope final
{
public:
  DoAfterScope (const FUNC &func) : mFunc (func) {}
  ~DoAfterScope () { mFunc (); }

private:
  const FUNC &mFunc;
};
}

#define DO_AFTER_SCOPE(...) [[maybe_unused]] Luma::RaiiUtils::DoAfterScope doAfterScope##__LINE__ ([&] { __VA_ARGS__ ; });
