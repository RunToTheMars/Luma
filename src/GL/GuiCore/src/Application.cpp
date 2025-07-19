#include "GL/Application.h"
#include "ApplicationPrivate.h"
#include "Common/Signal.h"
#include "GL/Monitor.h"
#include "GL/Window.h"
#include "ImplCast.inl"
#include <GLFW/glfw3.h>
#include <list>

namespace GL
{
Common::Signal<const Monitor &> Application::monitorAdded;
Common::Signal<const Monitor &> Application::monitorRemoved;
}

namespace
{
void MonitorEventHandle (GLFWmonitor *monitor, int event)
{
  GL::Monitor *GLmonitor = toGLmonitor (monitor);
  if (event == GLFW_CONNECTED)
    GL::Application::monitorAdded.notify (*GLmonitor);
  else
    GL::Application::monitorRemoved.notify (*GLmonitor);
}
}  // namespace

namespace GL
{
Application::Application ()
{
  glfwInit ();
  glfwSetMonitorCallback (&MonitorEventHandle);
}

Application::~Application ()
{
  glfwTerminate ();
}

void Application::runEventLoop ()
{
  if (mInEventLoop)
    return;

  mInEventLoop = true;
  const std::unordered_set<GL::Window *> &allWindows = GL::ApplicationPrivate::allWindows;

  while (mInEventLoop && !allWindows.empty ())
    {
      for (GL::Window *window : allWindows)
        {
          if (!window->isVisible () || window->isIconified ())
            continue;

          if (window->mUpdateNeeded)
            {
              window->mUpdateNeeded = false;
              window->repaint ();
            }
        }
      glfwPollEvents ();
    }

  mInEventLoop = false;
}

void Application::breakEventLoop ()
{
  mInEventLoop = false;
}

// GL::Widget *focusWidget ()
// {
//   return gFocusWidget;
// }

// void setFocusWidget (GL::Widget *widget)
// {
//   if (gFocusWidget == widget)
//     return;

//   GL::Widget *oldFocusWidget = gFocusWidget;
//   gFocusWidget = widget;

//   gFocusWidgetChanged.notify (oldFocusWidget, widget);
// }

GL::Window *Application::focusWindow ()
{
  return GL::ApplicationPrivate::focusWindow;
}

Monitor Application::primaryMonitor ()
{
  return Monitor (glfwGetPrimaryMonitor ());
}
}
