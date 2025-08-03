#pragma once

namespace Shader::Basic::v110
{
class BasicShader
{
public:
  BasicShader () noexcept;
  BasicShader (const BasicShader &) = delete;
  BasicShader (BasicShader &&) = delete;
  ~BasicShader () noexcept;

  BasicShader &operator= (const BasicShader &) = delete;
  BasicShader &operator= (BasicShader &&) = delete;

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
}  // namespace Shader::Basic::v110
