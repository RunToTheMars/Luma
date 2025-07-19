#include "Luma/Core/Application.h"
#include "Luma/Core/Window.h"

int main ()
{
  Luma::Core::Application app;
  Luma::Core::Window window ({800, 600}, "Hello Window");
  // GL::Window window ("Hello Window");
  app.exec ();
  return 0;
}
