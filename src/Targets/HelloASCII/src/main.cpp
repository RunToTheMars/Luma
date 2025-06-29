#include "GL/ASCII/v150/DebugShader.h"
#include "GL/KeyEvent.h"
#include "GL/ResizeEvent.h"
#include "GL/Window.h"
#include "Geometry/Size.h"
#include <GL/glew.h>
#include <chrono>
#include <stdexcept>

#define ALLOC
#define PATTERN GL_DYNAMIC_DRAW

class MyWindow : public GL::Window
{
  std::chrono::steady_clock::time_point m_start;
  int m_frames = 0;
  std::string m_curFPS;

  GL::ASCII::v150::DebugShader mTextRenderer;
  unsigned int mTextVBO;

public:
  MyWindow () noexcept = default;
  ~MyWindow () noexcept override
  {
    glDeleteBuffers (1, &mTextVBO);
  }

  void init () override
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    /// compile & link
    mTextRenderer.init ();

    /// for blending
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /// create buffer
    glGenBuffers (1, &mTextVBO);

    #ifdef ALLOC
    /// allocate
    glBindBuffer (GL_ARRAY_BUFFER, mTextVBO);
    glBufferData (GL_ARRAY_BUFFER, 100, nullptr, PATTERN);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    #endif

    mTextRenderer.bind ();
    mTextRenderer.setPosition (0.f, 0.f, 0.f);
    mTextRenderer.setColor (1.f, 1.f, 0.f, 1.f);
    mTextRenderer.setBackgroundColor (1.f, 1.f, 1.f, 0.f);
    mTextRenderer.unbind ();

    m_start = std::chrono::steady_clock::now ();
    m_frames = 0;
  }

  void renderEvent () override
  {
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);

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
        mTextRenderer.bind ();
        glBindBuffer (GL_ARRAY_BUFFER, mTextVBO);

        #ifdef ALLOC
        glBufferSubData (GL_ARRAY_BUFFER, 0, m_curFPS.size (), m_curFPS.data ());
        #else
        glBufferData (GL_ARRAY_BUFFER, m_curFPS.size (), m_curFPS.data (), PATTERN);
        #endif

        constexpr double scale = 1.;
        mTextRenderer.setSize (scale * 2.f * GL::ASCII::v150::DebugShader::glyphTextureWidth () / size ().width (),
                               scale * 2.f * GL::ASCII::v150::DebugShader::glyphTextureHeight () / size ().height ());

        glVertexAttribIPointer (mTextRenderer.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, (void *) nullptr);
        glEnableVertexAttribArray (mTextRenderer.attributeCodeLocation ());
        glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, m_curFPS.size () /* count */);
        glDisableVertexAttribArray (mTextRenderer.attributeCodeLocation ());
        glBindBuffer (GL_ARRAY_BUFFER, 0);
        mTextRenderer.unbind ();
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
