#pragma once

#include <utility>

namespace utils
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

#define DO_AFTER_SCOPE(...) [[maybe_unused]] utils::DoAfterScope do_after_scope_##__LINE__ ([&] { __VA_ARGS__ ; });
}  // namespace utils
