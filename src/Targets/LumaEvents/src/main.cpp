#include "Luma/Core/Application.h"
#include "Luma/Core/KeyEvent.h"
#include "Luma/Core/Monitor.h"
#include "Luma/Core/Window.h"
#include "Luma/GL/Buffer.h"
#include "Shader/ASCII/Debug/Common.h"
#include "Shader/ASCII/Debug/v150/TextLineShader.h"
#include <GL/glew.h>
#include <chrono>
#include <memory>

Luma::Core::WindowHints createHints (bool isModal)
{
  Luma::Core::WindowHints hints;
  hints.profile = Luma::Core::Profile::Compat;
  hints.contextVersionMajor = 3;
  hints.contextVersionMinor = 3;
  hints.floating = isModal;
  return hints;
}


class CustomWindow: public Luma::Core::Window
{
public:
  CustomWindow (bool isModal, Luma::Core::Window *parent): Luma::Core::Window ({800, 600}, isModal ? "Hello Window Events (Modal)" : "Hello Window Events", createHints (isModal), parent)
  {
    if (isModal)
      setModality (Modality::ApplicationModal);
  }

  ~CustomWindow ()
  {
    makeCurrent ();
  }

private:
  void keyEvent (const Luma::Core::KeyEvent &event) override
  {
    if (event.key () == Luma::Core::Key::Key_Escape && event.action () == Luma::Core::KeyEvent::Action::Press)
      return destroy ();

    if (event.key () == Luma::Core::Key::Key_F10 && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        if (isWindowed ())
          {
            if (!isMaximized ())
              maximize ();
            else
              restore ();
          }
      }

    if (event.key () == Luma::Core::Key::Key_F11 && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        if (isWindowed ())
          setMonitor (Luma::Core::Application::primaryMonitor ());
        else
          setWindowed (Luma::Core::Vec2I {0, 0}, Luma::Core::Vec2I {800, 600});
      }


    if (event.key () == Luma::Core::Key::Key_F5 && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        update ();
      }

    if (event.key () == Luma::Core::Key::Key_I && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        if (!isIconified ())
          {
            iconify ();
          }
        else
          {
            restore ();
          }
      }

    if (event.key () == Luma::Core::Key::Key_M && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        if (!isMaximized ())
          {
            maximize ();
          }
        else
          {
            restore ();
          }
      }


    if (event.key () == Luma::Core::Key::Key_A && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        requestAttention ();
      }

    if (event.key () == Luma::Core::Key::Key_W && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        if (mSubWindow)
          {
            delete mSubWindow;
            mSubWindow = nullptr;
          }
        else
          mSubWindow = new CustomWindow (false, this);
      }

    if (event.key () == Luma::Core::Key::Key_G && event.action () == Luma::Core::KeyEvent::Action::Press)
      {
        if (mSubWindow)
          {
            delete mSubWindow;
            mSubWindow = nullptr;
          }
        else
          mSubWindow = new CustomWindow (true, this);
      }
  }

  void renderEvent (const Luma::Core::RenderEvent &/*event*/) override
  {
    if (!mIsInited)
      {
        glewInit ();

        mIsInited = true;
        mTextLineShader.create ();

        mTextLineShader.bind ();
        mTextLineShader.setPosition (0.f, 0.f, 0.f);
        mTextLineShader.setColor (0.f, 1.f, 0.f, 1.f);
        mTextLineShader.setBackgroundColor (0.f, 0.f, 0.f, 0.f);
        mTextLineShader.setSize (Shader::ASCII::Debug::glyphTextureWidth (),
                                 Shader::ASCII::Debug::glyphTextureHeight ());

        mTextLineShader.unbind ();

        mTextVBO.create ();
        mTextVBO.bind ();
        mTextVBO.allocate (9);
        mTextVBO.unbind ();

        mStartTime = std::chrono::steady_clock::now ();
        mFrames = 0;
      }

    Luma::Core::Vec2I frameBufferSize = Luma::Core::Window::frameBufferSize ();
    float modelViewProjection[16] = { 2.f / frameBufferSize[0], 0.f                     , 0.f, 0.f,
                                      0.f                     , 2.f / frameBufferSize[1], 0.f, 0.f,
                                      0.f                     , 0.f                     , 1.f, 0.f,
                                     -1.f                     , -1.f                    , 0.f, 1.f};

    Luma::Core::Vec4F color = {Luma::Core::Window::pos ()[0] / 1000.f, 0.f, 0.f, 1.f};

    update ();

    glClearColor (color[0], color[1], color[2], color[3]);
    glClear (GL_COLOR_BUFFER_BIT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport (0, 0, frameBufferSize[0], frameBufferSize[1]);

    if (Luma::Core::Window::isHovered ())
      {
        Luma::Core::Vec2D cursorPos = Luma::Core::Window::cursorPos ();

        glMatrixMode (GL_MODELVIEW);
        glLoadMatrixf (modelViewProjection);

        glPointSize (8.0f);
        glBegin (GL_POINTS);
        glColor3f (0.0f, 0.0f, 1.0f);
        glVertex2f (cursorPos[0], frameBufferSize[1] - cursorPos[1]);
        glEnd ();
      }

    std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now ();
    const std::chrono::duration<double> elapsedSeconds{curTime - mStartTime};
    mFrames++;
    double dur_secs = elapsedSeconds.count ();
    if (dur_secs > 1.)
      {
        double fps = static_cast<double> (mFrames) / dur_secs;
        mStartTime = curTime;
        char text[10];
        sprintf (text, "FPS:%4d", static_cast<int> (fps));

        mTextVBO.bind ();
        mTextVBO.write (0, text, 9);
        mTextVBO.unbind ();
        mFrames = 0;
      }


    mTextVBO.bind ();
    glVertexAttribIPointer (mTextLineShader.textAttributeLocation (), 1, GL_UNSIGNED_BYTE, 0, (void *) nullptr);  /// Use current binded GL_ARRAY_BUFFER

    mTextLineShader.bind ();
    mTextLineShader.setModelViewProjectionMatrix (modelViewProjection);

    glEnableVertexAttribArray (mTextLineShader.textAttributeLocation ());
    glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, 9 /* count */);
    glDisableVertexAttribArray (mTextLineShader.textAttributeLocation ());
    mTextLineShader.unbind ();

    mTextVBO.unbind ();
  }

  void hoverEvent (const Luma::Core::HoverEvent &/*event*/) override
  {
    update ();
  }

  void leaveEvent () override
  {
    update ();
  }

private:
  CustomWindow *mSubWindow = nullptr;

  bool mIsInited = false;

  Shader::ASCII::Debug::v150::TextLineShader mTextLineShader;
  Luma::GL::Buffer mTextVBO;

  std::chrono::steady_clock::time_point mStartTime;
  int mFrames = 0;
};

int main ()
{
  Luma::Core::Application app;

  CustomWindow window (false, nullptr);

  app.exec ();
  return 0;
}
