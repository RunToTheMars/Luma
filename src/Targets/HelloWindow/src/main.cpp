#include "GL/Application.h"
#include "GL/Monitor.h"
#include "GL/Window.h"

int main ()
{
  GL::Application app;
  GL::Window window ({800, 600}, "Hello Window");
  // GL::Window window ("Hello Window", GL::Application::primaryMonitor ());
  app.runEventLoop ();
  return 0;
}
