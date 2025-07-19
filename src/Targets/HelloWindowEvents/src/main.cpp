#include "GL/Application.h"
#include "GL/KeyEvent.h"
#include "GL/Monitor.h"
#include "GL/MoveEvent.h"
#include "GL/Window.h"
#include <GL/glew.h>
#include <memory>

GL::WindowHints createHints (bool isModal)
{
  GL::WindowHints hints;
  hints.profile = GL::Profile::Compat;
  hints.contextVersionMajor = 3;
  hints.contextVersionMinor = 3;
  hints.floating = isModal;
  return hints;
}


class CustomWindow: public GL::Window
{
public:
  CustomWindow (bool isModal, GL::Window *parent):
  GL::Window ({800, 600}, isModal ? "Hello Window Events (Modal)" : "Hello Window Events", createHints (isModal), parent)
  {
    if (isModal)
      setModality (GL::Window::Modality::ApplicationModal);
  }

private:
  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      return destroy ();

    if (event.key () == GL::Key::Key_F10 && event.action () == GL::KeyAction::Press)
      {
        if (isWindowed ())
          {
            if (!isMaximized ())
              maximize ();
            else
              restore ();
          }
      }

    if (event.key () == GL::Key::Key_F11 && event.action () == GL::KeyAction::Press)
      {
        if (isWindowed ())
          setMonitor (GL::Application::primaryMonitor ());
        else
          setWindowed ({0, 0}, {800, 600});
      }


    if (event.key () == GL::Key::Key_F5 && event.action () == GL::KeyAction::Press)
      {
        update ();
      }

    if (event.key () == GL::Key::Key_I && event.action () == GL::KeyAction::Press)
      {
        if (!isIconified ())
          iconify ();
        else
          restore ();
      }

    if (event.key () == GL::Key::Key_M && event.action () == GL::KeyAction::Press)
      {
        if (!isMaximized ())
          maximize ();
        else
          restore ();
      }


    if (event.key () == GL::Key::Key_A && event.action () == GL::KeyAction::Press)
      {
        requestAttention ();
      }

    if (event.key () == GL::Key::Key_W && event.action () == GL::KeyAction::Press)
      {
        if (mSubWindow)
          mSubWindow.reset ();
        else
          mSubWindow = std::make_unique<CustomWindow> (false, this);
      }

    if (event.key () == GL::Key::Key_G && event.action () == GL::KeyAction::Press)
      {
        if (mSubWindow)
          mSubWindow.reset ();
        else
          mSubWindow = std::make_unique<CustomWindow> (true, this);
      }
  }

  void renderEvent (const GL::RenderEvent &/*event*/) override
  {
    Geom::Vec2I frameBufferSize = GL::Window::frameBufferSize ();
    Geom::Vec4F color = {GL::Window::pos ()[0] / 1000.f, 1.f, 1.f, 1.f};

    glClearColor (color[0], color[1], color[2], color[3]);
    glClear (GL_COLOR_BUFFER_BIT);
    glViewport (0, 0, frameBufferSize[0], frameBufferSize[1]);

    if (GL::Window::isHovered ())
      {
        Geom::Vec2F glPos;
        Geom::Vec2D cursorPos = GL::Window::cursorPos ();
        glPos[0] = 2. * cursorPos[0] / frameBufferSize[0] - 1.f;
        glPos[1] = 1.f - 2. * cursorPos[1] / frameBufferSize[1];

        glPointSize(8.0f);
        glBegin(GL_POINTS);
          glColor3f(0.0f, 0.0f, 1.0f);
          glVertex2f(glPos[0], glPos[1]);
        glEnd();
      }
  }

  void hoverEvent (const GL::HoverEvent &/*event*/) override
  {
    update ();
  }

  void leaveEvent () override
  {
    update ();
  }

private:
  std::unique_ptr<CustomWindow> mSubWindow;
};

int main ()
{
  GL::Application app;
  CustomWindow window (false, nullptr);

  app.runEventLoop ();
  return 0;
}
