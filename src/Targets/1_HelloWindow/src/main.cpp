#include "GL/KeyEvent.h"
#include "GL/Widget.h"
#include "GL/Widgets/FrameRateWidget.h"
#include "GL/Window.h"
#include <GL/glew.h>

class MainWidget : public GL::Widget
{
public:
  MainWidget () noexcept
  {
    addWidget (std::make_unique<GL::FrameRateWidget> ());
  }
  ~MainWidget () noexcept override = default;

private:
  void renderEvent () override
  {
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
  }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      GL::Window::getInstance ().close ();
  }
};

int main ()
{
  GL::Window::getInstance ().create ({800, 600} /* size */, "Hello Window!")
      .setResizable (true)
      .setDecorated (true)
      .setVisible (true)
      .setMaximized (true)
      .setFocused (true)
      .setAutoIconify (true)
      .setVersionMajor (3)
      .setVersionMinor (3)
      .setOpenGLProfile (GL::WindowCreateConfig::Profile::Core);

  if (glewInit () != GLEW_OK)
    throw std::runtime_error ("Can't init glew");


  GL::Window::getInstance ().open (std::make_unique<MainWidget> ());
}
