#include "GL/Widget.h"
#include "GL/KeyEvent.h"
#include "GL/MouseEvent.h"
#include "GL/ResizeEvent.h"

namespace GL
{
Widget::Widget (Widget *parent) : mSize (80, 60), mParent (parent)
{
}

Widget::~Widget () = default;

bool Widget::isEnabled () const
{
  return mIsEnabled;
}

void Widget::setEnabled (bool enable)
{
  mIsEnabled = enable;
}

bool Widget::isVisible () const
{
  return mIsVisible;
}

void Widget::setVisible (bool visible)
{
  mIsVisible = visible;
}

Geometry::Size Widget::size () const
{
  return mSize;
}

void Widget::setParent (Widget *parent)
{
  mParent = parent;
}

GL::Window *Widget::window () const
{
  if (mParent)
    return mParent->window ();

  return nullptr;
}

void Widget::resizeEvent (const GL::ResizeEvent &event)
{
  mSize = event.size ();
}

void Widget::keyEvent (const GL::KeyEvent & /*event*/)
{
}

void Widget::mouseEvent (const GL::MouseEvent & /*event*/)
{
}

void Widget::renderEvent ()
{
}
}  // namespace GL
