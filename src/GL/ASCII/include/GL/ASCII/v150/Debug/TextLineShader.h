#pragma once

#include "Common.h"

namespace GL
{
namespace ASCII
{
namespace v150
{
namespace Debug
{
class TextLineShader
{
public:
  TextLineShader () noexcept;
  TextLineShader (const TextLineShader &) = delete;
  TextLineShader (TextLineShader &&) = delete;
  ~TextLineShader () noexcept;

  TextLineShader &operator= (const TextLineShader &) = delete;
  TextLineShader &operator= (TextLineShader &&) = delete;

  void init () noexcept;

  const float *position () const noexcept;
  const float *color () const noexcept;
  const float *backgroundColor () const noexcept;
  const float *size () const noexcept;

  /// -------------------------------
  /// \note Only for Inited
  int attributeCodeLocation () const;

  void bind ();
  void unbind ();
  /// -------------------------------

  /// -------------------------------
  /// \note Only for Binded
  void setPosition (const float *pos) noexcept;
  void setPosition (float x, float y, float z) noexcept;

  void setColor (const float *color) noexcept;
  void setColor (float r, float g, float b, float a = 1.f) noexcept;

  void setBackgroundColor (const float *color) noexcept;
  void setBackgroundColor (float r, float g, float b, float a = 1.f) noexcept;

  void setSize (const float *size) noexcept;
  void setSize (float width, float height) noexcept;
  /// -------------------------------

private:
  int mProgramId = 0;

  int mUniformPositionLoc = 0;
  int mUniformColorLoc = 0;
  int mUniformBackgroundColorLoc = 0;
  int mUniformSizeLoc = 0;

  int mAttributeCodeLoc = 0;

  float mPosition[3] = {0.f, 0.f, 0.f};
  float mColor[4] = {0.f, 0.f, 0.f};
  float mBackgroundColor[4] = {0.f, 0.f, 0.f, 0.f};
  float mSize[2] = {0.f, 0.f};
};
}  // namespace Debug
}  // namespace v150
}  // namespace ASCII
}  // namespace GL
