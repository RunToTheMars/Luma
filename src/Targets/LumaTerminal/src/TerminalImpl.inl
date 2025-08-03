#include "GL/glew.h"
#include "Luma/Core/Application.h"
#include "Luma/Core/CharEvent.h"
#include "Luma/Core/KeyEvent.h"
#include "Luma/Core/Monitor.h"
#include "Luma/Core/MouseEvent.h"
#include "Luma/Core/RenderEvent.h"
#include "Luma/Core/Window.h"
#include "Luma/GL/Buffer.h"
#include "Luma/GL/VertexArray.h"
#include "Luma/RaiiUtils/DoAfterScope.h"
#include "Shader/ASCII/Debug/Common.h"
#include "Shader/ASCII/Debug/v150/TextBoxShader.h"
#include "Shader/ASCII/Debug/v150/TextLineShader.h"
#include "Shader/Basic/v110/BasicShader.h"
#include <algorithm>
#include <math.h>
#include <vector>

class TerminalImpl
{
public:
  TerminalImpl (Luma::Core::Window &window);

  void renderEvent (const Luma::Core::RenderEvent &event);
  void keyEvent (const Luma::Core::KeyEvent &event);
  void charEvent (const Luma::Core::CharEvent &event);
  void mouseEvent (const Luma::Core::MouseEvent &event);
  void focusInEvent ();
  void focusOutEvent ();
  void scaleEvent (const Luma::Core::ScaleEvent &event);

private:
  void init ();

  void clearAndRenderBackground ();
  void renderInput (const Luma::Core::Vec2I &viewSize);
  void renderInputText (const Luma::Core::Vec2I &viewSize);
  void renderInputPos (const Luma::Core::Vec2I &viewSize);
  //void renderFrame (const Luma::Core::Vec2I &frameBufferSize);

  void moveInputPosHandle (int pos);
  bool moveInputPosHandleUnsafe (int pos);
  void insertHandle (char c);
  void eraseHandle (int count);
  void deleteHandle (int count);

  void pasteHandle ();

  int KeyEventRepeatForce () const;

  // void startSelectHandle ();
  // void finishSelectHandle ();

private:
  Luma::Core::Window &mWindow;

  bool mInited = false;

  Shader::ASCII::Debug::v150::TextBoxShader mTextBoxShader;
  Shader::ASCII::Debug::v150::TextLineShader mTextLineShader;
  Shader::Basic::v110::BasicShader mBasicShader;

  Luma::GL::VertexArray mTextBoxVAO;
  Luma::GL::Buffer mTextBoxVBO;

  Luma::GL::VertexArray mInputTextVAO;
  Luma::GL::Buffer mInputTextVBO;
  std::vector<char> mUserInputText;
  bool mInputTextIsDirty = true;

  Luma::GL::VertexArray mInputPosVAO;
  Luma::GL::Buffer mInputPosVBO;
  int mInputPos = 0;
  bool mInputPosIsDirty = true;

  int mKeyEventRepeatCount = 0;

  // struct
  // {
  //   bool inEvent = false;
  //   Luma::Geom::Vec2I start = {0, 0};
  // } mSelectEventData;

  // struct
  // {
  //   int start = 0;
  //   int size = 0;
  // } mSelectData;

  // Luma::GL::VertexArray mWindowFrameVAO;
  // Luma::GL::Buffer mWindowFrameVBO;

  static constexpr Luma::Geom::Vec4F gBackgroundColor = {0.1f, 0.1f, 0.1f, 1.0f};

  static constexpr Luma::Geom::Vec4F gTextBackgroundColor = gBackgroundColor;
  static constexpr std::string_view gPrompt = "> ";
  static constexpr Luma::Geom::Vec4F gPromptColor = {0.0f, 0.65f, 0.0f, 1.0f};
  static constexpr Luma::Geom::Vec4F gUserInputColor = {0.65f, 0.65f, 0.0f, 1.0f};

