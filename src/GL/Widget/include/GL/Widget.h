#pragma once

#include "Common/Signal.h"
#include "Geometry/Rect.h"
#include <memory>
#include <vector>

namespace GL
{
class KeyEvent;
class MouseEvent;
class WindowCreateConfig;

class Widget
{
public:
  Widget () noexcept;
  virtual ~Widget () noexcept;

  Widget (const Widget &) = delete;
  Widget &operator= (const Widget &) = delete;

  Widget (Widget &&) = delete;
  Widget &operator= (Widget &&) = delete;

  bool isVisible () const;
  void setVisible (bool visible);

  Geometry::Rect rect () const;
  void setRect (const Geometry::Rect &rect);

  void addWidget (std::unique_ptr<Widget> widget);
  bool hasWidget (const Widget *widget) const;
  std::unique_ptr<Widget> releaseWidget (const Widget *widget);

/// signals
public:
  Common::Signal<Geometry::Rect> rectChanged;

protected:
  virtual void keyEvent (const KeyEvent &event);
  virtual bool mouseEvent (const MouseEvent &event);
  virtual void renderEvent ();

private:
  friend class WindowCreateConfig;
  friend class Window;
  void keyEventPrivate (const KeyEvent &event);
  bool mouseEventPrivate (const MouseEvent &event);
  void renderEventPrivate ();

private:
  Widget *mParent = nullptr;
  bool mIsVisible = true;

  Geometry::Rect mRect;
  std::vector<std::unique_ptr<Widget>> mChildrens;
};

}  // namespace GL
