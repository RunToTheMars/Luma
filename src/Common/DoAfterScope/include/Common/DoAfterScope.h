#pragma once

#include <utility>

namespace Details
{
template<typename FUNC>
class DoAfterScope
{
public:
  DoAfterScope (FUNC &&func) : m_func (std::move (func)) {}
  ~DoAfterScope () { m_func (); }

private:
  FUNC m_func;
};
}

#define DO_AFTER_SCOPE(...) [[maybe_unused]] Details::DoAfterScope doAfterScope##__LINE__ ([&] { __VA_ARGS__ ; });