  // static constexpr Luma::Geom::Vec4F gFrameColor = {0.3f, 0.3f, 0.3f, 1.0f};
  // static constexpr float gFrameSize = 1.f;
  static constexpr int gFrameSize = 10;

  static constexpr float gTextScale = 1.f;
  static constexpr Luma::Geom::Vec2F gTextSize = gTextScale * Luma::Geom::Vec2F{Shader::ASCII::Debug::glyphTextureWidth (), Shader::ASCII::Debug::glyphTextureHeight ()};
};


TerminalImpl::TerminalImpl (Luma::Core::Window &window): mWindow (window)
{
}


void TerminalImpl::renderEvent (const Luma::Core::RenderEvent &event)
{
  if (!mInited)
    {
      init ();
      mInited = true;
    }

  Luma::Core::Vec2I frameBufferSize = mWindow.frameBufferSize ();
  Luma::Core::RectI drawRect = {{gFrameSize, gFrameSize}, {frameBufferSize[0] - 2 * gFrameSize, frameBufferSize[1] - 2 * gFrameSize}};

  {

    float modelViewProjection[16] = { 2.f / drawRect.size[0], 0.f                   , 0.f, 0.f,
                                      0.f                   , 2.f / drawRect.size[1], 0.f, 0.f,
                                      0.f                   , 0.f                   , 1.f, 0.f,
                                     -1.f                   , -1.f                  , 0.f, 1.f};

    mBasicShader.bind ();
    mBasicShader.setModelViewProjectionMatrix (modelViewProjection);

    mTextBoxShader.bind ();
    mTextBoxShader.setModelViewProjectionMatrix (modelViewProjection);

    DO_AFTER_SCOPE (mTextBoxShader.unbind ());
  }

  glViewport (drawRect.leftBottom[0], drawRect.leftBottom[1], drawRect.size[0], drawRect.size[1]);

  clearAndRenderBackground ();
  // renderFrame (frameBufferSize);
  renderInput (drawRect.size);
}

void TerminalImpl::clearAndRenderBackground ()
{
  glClearColor (gBackgroundColor[0], gBackgroundColor[1], gBackgroundColor[2], gBackgroundColor[3]);
  glClear (GL_COLOR_BUFFER_BIT);
}

// void TerminalImpl::renderFrame (const Luma::Core::Vec2I &frameBufferSize)
// {
//   mWindowFrameVAO.bind ();
//   //DO_AFTER_SCOPE (mWindowFrameVAO.unbind ());

//   {
//     float halfFrameMargin = 0.5 * gFrameMargin;
//     float data[8] = {halfFrameMargin                     , halfFrameMargin,
//                      halfFrameMargin                     , frameBufferSize[1] - halfFrameMargin,
//                      frameBufferSize[0] - halfFrameMargin, frameBufferSize[1] - halfFrameMargin,
//                      frameBufferSize[0] - halfFrameMargin, halfFrameMargin};

//     mWindowFrameVBO.bind ();
//     mWindowFrameVBO.write (0 /*offset*/, data, sizeof (data));
//   }

//   mBasicShader.bind ();
//   DO_AFTER_SCOPE (mBasicShader.unbind ());

//   glLineWidth (gFrameMargin);
//   glDrawArrays (GL_LINE_LOOP /*mode*/, 0 /* first */, 4 /* count */);
// }


void TerminalImpl::renderInput (const Luma::Core::Vec2I &viewSize)
{
  renderInputText (viewSize);
  renderInputPos (viewSize);
}

