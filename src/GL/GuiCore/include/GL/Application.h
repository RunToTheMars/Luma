#pragma once

#include "Common/Signal.h"

namespace GL
{
class Widget;
class Window;
class Monitor;
class MonitorList;

class Application
{
public:
  Application ();
  ~Application ();

  void runEventLoop ();
  void breakEventLoop ();

  // GL::Widget *focusWidget ();
  // void setFocusWidget (GL::Widget *widget);

  static GL::Window *focusWindow ();

  static Monitor primaryMonitor ();
  static MonitorList monitorList ();

  static Common::Signal<const Monitor &, bool> monitorEvent;

private:
  bool mInEventLoop = false;
};
}  // namespace GL
