#pragma once

#include "GL/KeyEvent.h"
#include "GL/MouseEvent.h"
#include "GL/ResizeEvent.h"
#include <memory>

namespace GL
{
class WidgetImpl;
class Widget
{
public:
  Widget (Widget *parent);
  ~Widget ();

  const Widget *parent () const;
  Widget *parent ();

  void setGeometry (int x, int y, int width, int height);

  int x () const;
  int y () const;
  int width () const;
  int height () const;

  void mapFrom (Widget *widget, int x, int y);

private:
  virtual void resizeEvent (const ResizeEvent &event);
  virtual void keyEvent (const KeyEvent &event);
  virtual void mouseEvent (const MouseEvent &event);
  virtual void renderEvent ();

private:
  std::unique_ptr<WidgetImpl> m_pimpl;
};
}  // namespace GL
