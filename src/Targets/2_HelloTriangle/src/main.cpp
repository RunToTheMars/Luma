#include "GL/KeyEvent.h"
#include "GL/ResizeEvent.h"
#include "GL/Window.h"
#include "GL/Buffer.h"
#include "GL/ShaderProgram.h"
#ifdef COREPROFILE
#include "GL/VertexArray.h"
#endif
#include "Geometry/Size.h"
#include <GL/glew.h>
#include <stdexcept>
#include <stdio.h>

namespace
{
class TriangleShader
{
public:
  void init ()
  {
    GL::Shader vertexShader (GL::Shader::Type::Vertex);
    vertexShader.create ();
    vertexShader.compileSource (
        R"(#version 130
           in vec2 aVertex;
           in vec4 aColor;

           out vec4 vColor;
           void main()
           {
             gl_Position = vec4(aVertex.x, aVertex.y, 0., 1.0);
             vColor = aColor;
           })");

    if (vertexShader.hasCompileError ())
      {
        int size = vertexShader.compileErrorSize ();
        std::unique_ptr<char[]> error = std::make_unique<char[]> (size);

        vertexShader.compileError (error.get (), size);
        fprintf (stderr, "Vertex Shader compilation fail: %s\n", error.get ());
      }

    GL::Shader fragmentShader (GL::Shader::Type::Fragment);
    fragmentShader.create ();
    fragmentShader.compileSource (
        R"(#version 130
           in vec4 vColor;
           void main()
           {
             gl_FragColor = vColor;
           })");

    if (fragmentShader.hasCompileError ())
      {
        int size = fragmentShader.compileErrorSize ();
        std::unique_ptr<char[]> error = std::make_unique<char[]> (size);

        fragmentShader.compileError (error.get (), size);
        fprintf (stderr, "Fragment Shader compilation fail: %s\n", error.get ());
      }

    mProgram.create ();
    mProgram.attach (vertexShader.id ());
    mProgram.attach (fragmentShader.id ());
    mProgram.link ();

    if (mProgram.hasLinkError ())
      {
        int size = mProgram.linkErrorSize ();
        std::unique_ptr<char[]> error = std::make_unique<char[]> (size);

        mProgram.linkError (error.get (), size);
        fprintf (stderr, "Shader Program link fail: %s\n", error.get ());
      }

    mVertexAttributeLocation = mProgram.attributeLocation ("aVertex");
    mColorAttributeLocation  = mProgram.attributeLocation ("aColor");
  }

  int vertexAttributeLocation () const { return mVertexAttributeLocation; }
  int colorAttributeLocation () const{ return mColorAttributeLocation; }
  GL::ShaderProgram &program () { return mProgram; }

private:
  GL::ShaderProgram mProgram;
  int mVertexAttributeLocation;
  int mColorAttributeLocation;
};
}  // namespace

class MyWindow : public GL::Window
{
  TriangleShader mTriangleShader;
  GL::Buffer mTriangleVertexVBO;
  GL::Buffer mTriangleColorVBO;

#ifdef COREPROFILE
  GL::VertexArray mTriangleVAO;
#endif

public:
  MyWindow () noexcept = default;
  ~MyWindow () noexcept override {}

