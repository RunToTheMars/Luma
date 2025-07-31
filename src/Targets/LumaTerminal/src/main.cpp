#include "Luma/Core/Application.h"
#include "Terminal.h"

int main ()
{
  Luma::Core::Application app;

  Terminal terminal;

  app.exec ();
  return 0;
}
