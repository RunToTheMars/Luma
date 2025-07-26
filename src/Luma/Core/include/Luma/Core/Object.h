#pragma once

#include <memory>

#include "Luma/Signal/Signal_fwd.h"

namespace Luma::Core
{
using namespace Luma::Signal;

class ObjectImpl;

class Object
{
public:
  Object (Object *parent = nullptr);
  virtual ~Object ();

  Object *parent () const;
  void setParent (Object *parent);

  void deleteLater ();
  Luma::Core::Signal<> &destroyed ();

private:
  Object (const Object &) = delete;
  Object (Object &&) = delete;

  Object &operator= (const Object &) = delete;
  Object &operator= (Object &&) = delete;

  std::unique_ptr<ObjectImpl> mPimpl;
};
}  // namespace Luma::Core