  void init () override
  {
    if (glewInit () != GLEW_OK)
      throw std::runtime_error ("Can't init glew");

    mTriangleShader.init ();

    float vertices[6] =
    {
      -0.8f, -0.8f,
       0.f,   0.8f,
       0.8f, -0.8f,
    };

    float colors[12] =
    {
      1.f, 0.f, 0.f, 1.f,
      0.f, 1.f, 0.f, 1.f,
      0.f, 0.f, 1.f, 1.f,
    };

#ifdef COREPROFILE
    mTriangleVAO.create ();
    mTriangleVAO.bind ();

    mTriangleVertexVBO.create ();
    mTriangleVertexVBO.bind ();
    mTriangleVertexVBO.allocate (vertices, 3 * 2 * sizeof (float), GL::Buffer::UsagePattern::StaticDraw);

    glVertexAttribPointer (mTriangleShader.vertexAttributeLocation (), 2, GL_FLOAT, 0 /*normalized*/, 0 /*stride*/, (void *) nullptr);  /// Use current binded GL_ARRAY_BUFFER
    glEnableVertexAttribArray (mTriangleShader.vertexAttributeLocation ());

    mTriangleColorVBO.create ();
    mTriangleColorVBO.bind ();
    mTriangleColorVBO.allocate (colors, 3 * 4 * sizeof (float), GL::Buffer::UsagePattern::StaticDraw);

    glVertexAttribPointer (mTriangleShader.colorAttributeLocation (), 4, GL_FLOAT, 0 /*normalized*/, 0 /*stride*/, (void *) nullptr);  /// Use current binded GL_ARRAY_BUFFER
    glEnableVertexAttribArray (mTriangleShader.colorAttributeLocation ());

    mTriangleVAO.unbind ();
#else
    mTriangleVertexVBO.create ();
    mTriangleVertexVBO.bind ();
    mTriangleVertexVBO.allocate (vertices, 3 * 2 * sizeof (float), GL::Buffer::UsagePattern::StaticDraw);

    mTriangleColorVBO.create ();
    mTriangleColorVBO.bind ();
    mTriangleColorVBO.allocate (colors, 3 * 4 * sizeof (float), GL::Buffer::UsagePattern::StaticDraw);

    mTriangleColorVBO.unbind ();
#endif // COREPROFILE
  }

  void renderEvent () override
  {
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);

#ifdef COREPROFILE
    mTriangleVAO.bind ();
    mTriangleShader.program ().bind ();
    glDrawArrays (GL_TRIANGLES /*mode*/, 0 /* first */, 3 /* count */);
    mTriangleShader.program ().unbind ();
    mTriangleVAO.unbind ();
#else
    mTriangleVertexVBO.bind ();
    glVertexAttribPointer (mTriangleShader.vertexAttributeLocation (), 2, GL_FLOAT, 0 /*normalized*/, 0 /*stride*/, (void *) nullptr);  /// Use current binded GL_ARRAY_BUFFER

    mTriangleColorVBO.bind ();
    glVertexAttribPointer (mTriangleShader.colorAttributeLocation (), 4, GL_FLOAT, 0 /*normalized*/, 0 /*stride*/, (void *) nullptr);  /// Use current binded GL_ARRAY_BUFFER

    mTriangleColorVBO.unbind ();

    glEnableVertexAttribArray (mTriangleShader.vertexAttributeLocation ());
    glEnableVertexAttribArray (mTriangleShader.colorAttributeLocation ());

    mTriangleShader.program ().bind ();
    glDrawArrays (GL_TRIANGLES /*mode*/, 0 /* first */, 3 /* count */);
    mTriangleShader.program ().unbind ();

    glDisableVertexAttribArray (mTriangleShader.vertexAttributeLocation ());
    glDisableVertexAttribArray (mTriangleShader.colorAttributeLocation ());
#endif
  }

  void resizeEvent (const GL::ResizeEvent &event) override
  {
    glViewport (0, 0, event.size ().width (), event.size ().height ());
    GL::Widget::resizeEvent (event);
  }

  void keyEvent (const GL::KeyEvent &event) override
  {
    if (event.key () == GL::Key::Key_Escape && event.action () == GL::KeyAction::Press)
      close ();
  }
};

int main ()
{
    MyWindow window;
#ifdef COREPROFILE
    window.create ({800, 600} /* size */, "Hello Triangle! (Core)")
#else
    window.create ({800, 600} /* size */, "Hello Triangle! (Compatibility)")
#endif
    .setResizable (true)
    .setDecorated (true)
    .setVisible (true)
    .setMaximized (true)
    .setFocused (true)
    .setAutoIconify (true)
    .setVersionMajor (3)
    .setVersionMinor (3)
#ifdef COREPROFILE
    .setOpenGLProfile (GL::WindowCreateConfig::Profile::CORE);
#else
    .setOpenGLProfile (GL::WindowCreateConfig::Profile::COMPAT);
#endif
    window.exec ();
}