void TerminalImpl::renderInputText (const Luma::Core::Vec2I &viewSize)
{
  mInputTextVAO.bind ();
  DO_AFTER_SCOPE (mInputTextVAO.unbind ());

  int promptSize = std::ssize (gPrompt);
  int userInputSize = std::ssize (mUserInputText);
  int inputSize = promptSize + userInputSize;

  if (mInputTextIsDirty)
    {
      DO_AFTER_SCOPE (mInputTextIsDirty = false);

      mInputTextVBO.bind ();
      mInputTextVBO.allocate (/* count */ inputSize * sizeof (char) );

      mInputTextVBO.write (/* offset */ 0,
                           /* data   */ gPrompt.data (),
                           /* count  */ gPrompt.size () * sizeof (char));

      mInputTextVBO.write (/* offset */ gPrompt.size () * sizeof (char),
                           /* data   */ mUserInputText.data (),
                           /* count  */ mUserInputText.size () * sizeof (char));
    }

  int inputBoxWidth = static_cast<int> (std::floor (viewSize[0] / gTextSize[0]));
  int inputBoxHeight = (inputSize - 1) / inputBoxWidth;

  mTextBoxShader.bind ();
  DO_AFTER_SCOPE (mTextBoxShader.unbind ());
  mTextBoxShader.setBoxWidth (inputBoxWidth);
  mTextBoxShader.setPosition (0, inputBoxHeight * gTextSize[1], 0.f);

  {
    mTextBoxShader.setColor (gPromptColor.data ());
    glDrawArrays (/* mode  */ GL_POINTS,
                  /* first */ 0,
                  /* count */ promptSize);
  }

  {
    mTextBoxShader.setColor (gUserInputColor.data ());
    glDrawArrays (/* mode  */ GL_POINTS,
                  /* first */ promptSize,
                  /* count */ userInputSize);
  }
}

void TerminalImpl::renderInputPos (const Luma::Core::Vec2I &viewSize)
{
  mInputPosVAO.bind ();
  DO_AFTER_SCOPE (mInputPosVAO.unbind ());

  int promptSize = std::ssize (gPrompt);
  int userInputSize = std::ssize (mUserInputText);
  int inputSize = promptSize + userInputSize;
  int inputPos = promptSize + mInputPos;

  int inputBoxWidth = static_cast<int> (std::floor (viewSize[0] / gTextSize[0]));
  int inputBoxHeight = (inputSize - 1) / inputBoxWidth;
  Luma::Geom::Vec2I posCell = {inputPos % inputBoxWidth, inputBoxHeight - inputPos / inputBoxWidth};

  if (mInputPosIsDirty)
    {
      DO_AFTER_SCOPE (mInputPosIsDirty = false);

      mInputPosVBO.bind ();

      float data[4] = {posCell[0]       * gTextSize[0],
                       posCell[1]       * gTextSize[1],
                       (posCell[0] + 1) * gTextSize[0],
                       posCell[1]       * gTextSize[1]};

      mInputPosVBO.write (/* offset */ 0,
                          /* data   */ data,
                          /* count  */ 4 * sizeof (float));
    }

  mBasicShader.bind ();
  DO_AFTER_SCOPE (mBasicShader.unbind ());

  {
    glLineWidth (2.f);
    glDrawArrays (/* mode  */ GL_LINES,
                  /* first */ 0,
                  /* count */ 2);
  }
}


