#include "GL/ASCII/v150/Debug/TextLineShader.h"
#include "GL/KeyEvent.h"
#include "GL/ResizeEvent.h"
#include "GL/Window.h"
#include "GL/Buffer.h"
#ifdef COREPROFILE
#include "GL/VertexArray.h"
#endif
#include "Geometry/Size.h"
#include <GL/glew.h>
#include <chrono>
#include <stdexcept>

//#define USE_INDICES

#ifdef USE_INDICES
char INDICES[13]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
#endif

class MyWindow : public GL::Window
{
  std::chrono::steady_clock::time_point m_start;
  int m_frames = 0;
  std::string m_curFPS;

  GL::ASCII::v150::Debug::TextLineShader mTextLineShader;
  GL::Buffer mTextVBO;

#ifdef COREPROFILE
  GL::VertexArray mTextVAO;
#endif

#ifdef USE_INDICES
  GL::Buffer mTextEBO = GL::Buffer (GL::Buffer::Type::Index);
#endif

public:
  MyWindow () noexcept = default;
  ~MyWindow () noexcept override
  {
  }

  void init () override
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    /// compile & link
    mTextLineShader.init ();

    /// for blending
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef COREPROFILE
    mTextVAO.create ();
    mTextVAO.bind ();

    mTextVBO.create ();
    mTextVBO.bind ();
    mTextVBO.allocate (20, GL::Buffer::UsagePattern::StreamDraw);

#ifdef USE_INDICES
    mTextEBO.create ();
    mTextEBO.bind ();
    mTextEBO.allocate (INDICES, sizeof (INDICES), GL::Buffer::UsagePattern::StaticDraw);
#endif

    glVertexAttribIPointer (mTextLineShader.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, (void *) nullptr); /// Use current binded GL_ARRAY_BUFFER
    glEnableVertexAttribArray (mTextLineShader.attributeCodeLocation ());

    mTextVAO.unbind ();
#else

#ifdef USE_INDICES
    mTextEBO.create ();
    mTextEBO.bind ();
    mTextEBO.allocate (INDICES, sizeof (INDICES), GL::Buffer::UsagePattern::StaticDraw);
    mTextEBO.unbind ();
#endif // USE_INDICES

    mTextVBO.create ();
    mTextVBO.bind ();
    mTextVBO.allocate (20, GL::Buffer::UsagePattern::DynamicDraw);
    mTextVBO.unbind ();
#endif // COREPROFILE

    mTextLineShader.bind ();
    mTextLineShader.setPosition (-1.f, -1.f, 0.f);
    mTextLineShader.setColor (1.f, 1.f, 0.f, 1.f);
    mTextLineShader.setBackgroundColor (1.f, 1.f, 1.f, 0.f);
    mTextLineShader.unbind ();

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
#ifdef COREPROFILE
          mTextLineShader.bind ();
          constexpr double scale = 1.;
          mTextLineShader.setSize (scale * 2.f * GL::ASCII::v150::Debug::glyphTextureWidth () / size ().width (),
                                   scale * 2.f * GL::ASCII::v150::Debug::glyphTextureHeight () / size ().height ());
          mTextVAO.bind ();
          mTextVBO.write (0, m_curFPS.data (), m_curFPS.size ());
#ifdef USE_INDICES
          glDrawElements (GL_POINTS /*mode*/, sizeof (INDICES) /* count */, GL_UNSIGNED_BYTE /* type */, nullptr /*indices*/);

#else
          glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, m_curFPS.size () /* count */);
#endif
          mTextVAO.unbind ();
          mTextLineShader.unbind ();
          #else
          mTextVBO.bind ();
          mTextVBO.write (0, m_curFPS.data (), m_curFPS.size ());

          glVertexAttribIPointer (mTextLineShader.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, (void *) nullptr); /// Use current binded GL_ARRAY_BUFFER
          glEnableVertexAttribArray (mTextLineShader.attributeCodeLocation ());

          mTextLineShader.bind ();

          constexpr double scale = 1.;
          mTextLineShader.setSize (scale * 2.f * GL::ASCII::v150::Debug::glyphTextureWidth () / size ().width (),
                                   scale * 2.f * GL::ASCII::v150::Debug::glyphTextureHeight () / size ().height ());
#ifdef USE_INDICES
          mTextEBO.bind ();
          glDrawElements (GL_POINTS /*mode*/, sizeof (INDICES) /* count */, GL_UNSIGNED_BYTE /* type */, nullptr /*indices*/);
          mTextEBO.unbind ();
#else
          glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, m_curFPS.size () /* count */);
#endif
          glDisableVertexAttribArray (mTextLineShader.attributeCodeLocation ());
          glBindBuffer (GL_ARRAY_BUFFER, 0);
          mTextLineShader.unbind ();

          mTextVBO.unbind ();
#endif
      }
  }

  void resizeEvent (const GL::ResizeEvent &event) override
  {
    glViewport (0, 0, event.size ().width (), event.size ().height ());
    GL::Widget::resizeEvent (event);
  }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      close ();
  }
};

int main ()
{
  MyWindow window;
  #ifdef COREPROFILE
  window.create ({800, 600} /* size */, "Hello FPS! (Core)")
  #else
  window.create ({800, 600} /* size */, "Hello FPS! (Compatibility)")
  #endif
      .setResizable (true)
      .setDecorated (true)
      .setVisible (true)
      .setMaximized (true)
      .setFocused (true)
      .setAutoIconify (true)
      .setVersionMajor (3)
      .setVersionMinor (3)
#ifdef COREPROFILE
      .setOpenGLProfile (GL::WindowCreateConfig::Profile::CORE);
#else
      .setOpenGLProfile (GL::WindowCreateConfig::Profile::COMPAT);
#endif
  window.exec ();
}
