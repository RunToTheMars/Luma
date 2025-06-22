#include "GL/Window.h"
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

  void resizeEvent (const GL::ResizeEvent &event) override { glViewport (0, 0, event.width (), event.height ()); }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_ESCAPE && event.action () == GL::KeyAction::Press)
      close ();
  }
};


int main ()
{
  MyWindow window;
  window.create (800 /*width*/, 600 /*height*/, "Hello World!")
      .setResizable (true)
      .setDecorated (true)
      .setVisible (true)
      .setMaximized (false)
      .setFocused (true)
      .setAutoIconify (true)
      .setVersionMajor (3)
      .setVersionMinor (3)
      .setOpenGLProfile (GL::WindowCreateConfig::Profile::COMPAT);
  window.exec ();
}
