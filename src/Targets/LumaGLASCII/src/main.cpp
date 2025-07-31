#include "GLASCII/v150/Debug/TextBoxShader.h"
#include "GLASCII/v150/Debug/TextLineShader.h"
#include "Luma/Core/Application.h"
#include "Luma/Core/KeyEvent.h"
#include "Luma/Core/Window.h"
#include "Luma/GL/Buffer.h"
#include "Luma/GL/VertexArray.h"
#include <GL/glew.h>
#include <stdexcept>

namespace
{
constexpr int TableWidth = 8;
constexpr int TableHeight = 32;
}

class MainWindow : public Luma::Core::Window
{
  GLASCII::v150::Debug::TextBoxShader mTextBoxShader;
  GLASCII::v150::Debug::TextLineShader mTextLineShader;

  Luma::GL::VertexArray mTextBoxVAO;
  Luma::GL::Buffer mTextBoxVBO;

  Luma::GL::VertexArray mTextLineVAO;
  Luma::GL::Buffer mTextLineVBO;

  bool mInited = false;
  int mScalePercent = 100;

  static constexpr int textVBOSize = 11;
  static constexpr int scalePercentStep = 10;
  static constexpr int maxScalePercent = 800;

public:
  MainWindow () noexcept: Luma::Core::Window ({800, 600}, "Luma GLASCII")
  {
  }

  void init ()
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    mTextBoxShader.create ();
    mTextLineShader.create ();

    {
      mTextLineVAO.create ();
      mTextLineVAO.bind ();

      /// 012345678910
      /// Scale: 100%
      ///

      mTextLineVBO.create ();
      mTextLineVBO.bind ();
      mTextLineVBO.allocate (textVBOSize, Luma::GL::Buffer::UsagePattern::StaticDraw);

      glEnableVertexAttribArray (mTextLineShader.textAttributeLocation ());
      glVertexAttribIPointer (mTextLineShader.textAttributeLocation (), 1, GL_UNSIGNED_BYTE, 0, nullptr);  /// Use current binded GL_ARRAY_BUFFER

      mTextLineVAO.unbind ();
    }

    {
      mTextBoxVAO.create ();
      mTextBoxVAO.bind ();

      /// ...
      /// _065_A
      /// _066_B_
      /// ...
      /// _120_x_
      /// ...
      char ASCIITable[7 * 256];
      for (int pos = 0; pos < 256; pos++)
        {
          int h = pos / TableWidth;
          int w = pos % TableWidth;

          int num = w * TableHeight + h;

          int posIndex = 7 * pos;

          ASCIITable[posIndex + 0] = ' ';
          ASCIITable[posIndex + 1] = '0' + static_cast<char> (num / 100);
          ASCIITable[posIndex + 2] = '0' + static_cast<char> ((num / 10) % 10);
          ASCIITable[posIndex + 3] = '0' + static_cast<char> (num % 10);
          ASCIITable[posIndex + 4] = ' ';
          ASCIITable[posIndex + 5] = static_cast<char> (num);
          ASCIITable[posIndex + 6] = ' ';
        }

      mTextBoxVBO.create ();
      mTextBoxVBO.bind ();
      mTextBoxVBO.allocate (ASCIITable, sizeof (ASCIITable), Luma::GL::Buffer::UsagePattern::StaticDraw);

      glEnableVertexAttribArray (mTextBoxShader.textAttributeLocation ());
      glVertexAttribIPointer (mTextBoxShader.textAttributeLocation (), 1, GL_UNSIGNED_BYTE, 0, nullptr);  /// Use current binded GL_ARRAY_BUFFER

      mTextBoxVAO.unbind ();
    }

    mTextBoxShader.bind ();
    mTextBoxShader.setColor (0.f, 0.f, 0.f, 1.f);
    mTextBoxShader.setBackgroundColor (0.45f, 0.45f, 0.45f, 1.f);
    mTextBoxShader.setBoxWidth (7 * TableWidth);
    mTextBoxShader.unbind ();