void TerminalImpl::keyEvent (const Luma::Core::KeyEvent &event)
{
  if (   event.key () == Luma::Core::KeyEvent::Key::Key_Escape
      && event.action () == Luma::Core::KeyEvent::Action::Press)
    mWindow.destroy ();

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_F11
      && event.action () == Luma::Core::KeyEvent::Action::Press)
    {
      if (mWindow.isWindowed ())
        {
          mWindow.setMonitor (Luma::Core::Application::primaryMonitor ());
        }
      else
        {
          mWindow.setWindowed ({400, 300} /*pos*/, {800, 600} /*size*/);
        }
    }

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_F11
      && event.action () == Luma::Core::KeyEvent::Action::Press)
    {
      if (mWindow.isWindowed ())
        {
          mWindow.setMonitor (Luma::Core::Application::primaryMonitor ());
        }
      else
        {
          mWindow.setWindowed ({400, 300} /*pos*/, {800, 600} /*size*/);
        }
    }

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_Home
      && (event.action () == Luma::Core::KeyEvent::Action::Press))
    {
      moveInputPosHandle (0);
    }

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_End
      && (event.action () == Luma::Core::KeyEvent::Action::Press))
    {
      moveInputPosHandle (std::ssize (mUserInputText));
    }

  if (event.key () == Luma::Core::KeyEvent::Key::Key_Left)
    {
      if (event.action () == Luma::Core::KeyEvent::Action::Press)
        {
          moveInputPosHandle (mInputPos - 1);
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Repeat)
        {
          mKeyEventRepeatCount++;
          moveInputPosHandle (mInputPos - KeyEventRepeatForce ());
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Release)
        {
          mKeyEventRepeatCount = 0;
        }
    }

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_Right)
    {
      if (event.action () == Luma::Core::KeyEvent::Action::Press)
        {
          moveInputPosHandle (mInputPos + 1);
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Repeat)
        {
          mKeyEventRepeatCount++;
          moveInputPosHandle (mInputPos + KeyEventRepeatForce ());
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Release)
        {
          mKeyEventRepeatCount = 0;
        }
    }

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_BackSpace
      && (event.action () == Luma::Core::KeyEvent::Action::Press || event.action () == Luma::Core::KeyEvent::Action::Repeat))
    {
      if (event.action () == Luma::Core::KeyEvent::Action::Press)
        {
          eraseHandle (1);
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Repeat)
        {
          mKeyEventRepeatCount++;
          eraseHandle (KeyEventRepeatForce ());
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Release)
        {
          mKeyEventRepeatCount = 0;
        }
    }

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_Delete
      && (event.action () == Luma::Core::KeyEvent::Action::Press || event.action () == Luma::Core::KeyEvent::Action::Repeat))
    {
      if (event.action () == Luma::Core::KeyEvent::Action::Press)
        {
          deleteHandle (1);
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Repeat)
        {
          mKeyEventRepeatCount++;
          deleteHandle (KeyEventRepeatForce ());
        }

      if (event.action () == Luma::Core::KeyEvent::Action::Release)
        {
          mKeyEventRepeatCount = 0;
        }
    }

  if (   event.key () == Luma::Core::KeyEvent::Key::Key_V
      && event.mode () & Luma::Core::KeyEvent::Mode::Ctrl
      && (event.action () == Luma::Core::KeyEvent::Action::Press || event.action () == Luma::Core::KeyEvent::Action::Repeat))
    {
      pasteHandle ();
    }
}

void TerminalImpl::charEvent (const Luma::Core::CharEvent &event)
{
  if (' ' <= event.code () && event.code () <= '~')
    insertHandle (static_cast<char> (event.code ()));
}

void TerminalImpl::moveInputPosHandle (int pos)
{
  if (moveInputPosHandleUnsafe (pos))
    mWindow.update ();
}

bool TerminalImpl::moveInputPosHandleUnsafe (int pos)
{
  int userInputSize = std::ssize (mUserInputText);
  int newInputPos = std::clamp (pos, 0, userInputSize);

  if (newInputPos == mInputPos)
    return false;

  mInputPos = newInputPos;
  mInputPosIsDirty = true;

  return true;
}

void TerminalImpl::insertHandle (char c)
{
  mUserInputText.insert (mUserInputText.begin () + mInputPos, c);
  mInputTextIsDirty = true;

  moveInputPosHandleUnsafe (mInputPos+ 1);

  mWindow.update ();
}

void TerminalImpl::eraseHandle (int count)
{
  if (mUserInputText.empty ())
    return;

  if (mInputPos == 0)
    return;

  mUserInputText.erase (mUserInputText.begin () + mInputPos - count, mUserInputText.begin () + mInputPos);
  mInputTextIsDirty = true;

  moveInputPosHandleUnsafe (mInputPos - count);

  mWindow.update ();
}

void TerminalImpl::deleteHandle (int count)
{
  if (mUserInputText.empty ())
    return;

  if (mInputPos == std::ssize (mUserInputText))
    return;

  mUserInputText.erase (mUserInputText.begin () + mInputPos, mUserInputText.begin () + mInputPos + count);
  mInputTextIsDirty = true;

  moveInputPosHandleUnsafe (mInputPos);

  mWindow.update ();
}

