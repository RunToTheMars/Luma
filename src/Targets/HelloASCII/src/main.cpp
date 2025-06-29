#include "GL/ASCII/v130/Word.h"
#include "GL/ASCII/v150/Word.h"
#include "GL/KeyEvent.h"
#include "GL/ResizeEvent.h"
#include "GL/Window.h"
#include "Geometry/Size.h"
#include <GL/glew.h>
#include <chrono>
#include <stdexcept>

namespace GL_ASCII = GL::ASCII::v130;

class MyWindow : public GL::Window
{
  std::chrono::steady_clock::time_point m_start;
  int m_frames = 0;
  std::string m_curFPS;

  GL_ASCII::Renderer mTextRenderer;

public:
  MyWindow () noexcept = default;
  ~MyWindow () noexcept override = default;

  void init () override
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    mTextRenderer.initialize ();
    mTextRenderer.bind (GL::ShaderProgramBinderRestore ())
        .setPosition (0.f, 0.f, 0.f)
        .setColor (1.f, 1.f, 0.f, 1.f)
        .setBackgroundColor (1.f, 1.f, 1.f, 0.f);

    m_start = std::chrono::steady_clock::now ();
    m_frames = 0;
  }

  void renderEvent () override
  {
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::chrono::steady_clock::time_point cur_time = std::chrono::steady_clock::now ();
    const std::chrono::duration<double> elapsed_seconds{cur_time - m_start};
    m_frames++;
    double dur_secs = elapsed_seconds.count ();
    if (dur_secs > 1.)
      {
        double fps = static_cast<double> (m_frames) / dur_secs;
        m_start = cur_time;
        m_curFPS = "FPS: " + std::to_string (fps);
        m_frames = 0;
      }

    if (!m_curFPS.empty ())
      {
        constexpr double scale = 1.;

        GL::ShaderProgramBinderRestore b;
        auto binder = mTextRenderer.bind (b);

        binder.setSize (scale * 2.f * GL_ASCII::Renderer::glyphTextureWidth () / size ().width (),
                        scale * 2.f * GL_ASCII::Renderer::glyphTextureHeight () / size ().height ());
        GL_ASCII::Renderer::Data data (m_curFPS.c_str (), m_curFPS.size ());
        for (int i = 0; i < 1; i++)
          binder.draw (data);
      }
  }

  void resizeEvent (const GL::ResizeEvent &event) override
  {
    glViewport (0, 0, event.size ().width (), event.size ().height ());
    GL::Widget::resizeEvent (event);
  }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_ESCAPE && event.action () == GL::KeyAction::Press)
      close ();
  }
};

int main ()
{
  MyWindow window;
  window.create ({800, 600} /* size */, "Hello ASCII!")
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
