#include "GL/Widget.h"
#include "GL/KeyEvent.h"
#include "GL/MouseEvent.h"
#include "GL/ResizeEvent.h"

namespace GL
{
Widget::Widget (Widget *parent) : m_size (80, 60), m_parent (parent) {}
Widget::~Widget () = default;

const Geometry::Size &Widget::size () const { return m_size; }

void Widget::setParent (Widget *parent) { m_parent = parent; }
GL::Window *Widget::window () const
{
  if (m_parent)
    return m_parent->window ();

  return nullptr;
}

void Widget::resizeEvent (const GL::ResizeEvent &event) { m_size = event.size (); }
void Widget::keyEvent (const GL::KeyEvent & /*event*/) {}
void Widget::mouseEvent (const GL::MouseEvent & /*event*/) {}
void Widget::renderEvent () {}
}  // namespace GL
