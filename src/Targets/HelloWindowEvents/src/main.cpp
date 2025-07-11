#include "Common/Signal.h"
#include "GL/Application.h"
#include "GL/Monitor.h"
#include "GL/Window.h"
#include <GL/glew.h>
#include <memory>

GL::WindowHints createHints (bool isModal)
{
  GL::WindowHints hints;
  hints.profile = GL::WindowHints::Profile::Compat;
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
              {
                mPosBeforeFullScreen = mWindow.pos ();
                mSizeBeforeFullScreen = mWindow.size ();
                mWindow.setDecorated (false);
                mWindow.maximize ();
                // window.setMonitor (GL::Application::primaryMonitor ());
              }
            else
              {
                mWindow.setDecorated (true);
                mWindow.setPos (mPosBeforeFullScreen);
                mWindow.setSize (mSizeBeforeFullScreen);
              }
          }
      }

    if (event.key () == GL::Key::Key_F11 && event.action () == GL::KeyAction::Press)
      {
        if (mWindow.isWindowed ())
          {
            mPosBeforeFullScreen = mWindow.pos ();
            mSizeBeforeFullScreen = mWindow.size ();
            mWindow.setMonitor (GL::Application::primaryMonitor ());
          }
        else
          mWindow.setWindowed (mPosBeforeFullScreen, mSizeBeforeFullScreen);
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

    glClearColor (mColor[0], mColor[1], mColor[2], mColor[3]);
    glClear (GL_COLOR_BUFFER_BIT);
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
        glFlush();
      }
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

  Geom::Vec2I mPosBeforeFullScreen;
  Geom::Vec2I mSizeBeforeFullScreen = {800, 600};

  Geom::Vec4F mColor = {0.5f, 0.5f, 0.0f, 1.0f};

  std::unique_ptr<CustomWindow> mSubWindow;
  std::optional<Geom::Vec2D> mHoverPos;
};

int main ()
{
  GL::Application app;
  CustomWindow window (false, nullptr);

  app.runEventLoop ();
  return 0;
}
