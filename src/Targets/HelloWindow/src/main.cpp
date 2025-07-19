#include "GL/Application.h"
#include "GL/Window.h"

int main ()
{
  GL::Application app;
  GL::Window window ({800, 600}, "Hello Window");
  //GL::Window window ("Hello Window");
  app.runEventLoop ();
  return 0;
}
