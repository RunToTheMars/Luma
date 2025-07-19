#include "GL/Object.h"
#include "Common/Signal.h"
#include <vector>

namespace GL
{
class ObjectImpl
{
public:
  Object *parent = nullptr;
  bool inDelete = false;
  std::vector<Object *> childrens;

  Common::Signal<> destroyed;
};

Object::Object (Object *parent): mPimpl (std::make_unique<ObjectImpl> ())
{
  setParent (parent);
}

Object::~Object ()
{
  mPimpl->destroyed.notify ();
  mPimpl->inDelete = true;

  /// \note For removing "this" from parent childrens list
  if (mPimpl->parent && !mPimpl->parent->mPimpl->inDelete)
    setParent (nullptr);

  for (std::size_t i = 0; i < mPimpl->childrens.size (); i++)
    {
      Object *object = mPimpl->childrens[i];
      mPimpl->childrens[i] = nullptr;
      delete object;
    }

  mPimpl->childrens.clear ();
}

Object *Object::parent () const
{
  return mPimpl->parent;
}

void Object::setParent (Object *parent)
{
  if (mPimpl->parent)
    {
      std::vector<Object *> &parentChildrens = mPimpl->parent->mPimpl->childrens;
      for (auto it = parentChildrens.begin (); it != parentChildrens.end (); it++)
        {
          Object *object = *it;
          if (object != this)
            continue;

          parentChildrens.erase (it);
          break;
        }
    }

  mPimpl->parent = parent;

  if (mPimpl->parent)
    mPimpl->parent->mPimpl->childrens.push_back (this);
}

Common::Signal<> &Object::destroyed ()
{
  return mPimpl->destroyed;
}
}  // namespace GL
