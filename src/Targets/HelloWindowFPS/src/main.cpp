#include "Common/Signal.h"
#include "GL/ASCII/v150/Debug/TextLineShader.h"
#include "GL/Application.h"
#include "GL/Buffer.h"
#include "GL/Monitor.h"
#include "GL/Window.h"
#include <GL/glew.h>
#include <memory>
#include <chrono>

GL::WindowHints createHints (bool isModal)
{
  GL::WindowHints hints;
  hints.profile = GL::Profile::Compat;
  hints.contextVersionMajor = 3;
  hints.contextVersionMinor = 3;
  hints.floating = isModal;
  return hints;
}


class CustomWindow
{
public:
  CustomWindow (bool isModal, GL::Window *parent):
  mWindow ({800, 600}, isModal ? "Hello Window Events (Modal)" : "Hello Window Events", createHints (isModal), parent)
  {
    #define BIND_THIS(method) [this] (auto &&...args) { this->method (std::forward<decltype (args)> (args)...); }

    mSlots.connect (mWindow.moveEvent  , BIND_THIS (moveEventHandle));
    mSlots.connect (mWindow.keyEvent   , BIND_THIS (keyEventHandle));
    mSlots.connect (mWindow.renderEvent, BIND_THIS (renderEventHandle));
    mSlots.connect (mWindow.hoverEvent , BIND_THIS (hoverEventHandle));
    mSlots.connect (mWindow.leaveEvent , BIND_THIS (leaveEventHandle));
    mSlots.connect (mWindow.closeEvent , BIND_THIS (closeEventHandle));

    if (isModal)
      mWindow.setModality (GL::Window::Modality::ApplicationModal);

    #undef BIND_THIS
  }

private:
  void moveEventHandle (const Geom::Vec2I &pos)
  {
    mColor[0] = pos[0] / 3000.f;
    mColor[1] = pos[1] / 2000.f;
  }

  void keyEventHandle (const GL::KeyEvent &event)
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      return mWindow.destroy ();

    if (event.key () == GL::Key::Key_F10 && event.action () == GL::KeyAction::Press)
      {
        if (mWindow.isWindowed ())
          {
            if (!mWindow.isMaximized ())
              mWindow.maximize ();
            else
              mWindow.restore ();
          }
      }

    if (event.key () == GL::Key::Key_F11 && event.action () == GL::KeyAction::Press)
      {
        if (mWindow.isWindowed ())
          mWindow.setMonitor (GL::Application::primaryMonitor ());
        else
          mWindow.setWindowed (Geom::Vec2I {0, 0}, Geom::Vec2I {800, 600});
      }


    if (event.key () == GL::Key::Key_F5 && event.action () == GL::KeyAction::Press)
      {
        mWindow.update ();
      }

    if (event.key () == GL::Key::Key_I && event.action () == GL::KeyAction::Press)
      {
        if (!mWindow.isIconified ())
          {
            mWindow.iconify ();
          }
        else
          {
            mWindow.restore ();
          }
      }

    if (event.key () == GL::Key::Key_M && event.action () == GL::KeyAction::Press)
      {
        if (!mWindow.isMaximized ())
          {
            mWindow.maximize ();
          }
        else
          {
            mWindow.restore ();
          }
      }


    if (event.key () == GL::Key::Key_A && event.action () == GL::KeyAction::Press)
      {
        mWindow.requestAttention ();
      }

    if (event.key () == GL::Key::Key_W && event.action () == GL::KeyAction::Press)
      {
        if (mSubWindow)
          mSubWindow.reset ();
        else
          mSubWindow = std::make_unique<CustomWindow> (false, &mWindow);
      }

    if (event.key () == GL::Key::Key_G && event.action () == GL::KeyAction::Press)
      {
        if (mSubWindow)
          mSubWindow.reset ();
        else
          mSubWindow = std::make_unique<CustomWindow> (true, &mWindow);
      }
  }

  void renderEventHandle ()
  {
    Geom::Vec2I frameBufferSize = mWindow.frameBufferSize ();
    mWindow.update ();

    glClearColor (mColor[0], mColor[1], mColor[2], mColor[3]);
    glClear (GL_COLOR_BUFFER_BIT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport (0, 0, frameBufferSize[0], frameBufferSize[1]);

    if (mHoverPos.has_value ())
      {
        Geom::Vec2F glPos;
        glPos[0] = 2. * mHoverPos->at (0) / frameBufferSize[0] - 1.f;
        glPos[1] = 1.f - 2. * mHoverPos->at (1) / frameBufferSize[1];

        glPointSize(8.0f);
        glBegin(GL_POINTS);
          glColor3f(0.0f, 0.0f, 1.0f);
          glVertex2f(glPos[0], glPos[1]);
        glEnd();
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

  void hoverEventHandle (const Geom::Vec2D &pos)
  {
    if (mHoverPos == pos)
      return;

    mHoverPos = pos;
    mWindow.update ();
  }

  void leaveEventHandle ()
  {
    if (mHoverPos)
      {
        mHoverPos = std::nullopt;
        mWindow.update ();
      }
  }

  void closeEventHandle ()
  {
    mWindow.destroy ();
  }

private:
  GL::Window mWindow;
  Common::Slots mSlots;

  Geom::Vec4F mColor = {0.5f, 0.5f, 0.0f, 1.0f};

  std::unique_ptr<CustomWindow> mSubWindow;
  std::optional<Geom::Vec2D> mHoverPos;

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
