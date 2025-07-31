#include "GL/glew.h"
#include "GLASCII/v150/Debug/TextBoxShader.h"
#include "GLASCII/v150/Debug/TextLineShader.h"
#include "Luma/Core/KeyEvent.h"
#include "Luma/Core/MouseEvent.h"
#include "Luma/Core/RenderEvent.h"
#include "Luma/Core/Window.h"
#include "Luma/GL/Buffer.h"
#include "Luma/GL/VertexArray.h"
#include "Luma/GL/ShaderProgram.h"

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

  GLASCII::v150::Debug::TextBoxShader mTextBoxShader;
  GLASCII::v150::Debug::TextLineShader mTextLineShader;
  Luma::GL::ShaderProgram mFillShader;

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

  {
    Luma::GL::Shader vertexShader (Luma::GL::Shader::Type::Vertex);
    Luma::GL::Shader geometryShader (Luma::GL::Shader::Type::Geometry);
    Luma::GL::Shader fragmentShader (Luma::GL::Shader::Type::Fragment);

    vertexShader.create ();
    vertexShader.compileSource (R"(#version 130

                                        void main ()
                                        {
                                          gl_Position = glVertex;
                                        })");

    geometryShader.create ();
    geometryShader.compileSource (R"(#version 150

                                     layout (lines_adjacency) in;
                                     layout (triangle_strip, max_vertices = 4) out;

                                     void emitVertex (in int i);

                                     void main()
                                     {
                                       emitVertex (0);
                                       emitVertex (1);
                                       emitVertex (3);
                                       emitVertex (2);
                                       EndPrimitive ();
                                     }

                                     void emitVertex (in int i)
                                     {
                                       gl_Position = vec4 (gl_in[i].gl_Position);
                                       EmitVertex ();
                                     })");

    fragmentShader.create ();
    fragmentShader.compileSource (R"(#version 130

                                     uniform vec4 u_color;

                                     out vec4 FragColor;

                                     void main()
                                     {
                                       FragColor = u_color;
                                     })");

    mFillShader.create ();
    mFillShader.attach (vertexShader.id ());
    mFillShader.attach (geometryShader.id ());
    mFillShader.attach (fragmentShader.id ());
    mFillShader.link ();
  }
}


void TerminalImpl::scaleEvent (const Luma::Core::ScaleEvent &event)
{
  mWindow.update ();
}
