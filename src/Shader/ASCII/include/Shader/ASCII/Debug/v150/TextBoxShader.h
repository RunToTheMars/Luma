#pragma once


namespace Shader::ASCII::Debug::v150
{
class TextBoxShader
{
public:
  TextBoxShader () noexcept;
  TextBoxShader (const TextBoxShader &) = delete;
  TextBoxShader (TextBoxShader &&) = delete;
  ~TextBoxShader () noexcept;

  TextBoxShader &operator= (const TextBoxShader &) = delete;
  TextBoxShader &operator= (TextBoxShader &&) = delete;

  /// \note This function call bind and unbind
  void create () noexcept;
  bool isCreated () const noexcept;
  void destroy () noexcept;

  const float *position () const noexcept;
  const float *color () const noexcept;
  const float *backgroundColor () const noexcept;
  const float *size () const noexcept;
  const float *modelViewProjectionMatrix () const noexcept;
  int boxWidth () const noexcept;

  /// -------------------------------
  /// \note Only for Created
  int textAttributeLocation () const;

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

  void setModelViewProjectionMatrix (const float *matrix) noexcept;

  void setBoxWidth (int width) noexcept;
  /// -------------------------------

private:
  int mProgramId = 0;

  int mUniformPositionLoc = 0;
  int mUniformColorLoc = 0;
  int mUniformBackgroundColorLoc = 0;
  int mUniformSizeLoc = 0;
  int mUniformModelViewProjectionMatrixLoc = 0;
  int mUniformBoxWidthLoc = 0;

  int mTextAttributeLoc = 0;

  float mPosition[3] = {0.f, 0.f, 0.f};
  float mColor[4] = {0.f, 0.f, 0.f};
  float mBackgroundColor[4] = {0.f, 0.f, 0.f, 0.f};
  float mSize[2] = {0.f, 0.f};
  float mModelViewProjectionMatrix[16] = {1.f, 0.f, 0.f, 0.f,
                                          0.f, 1.f, 0.f, 0.f,
                                          0.f, 0.f, 1.f, 0.f,
                                          0.f, 0.f, 0.f, 1.f};
  int mBoxWidth = 16;
};
}  // namespace Shader::ASCII::v150::Debug
