#include "GL/glew.h"
#include "Luma/Core/KeyEvent.h"
#include "Luma/Core/MouseEvent.h"
#include "Luma/Core/RenderEvent.h"
#include "Luma/Core/Window.h"
#include "Luma/GL/Buffer.h"
#include "Luma/GL/ShaderProgram.h"
#include "Luma/GL/VertexArray.h"
#include "Shader/ASCII/Debug/v150/TextBoxShader.h"
#include "Shader/ASCII/Debug/v150/TextLineShader.h"
#include "Shader/Triangle/v150/TriangleShader.h"

class TerminalImpl
{
public:
  TerminalImpl (Luma::Core::Window &window);

  void renderEvent (const Luma::Core::RenderEvent &event);
  void keyEvent (const Luma::Core::KeyEvent &event);
  void mouseEvent (const Luma::Core::MouseEvent &event);
  void focusInEvent ();
  void focusOutEvent ();
  void scaleEvent (const Luma::Core::ScaleEvent &event);

private:
  void init ();

private:
  Luma::Core::Window &mWindow;

  bool mInited = false;

  Shader::ASCII::Debug::v150::TextBoxShader mTextBoxShader;
  Shader::ASCII::Debug::v150::TextLineShader mTextLineShader;
  Shader::Triangle::v150::TriangleShader mTriangleShader;

  Luma::GL::VertexArray mTextBoxVAO;
  Luma::GL::Buffer mTextBoxVBO;

  Luma::GL::VertexArray mTextLineVAO;
  Luma::GL::Buffer mTextLineVBO;
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

  glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
  glClear (GL_COLOR_BUFFER_BIT);
  glViewport (0, 0, frameBufferSize[0], frameBufferSize[1]);
}

void TerminalImpl::keyEvent (const Luma::Core::KeyEvent &event)
{
  if (event.key () == Luma::Core::Key::Key_Escape && event.action () == Luma::Core::KeyAction::Press)
    mWindow.destroy ();
}

void TerminalImpl::mouseEvent (const Luma::Core::MouseEvent &/*event*/)
{

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
  mTriangleShader.create ();
}


void TerminalImpl::scaleEvent (const Luma::Core::ScaleEvent &event)
{
  mWindow.update ();
}