    mTextLineShader.bind ();
    mTextLineShader.setColor (0.f, 0.7f, 0.f, 1.f);
    mTextLineShader.setBackgroundColor (0.0f, 0.0f, 0.0f, 0.f);
    mTextLineShader.unbind ();
  }

  void renderEvent (const Luma::Core::RenderEvent &/*event*/) override
  {
    if (!mInited)
      {
        init ();
        mInited = true;
      }

    Luma::Core::Vec2I frameBufferSize = Luma::Core::Window::frameBufferSize ();
    float modelViewProjection[16] = { 2.f / frameBufferSize[0], 0.f                     , 0.f, 0.f,
                                      0.f                     , 2.f / frameBufferSize[1], 0.f, 0.f,
                                      0.f                     , 0.f                     , 1.f, 0.f,
                                     -1.f                     , -1.f                    , 0.f, 1.f};

    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    glViewport (0, 0, frameBufferSize[0], frameBufferSize[1]);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Luma::Geom::Vec2F glyphSize = {GLASCII::v150::Debug::glyphTextureWidth (),
                                   GLASCII::v150::Debug::glyphTextureHeight ()};
    {
      float scale = mScalePercent / 100.f;
      Luma::Geom::Vec2F scaledGlyphSize = glyphSize * scale;
      glDisable (GL_BLEND);
      mTextBoxShader.bind ();

      mTextBoxShader.setModelViewProjectionMatrix (modelViewProjection);
      mTextBoxShader.setSize (scaledGlyphSize.data ());
      mTextBoxShader.setPosition (0, frameBufferSize[1] - scaledGlyphSize[1], 0.f);

      mTextBoxVAO.bind ();
      glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, TableWidth * TableHeight * 7 /* count */);
      mTextBoxVAO.unbind ();

      mTextBoxShader.unbind ();
    }

    {
      float scale = 2.f;
      Luma::Geom::Vec2F scaledGlyphSize = glyphSize * scale;

      glEnable (GL_BLEND);
      mTextLineShader.bind ();

      mTextLineShader.setModelViewProjectionMatrix (modelViewProjection);
      mTextLineShader.setSize (scaledGlyphSize.data ());
      mTextLineShader.setPosition (0.f, 0.f, 0.f);

      mTextLineVAO.bind ();
      {
      mTextLineVBO.bind ();
      char text[textVBOSize + 1];
      sprintf (text, "scale: %d%%", mScalePercent);
      mTextLineVBO.write (0, text, textVBOSize);
      }
      glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, textVBOSize /* count */);
      mTextLineVAO.unbind ();

      mTextLineShader.unbind ();
    }
  }

  void keyEvent (const Luma::Core::KeyEvent &event) override
  {
    if (event.key () == Luma::Core::Key::Key_Escape && event.action () == Luma::Core::KeyAction::Press)
      destroy ();

    if (event.key () == Luma::Core::Key::Key_Up && (event.action () == Luma::Core::KeyAction::Press || event.action () == Luma::Core::KeyAction::Repeat))
      {
        if (int newScalePercent = mScalePercent + scalePercentStep; newScalePercent <= maxScalePercent)
          {
            mScalePercent = newScalePercent;
            update ();
          }
      }

    if (event.key () == Luma::Core::Key::Key_Down && (event.action () == Luma::Core::KeyAction::Press || event.action () == Luma::Core::KeyAction::Repeat))
      {
        if (int newScalePercent = mScalePercent - scalePercentStep; newScalePercent > 0)
          {
            mScalePercent = newScalePercent;
            update ();
          }
      }
  }

  int scalePercent (int scaleInterval)
  {
    return 100 + 5 * scaleInterval;
  }
};

int main ()
{
  Luma::Core::Application app;

  MainWindow window;

  app.exec ();
  return 0;
}
