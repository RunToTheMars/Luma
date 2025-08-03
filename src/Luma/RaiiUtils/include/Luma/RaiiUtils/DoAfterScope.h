#pragma once

#include <utility>

namespace Luma::RaiiUtils
{
template<typename FUNC>
class DoAfterScope final
{
public:
  DoAfterScope (FUNC func) : mFunc (std::move (func)) {}
  ~DoAfterScope () { mFunc (); }

private:
  FUNC mFunc;
};
}

#define DO_AFTER_SCOPE_CAT_IMPL(X,Y) X ## Y
#define DO_AFTER_SCOPE_CAT(X,Y) DO_AFTER_SCOPE_CAT_IMPL(X,Y)
#define DO_AFTER_SCOPE(...) [[maybe_unused]] Luma::RaiiUtils::DoAfterScope DO_AFTER_SCOPE_CAT(doAfterScope,__LINE__) ([&] { __VA_ARGS__ ; });
