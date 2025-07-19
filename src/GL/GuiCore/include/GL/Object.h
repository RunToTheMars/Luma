#pragma once

#include <memory>

namespace Common
{
template<typename... Args>
class Signal;
}

namespace GL
{
class ObjectImpl;
class Object
{
public:
  Object (Object *parent = nullptr);
  virtual ~Object ();

  Object *parent () const;
  void setParent (Object *parent);

  Common::Signal<> &destroyed ();

private:
  std::unique_ptr<ObjectImpl> mPimpl;
};
}  // namespace GL
