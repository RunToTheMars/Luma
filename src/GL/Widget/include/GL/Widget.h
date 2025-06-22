#pragma once

#include "Geometry/Size.h"

namespace GL
{
class Window;
class ResizeEvent;
class KeyEvent;
class MouseEvent;

class Widget
{
public:
  Widget (Widget *parent = nullptr);
  virtual ~Widget ();

  Widget (const Widget &) = delete;
  Widget &operator= (const Widget &) = delete;

  Widget (Widget &&) = delete;
  Widget &operator= (Widget &&) = delete;

  const Geometry::Size &size () const;

  void setParent (Widget *parent);
  GL::Window *window () const;

protected:
  virtual void resizeEvent (const ResizeEvent &event);
  virtual void keyEvent (const KeyEvent &event);
  virtual void mouseEvent (const MouseEvent &event);
  virtual void renderEvent ();

private:
  Widget *m_parent = nullptr;
  Geometry::Size m_size;
};

}  // namespace GL
