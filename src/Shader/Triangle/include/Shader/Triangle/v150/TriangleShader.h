#pragma once

namespace Shader::Triangle::v150
{
class TriangleShader
{
public:
  TriangleShader () noexcept;
  TriangleShader (const TriangleShader &) = delete;
  TriangleShader (TriangleShader &&) = delete;
  ~TriangleShader () noexcept;

  TriangleShader &operator= (const TriangleShader &) = delete;
  TriangleShader &operator= (TriangleShader &&) = delete;

  /// \note This function call bind and unbind
  void create () noexcept;
  bool isCreated () const noexcept;
  void destroy () noexcept;

  const float *modelViewProjectionMatrix () const noexcept;

  /// -------------------------------
  /// \note Only for Created
  int positionAttributeLocation () const;
  int colorAttributeLocation () const;

  void bind ();
  void unbind ();
  /// -------------------------------

  /// -------------------------------
  /// \note Only for Binded
  void setModelViewProjectionMatrix (const float *matrix) noexcept;
  /// -------------------------------

private:
  int mProgramId = 0;

  int mUniformModelViewProjectionMatrixLoc = 0;

  int mPositionAttributeLoc = 0;
  int mColorAttributeLoc = 0;

  float mModelViewProjectionMatrix[16] = {1.f, 0.f, 0.f, 0.f,
                                          0.f, 1.f, 0.f, 0.f,
                                          0.f, 0.f, 1.f, 0.f,
                                          0.f, 0.f, 0.f, 1.f};
};
}  // namespace GLASCII::v150::Debug
