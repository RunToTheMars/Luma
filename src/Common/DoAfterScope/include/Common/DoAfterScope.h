#pragma once

#include <utility>

namespace Details
{
template<typename FUNC>
class DoAfterScope
{
public:
  DoAfterScope (FUNC &&func) : mFunc (std::move (mFunc)) {}
  ~DoAfterScope () { mFunc (); }

private:
  FUNC mFunc;
};
}

#define DO_AFTER_SCOPE(...) [[maybe_unused]] Details::DoAfterScope doAfterScope##__LINE__ ([&] { __VA_ARGS__ ; });
