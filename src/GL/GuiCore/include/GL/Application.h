#pragma once

#include "Common/Signal.h"

namespace Luma::Core
{
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

  static Window *focusWindow ();

  static Monitor primaryMonitor ();
  static MonitorList monitorList ();

  static Common::Signal<const Monitor &> monitorAdded;
  static Common::Signal<const Monitor &> monitorRemoved;

private:
  bool mInEventLoop = false;
};
}  // namespace Luma::Core
