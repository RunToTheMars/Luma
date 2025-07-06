#include "GL/ASCII/v150/Debug/TextBoxShader.h"
#include "GL/Buffer.h"
#include "GL/KeyEvent.h"
#include "GL/VertexArray.h"
#include "GL/Widget.h"
#include "GL/Widgets/FrameRateWidget.h"
#include "GL/Window.h"
#include <GL/glew.h>
#include <stdexcept>

namespace
{
constexpr int TableWidth = 8;
constexpr int TableHeight = 32;
}

class MainWidget : public GL::Widget
{
  GL::ASCII::v150::Debug::TextBoxShader mTextBoxShader;
  GL::Buffer mTextVBO;
  GL::VertexArray mTextVAO;

public:
  MainWidget () noexcept = default;
  ~MainWidget () noexcept override = default;

  void init ()
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    mTextBoxShader.init ();

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

    glEnableVertexAttribArray (mTextBoxShader.attributeCodeLocation ());
    glVertexAttribIPointer (mTextBoxShader.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, nullptr);  /// Use current binded GL_ARRAY_BUFFER

    mTextVAO.unbind ();

    mTextBoxShader.bind ();
    mTextBoxShader.setColor (0.f, 0.f, 0.f, 1.f);
    mTextBoxShader.setBackgroundColor (0.45f, 0.45f, 0.45f, 1.f);
    mTextBoxShader.setBoxWidth (7 * TableWidth);
    mTextBoxShader.unbind ();

    addWidget (std::make_unique<GL::FrameRateWidget> ());
  }

  void renderEvent () override
  {
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);

    mTextBoxShader.bind ();

    constexpr float scale = 2.;
    float glyphSize[2] = {scale * 2.f * GL::ASCII::v150::Debug::glyphTextureWidth () / GL::Window::getInstance ().size ().width (),
                          scale * 2.f * GL::ASCII::v150::Debug::glyphTextureHeight () / GL::Window::getInstance ().size ().height ()};

    mTextBoxShader.setSize (glyphSize);
    mTextBoxShader.setPosition (-1.f, 1.f - glyphSize[1], 0.f);

    mTextVAO.bind ();
    glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, TableWidth * TableHeight * 7 /* count */);
    mTextVAO.unbind ();

    mTextBoxShader.unbind ();
  }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      GL::Window::getInstance ().close ();
  }
};

int main ()
{
  GL::Window::getInstance ().create ({800, 600} /* size */, "Hello ASCII!")
   .setResizable (true)
   .setDecorated (true)
   .setVisible (true)
   .setMaximized (true)
   .setFocused (true)
   .setAutoIconify (true)
   .setVersionMajor (3)
   .setVersionMinor (3)
   .setOpenGLProfile (GL::WindowCreateConfig::Profile::Core);

  if (glewInit () != GLEW_OK)
    throw std::runtime_error ("Can't init glew");

  std::unique_ptr<MainWidget> mainWidget = std::make_unique<MainWidget> ();
  mainWidget->init ();
  GL::Window::getInstance ().open (std::move (mainWidget));

  // std::unique_ptr<GL::FrameRateWidget> frameRateWidget = std::make_unique<GL::FrameRateWidget> ();
  // frameRateWidget->init ();
  // GL::Window::getInstance ().open (std::move (frameRateWidget));
}
