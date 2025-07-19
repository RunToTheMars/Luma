#pragma once

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

  static int exec ();
  static bool isExecuted ();
  static void exit (int returnCode = 0);
  static void quit ();

  static Window *focusWindow ();

  static Monitor primaryMonitor ();
  // static MonitorList monitorList ();
};
}  // namespace Luma::Core
