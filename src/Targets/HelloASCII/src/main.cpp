#include "GL/ASCII/v150/Debug/TextLineShader.h"
#include "GL/KeyEvent.h"
#include "GL/ResizeEvent.h"
#include "GL/Window.h"
#include "GL/Buffer.h"
#include "GL/VertexArray.h"
#include "Geometry/Size.h"
#include <GL/glew.h>
#include <stdexcept>

namespace
{
constexpr int TableWidth = 8;
constexpr int TableHeight = 32;
}

class MyWindow : public GL::Window
{
  GL::ASCII::v150::Debug::TextLineShader mTextLineShader;
  GL::Buffer mTextVBO;
  GL::VertexArray mTextVAO;

public:
  MyWindow () noexcept = default;
  ~MyWindow () noexcept override
  {
  }

  void init () override
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    mTextLineShader.init ();

    mTextVAO.create ();
    mTextVAO.bind ();

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

    mTextVBO.create ();
    mTextVBO.bind ();
    mTextVBO.allocate (ASCIITable, sizeof (ASCIITable), GL::Buffer::UsagePattern::StaticDraw);
    mTextVBO.unbind ();

    mTextLineShader.bind ();
    mTextLineShader.setColor (0.f, 0.f, 0.f, 1.f);
    mTextLineShader.setBackgroundColor (0.45f, 0.45f, 0.45f, 1.f);
    mTextLineShader.unbind ();
  }

  void renderEvent () override
  {
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);

    mTextLineShader.bind ();

    constexpr float scale = 2.;
    float glyphSize[2] = {scale * 2.f * GL::ASCII::v150::Debug::glyphTextureWidth () / size ().width (),
                          scale * 2.f * GL::ASCII::v150::Debug::glyphTextureHeight () / size ().height ()};

    mTextLineShader.setSize (glyphSize);

    mTextVBO.bind ();
    glEnableVertexAttribArray (mTextLineShader.attributeCodeLocation ());
    for (int h = 0; h < TableHeight; h++)
      {
        mTextLineShader.setPosition (-1.f, 1.f - (h + 1) * glyphSize[1], 0.f);
        glVertexAttribIPointer (mTextLineShader.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, (void *) (h * TableWidth  * 7)); /// Use current binded GL_ARRAY_BUFFER
        glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, TableWidth * 7 /* count */);
      }
    glDisableVertexAttribArray (mTextLineShader.attributeCodeLocation ());
    mTextVBO.unbind ();

    mTextLineShader.unbind ();
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
        .setMaximized (true)
        .setFocused (true)
        .setAutoIconify (true)
        .setVersionMajor (3)
        .setVersionMinor (3)
        .setOpenGLProfile (GL::WindowCreateConfig::Profile::CORE);
  window.exec ();
}
