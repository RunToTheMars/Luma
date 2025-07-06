#include "GL/ASCII/v150/Debug/TextBoxShader.h"
#include "GL/Buffer.h"
#include "GL/KeyEvent.h"
#include "GL/VertexArray.h"
#include "GL/Widget.h"
#include "GL/Window.h"
#include <GL/glew.h>
#include <array>
#include <stdexcept>

namespace
{
constexpr int Width = 32;
constexpr int Height = 8;
}

class MainWidget : public GL::Widget
{
  GL::ASCII::v150::Debug::TextBoxShader mTextBoxShader;
  GL::Buffer mTextVBO;
  GL::VertexArray mTextVAO;

  std::array<char, Width * Height> mBuffer;
  int mBufferSize = 0;

public:
  MainWidget () noexcept = default;
  ~MainWidget () noexcept override
  {
  }

  void init ()
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    mTextBoxShader.init ();

    mTextVAO.create ();
    mTextVAO.bind ();

    mTextVBO.create ();
    mTextVBO.bind ();
    mTextVBO.allocate (Width * Height, GL::Buffer::UsagePattern::DynamicDraw);
    mTextVBO.unbind ();

    mTextBoxShader.bind ();
    mTextBoxShader.setColor (0.f, 0.f, 0.f, 1.f);
    mTextBoxShader.setBackgroundColor (0.45f, 0.45f, 0.45f, 1.f);
    mTextBoxShader.setBoxWidth (Width);
    mTextBoxShader.unbind ();
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

    mTextVBO.bind ();
    mTextVBO.write (0, mBuffer.data (), mBufferSize);

    glEnableVertexAttribArray (mTextBoxShader.attributeCodeLocation ());
    glVertexAttribIPointer (mTextBoxShader.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, nullptr); /// Use current binded GL_ARRAY_BUFFER
    glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, mBufferSize /* count */);
    glDisableVertexAttribArray (mTextBoxShader.attributeCodeLocation ());
    mTextVBO.unbind ();

    mTextBoxShader.unbind ();
  }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      return GL::Window::getInstance ().close ();

    int key_code = static_cast<int> (event.key ());
    if ((static_cast<int> (GL::Key::Key_Space) <= key_code && key_code <= static_cast<int> (GL::Key::Key_GraveAcent))
         && (event.action () == GL::KeyAction::Press || event.action () == GL::KeyAction::Repeat))
      {
        push (static_cast<char> (event.key ()));
        return;
      }

    if ((   event.key () == GL::Key::Key_Delete
         || event.key () == GL::Key::Key_BackSpace)
        && (event.action () == GL::KeyAction::Press || event.action () == GL::KeyAction::Repeat))
      {
        pop ();
        return;
      }
  }

  void push (char c)
  {
    if (mBufferSize >= Width * Height)
      return;

    mBuffer[mBufferSize] = c;
    mBufferSize++;
  }

  void pop ()
  {
    if (mBufferSize == 0)
      return;

    mBufferSize--;
  }
};

int main ()
{
  GL::Window::getInstance ().create ({800, 600} /* size */, "Hello Terminal!")
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
}
