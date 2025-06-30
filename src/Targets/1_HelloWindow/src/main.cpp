#include "GL/KeyEvent.h"
#include "GL/ResizeEvent.h"
#include "GL/Window.h"
#include "Geometry/Size.h"
#include <GL/glew.h>

class MyWindow : public GL::Window
{
public:
  MyWindow () noexcept = default;
  ~MyWindow () noexcept override = default;

  void init () override {}

  void renderEvent () override
  {
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
  }

  void resizeEvent (const GL::ResizeEvent &event) override { glViewport (0, 0, event.size ().width (), event.size ().height ()); }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      close ();
  }
};

int main ()
{
  MyWindow window;
  window.create ({800, 600} /* size */, "Hello Window!")
      .setResizable (true)
      .setDecorated (true)
      .setVisible (true)
      .setMaximized (false)
      .setFocused (true)
      .setAutoIconify (true)
      .setVersionMajor (3)
      .setVersionMinor (3)
      .setOpenGLProfile (GL::WindowCreateConfig::Profile::CORE);
  window.exec ();
}
