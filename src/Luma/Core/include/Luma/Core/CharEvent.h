#pragma once

namespace Luma::Core
{
class CharEvent
{
public:
  CharEvent (unsigned int code) : mCode (code) {}
  ~CharEvent () = default;

  const unsigned int code () const { return mCode; }

private:
  const unsigned int mCode;
};
}  // namespace Luma::Core