void TerminalImpl::pasteHandle ()
{
  const char *clipboard = mWindow.clipboard ();
  if (clipboard == nullptr)
    return;

  int size = strlen (clipboard);
  mUserInputText.insert (mUserInputText.begin () + mInputPos, clipboard, clipboard + size);
  mInputTextIsDirty = true;

  moveInputPosHandleUnsafe (mInputPos + size);

  mWindow.update ();
}

int TerminalImpl::KeyEventRepeatForce () const
{
  if (mKeyEventRepeatCount <= 16)
    return 1;

  if (mKeyEventRepeatCount <= 256)
    return 2;

  return 3;
}

// void TerminalImpl::startSelectHandle ()
// {
//   Luma::Geom::Vec2I pos = mWindow.pos ();

// }

// void TerminalImpl::finishSelectHandle ()
// {

// }

void TerminalImpl::mouseEvent (const Luma::Core::MouseEvent &/*event*/)
{
  // if (event.button () == Luma::Core::MouseEvent::Button::BUTTON_1)
  //   {
  //     if (event.action () == Luma::Core::MouseEvent::Action::Press)
  //       startSelectHandle ();
  //     else
  //       finishSelectHandle ();
  //   }
}

void TerminalImpl::focusInEvent ()
{

}

void TerminalImpl::focusOutEvent ()
{

}

void TerminalImpl::init ()
{
  glewInit ();

  mTextBoxShader.create ();
  mTextLineShader.create ();
  mBasicShader.create ();

  {
    mTextBoxShader.bind ();
    DO_AFTER_SCOPE (mTextBoxShader.unbind ());

    mTextBoxShader.setBackgroundColor (gTextBackgroundColor.data ());
    mTextBoxShader.setSize (gTextSize.data ());
  }

  mTextBoxVAO.create ();
  mInputTextVAO.create ();
  mInputPosVAO.create ();
  // mWindowFrameVAO.create ();

  mTextBoxVBO.create ();
  mInputTextVBO.create ();
  // mWindowFrameVBO.create ();
  mInputPosVBO.create ();

  // {
  //   mWindowFrameVAO.bind ();
  //   DO_AFTER_SCOPE (mWindowFrameVAO.unbind ());

  //   mWindowFrameVBO.bind ();
  //   mWindowFrameVBO.allocate (4 * 2 * sizeof (float));

  //   glEnableVertexAttribArray (mBasicShader.positionAttributeLocation ());
  //   glVertexAttribPointer (mBasicShader.positionAttributeLocation (), 2, GL_FLOAT, GL_FALSE /*normalized*/, 0/*stride*/, nullptr);

  //   glVertexAttrib4f (mBasicShader.colorAttributeLocation (), gFrameColor[0], gFrameColor[1], gFrameColor[2], gFrameColor[3]);
  // }

  {
    mInputTextVAO.bind ();
    DO_AFTER_SCOPE (mInputTextVAO.unbind ());

    mInputTextVBO.bind ();

    glEnableVertexAttribArray (mTextBoxShader.textAttributeLocation ());
    glVertexAttribIPointer (mTextBoxShader.textAttributeLocation (), 1, GL_UNSIGNED_BYTE, 0 /*stride*/, nullptr);
  }

  {
    mInputPosVAO.bind ();
    DO_AFTER_SCOPE (mInputPosVAO.unbind ());

    mInputPosVBO.bind ();
    mInputPosVBO.allocate (/* count */ 4 * sizeof (float));

    glEnableVertexAttribArray (mBasicShader.positionAttributeLocation ());
    glVertexAttribPointer (mBasicShader.positionAttributeLocation (), 2, GL_FLOAT, GL_FALSE /*normalized*/, 0/*stride*/, nullptr);

    glVertexAttrib4f (mBasicShader.colorAttributeLocation (), gUserInputColor[0], gUserInputColor[1], gUserInputColor[2], gUserInputColor[3]);
  }
}

void TerminalImpl::scaleEvent (const Luma::Core::ScaleEvent &event)
{
  mWindow.update ();
}
