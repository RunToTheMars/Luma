#include "GL/ASCII/v150/Debug/TextLineShader.h"
#include "GL/Application.h"
#include "GL/Buffer.h"
#include "GL/KeyEvent.h"
#include "GL/Monitor.h"
#include "GL/Window.h"
#include <GL/glew.h>
#include <chrono>
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
  CustomWindow (bool isModal, GL::Window *parent): GL::Window ({800, 600}, isModal ? "Hello Window Events (Modal)" : "Hello Window Events", createHints (isModal), parent)
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
          setWindowed (Geom::Vec2I {0, 0}, Geom::Vec2I {800, 600});
      }


    if (event.key () == GL::Key::Key_F5 && event.action () == GL::KeyAction::Press)
      {
        update ();
      }

    if (event.key () == GL::Key::Key_I && event.action () == GL::KeyAction::Press)
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

    if (event.key () == GL::Key::Key_M && event.action () == GL::KeyAction::Press)
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
    Geom::Vec4F color = {GL::Window::pos ()[0] / 1000.f, 0.f, 0.f, 1.f};

    update ();

    glClearColor (color[0], color[1], color[2], color[3]);
    glClear (GL_COLOR_BUFFER_BIT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport (0, 0, frameBufferSize[0], frameBufferSize[1]);

    if (GL::Window::isHovered ())
      {
        Geom::Vec2F glPos;
        Geom::Vec2D cursorPos = GL::Window::cursorPos ();
        glPos[0] = 2. * cursorPos[0] / frameBufferSize[0] - 1.f;
        glPos[1] = 1.f - 2. * cursorPos[1] / frameBufferSize[1];

        glPointSize (8.0f);
        glBegin (GL_POINTS);
        glColor3f (0.0f, 0.0f, 1.0f);
        glVertex2f (glPos[0], glPos[1]);
        glEnd ();
      }

    if (!mIsInited)
      {
        glewInit ();
        mIsInited = true;
        mTextLineShader.init ();

        mTextLineShader.bind ();
        mTextLineShader.setPosition (-1.f, -1.f, 0.f);
        mTextLineShader.setColor (0.f, 1.f, 0.f, 1.f);
        mTextLineShader.setBackgroundColor (0.f, 0.f, 0.f, 0.f);
        mTextLineShader.unbind ();

        mTextVBO.create ();
        mTextVBO.bind ();
        mTextVBO.allocate (9);
        mTextVBO.unbind ();

        mStartTime = std::chrono::steady_clock::now ();
        mFrames = 0;
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
    glVertexAttribIPointer (mTextLineShader.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, (void *) nullptr);  /// Use current binded GL_ARRAY_BUFFER

    mTextLineShader.bind ();

    mTextLineShader.setSize (
        2.f * GL::ASCII::v150::Debug::glyphTextureWidth () / frameBufferSize[0],
        2.f * GL::ASCII::v150::Debug::glyphTextureHeight () / frameBufferSize[1]);

    glEnableVertexAttribArray (mTextLineShader.attributeCodeLocation ());
    glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, 9 /* count */);
    glDisableVertexAttribArray (mTextLineShader.attributeCodeLocation ());
    mTextLineShader.unbind ();

    mTextVBO.unbind ();
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

  bool mIsInited = false;

  GL::ASCII::v150::Debug::TextLineShader mTextLineShader;
  GL::Buffer mTextVBO;

  std::chrono::steady_clock::time_point mStartTime;
  int mFrames = 0;
};

int main ()
{
  GL::Application app;
  CustomWindow window (false, nullptr);

  app.runEventLoop ();
  return 0;
}
