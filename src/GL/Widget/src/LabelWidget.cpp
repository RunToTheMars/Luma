#include "GL/Widgets/LabelWidget.h"
#include "Common/Signal.h"
#include "GL/ASCII/v150/Debug/TextLineShader.h"
#include "GL/Buffer.h"
#include "GL/VertexArray.h"
#include "GL/Window.h"
#include <GL/glew.h>
#include <chrono>

namespace GL
{
class LabelWidgetImpl
{
  static constexpr double scale = 1.;

public:
  LabelWidgetImpl () noexcept;

  void init () noexcept;
  void renderEvent () noexcept;

private:
  void WindowSizeChangedHandle (const Geometry::Size &size) noexcept;

private:
  std::chrono::steady_clock::time_point mStartTime;
  int mFrames = 0;

  GL::ASCII::v150::Debug::TextLineShader mTextLineShader;
  GL::Buffer mTextVBO;
  GL::VertexArray mTextVAO;

  Common::Signal<Geometry::Size>::Connection mWindowSizeChanged;
};

LabelWidgetImpl::LabelWidgetImpl () noexcept
  : mWindowSizeChanged (GL::Window::getInstance ().sizeChanged.connect ([&] (const Geometry::Size &size) { WindowSizeChangedHandle (size); }))
{
  init ();
}

void LabelWidgetImpl::init () noexcept
{
  mTextLineShader.init ();

  mTextLineShader.bind ();
  mTextLineShader.setPosition (-1.f, -1.f, 0.f);
  mTextLineShader.setColor (0.f, 1.f, 0.f, 1.f);
  mTextLineShader.setBackgroundColor (1.f, 1.f, 1.f, 0.f);
  mTextLineShader.setSize (
    scale * 2.f * GL::ASCII::v150::Debug::glyphTextureWidth () / GL::Window::getInstance ().size ().width (),
    scale * 2.f * GL::ASCII::v150::Debug::glyphTextureHeight () / GL::Window::getInstance ().size ().height ());
  mTextLineShader.unbind ();

  mTextVAO.create ();
  mTextVAO.bind ();

  mTextVBO.create ();
  mTextVBO.bind ();
  mTextVBO.allocate (9, GL::Buffer::UsagePattern::StreamDraw);

  glVertexAttribIPointer (mTextLineShader.attributeCodeLocation (), 1, GL_UNSIGNED_BYTE, 0, (void *) nullptr);  /// Use current binded GL_ARRAY_BUFFER
  glEnableVertexAttribArray (mTextLineShader.attributeCodeLocation ());

  mTextVAO.unbind ();

  mStartTime = std::chrono::steady_clock::now ();
  mFrames = 0;
}

void LabelWidgetImpl::renderEvent () noexcept
{
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  mTextVAO.bind ();

  std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now ();
  const std::chrono::duration<double> elapsedSeconds{curTime - mStartTime};
  mFrames++;
  double dur_secs = elapsedSeconds.count ();
  if (dur_secs > 1.)
    {
      double fps = static_cast<double> (mFrames) / dur_secs;
      mStartTime = curTime;
      char text[16];
      sprintf (text, "FPS:%4d", static_cast<int> (fps));

      mTextVBO.write (0, text, 9);
      mFrames = 0;
    }

  mTextLineShader.bind ();
  glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, 9 /* count */);
  mTextLineShader.unbind ();

  mTextVAO.unbind ();
}

void LabelWidgetImpl::WindowSizeChangedHandle (const Geometry::Size &size) noexcept
{
  mTextLineShader.bind ();
  mTextLineShader.setSize (
      scale * 2.f * GL::ASCII::v150::Debug::glyphTextureWidth () / GL::Window::getInstance ().size ().width (),
      scale * 2.f * GL::ASCII::v150::Debug::glyphTextureHeight () / GL::Window::getInstance ().size ().height ());
  mTextLineShader.unbind ();
}

LabelWidget::LabelWidget () noexcept: mPimpl (std::make_unique<LabelWidgetImpl> ())
{
}

LabelWidget::~LabelWidget () noexcept = default;

void LabelWidget::renderEvent () noexcept
{
  mPimpl->renderEvent ();
}
}
