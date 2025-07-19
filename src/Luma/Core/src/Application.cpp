#include "Luma/Core/Application.h"
#include "ApplicationImpl.h"
#include "Luma/Core/Monitor.h"
#include "Luma/Core/Object.h"
#include "Luma/Core/Window.h"
#include <GLFW/glfw3.h>

namespace Luma::Core
{
bool ApplicationImpl::isExecuted = false;
bool ApplicationImpl::exit = false;
int ApplicationImpl::returnCode = -1;

std::unordered_set<Window *> ApplicationImpl::allWindows;
std::unordered_set<Window *> ApplicationImpl::allApplicationModalWindows;
Window *ApplicationImpl::focusWindow = nullptr;

std::vector<Window *> ApplicationImpl::windowsForRepaint[2];
std::vector<Window *> *ApplicationImpl::windowsForCurrRepaint = &ApplicationImpl::windowsForRepaint[0];
std::vector<Window *> *ApplicationImpl::windowsForPostRepaint = &ApplicationImpl::windowsForRepaint[1];

std::unordered_set<Object *> ApplicationImpl::objectToDelete;
}  // namespace Luma::Core

namespace Luma::Core
{

Application::Application ()
{
  glfwInit ();
}

Application::~Application ()
{
  glfwTerminate ();
}

int Application::exec ()
{
  if (ApplicationImpl::isExecuted)
    return -1;

  ApplicationImpl::exit = false;
  ApplicationImpl::isExecuted = true;

  while (!ApplicationImpl::exit && !ApplicationImpl::allWindows.empty ())
    {
      std::swap (ApplicationImpl::windowsForCurrRepaint, ApplicationImpl::windowsForPostRepaint);

      for (Window *window : *ApplicationImpl::windowsForCurrRepaint)
        {
          if (!window->isVisible () || window->isIconified ())
            continue;

          window->mUpdateNeeded = false;
          window->repaint (window->mUpdateRect);
        }

      ApplicationImpl::windowsForCurrRepaint->clear ();

      glfwPollEvents ();
    }

  ApplicationImpl::isExecuted = false;
  return ApplicationImpl::returnCode;
}

bool Application::isExecuted ()
{
  return ApplicationImpl::isExecuted;
}

void Application::exit (int returnCode)
{
  ApplicationImpl::exit = true;
  ApplicationImpl::returnCode = returnCode;
}

void Application::quit ()
{
  exit (0);
}

Window *Application::focusWindow ()
{
  return ApplicationImpl::focusWindow;
}

Monitor Application::primaryMonitor ()
{
  return Monitor (glfwGetPrimaryMonitor ());
}
}
