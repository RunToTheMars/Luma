#pragma once

#include "GL/ShaderProgram.h"

namespace GL
{
namespace ASCII
{
namespace v130
{
class Renderer
{
public:
  Renderer () noexcept;
  Renderer (const Renderer &) = delete;
  Renderer (Renderer &&) = delete;
  ~Renderer () noexcept;

  Renderer &operator= (const Renderer &) = delete;
  Renderer &operator= (Renderer &&) = delete;

  void initialize () noexcept;
  bool isInitialized () const noexcept;
  void clear () noexcept;

  bool hasError () const noexcept;
  const std::unique_ptr<char[]> &error () const noexcept;
  std::unique_ptr<char[]> &&takeError () noexcept;

  static constexpr int glyphTextureWidth () { return 8; }
  static constexpr int glyphTextureHeight () { return 16; }

  const float *position () const noexcept;
  const float *color () const noexcept;
  const float *backgroundColor () const noexcept;
  const float *size () const noexcept;

  class Data;
  class Binder;
  Binder bind (const ShaderProgramBinderInterface &binderInterface) noexcept;

  class Data
  {
  public:
    Data (const char *symbols) noexcept;
    Data (const char *symbols, unsigned int symbolsCount) noexcept;
    Data (const Data &) = delete;
    Data (Data &&) = delete;
    ~Data () noexcept;

    Data &operator= (const Data &) = delete;
    Data &operator= (Data &&) = delete;

    unsigned int VBO () const noexcept;
    unsigned int symbolsCount () const noexcept;

  private:
    unsigned int mVBO;
    unsigned int mSymbolsCount;
  };

  class Binder
  {
  public:
    ~Binder () noexcept;

    Binder &setPosition (const float *pos) noexcept;
    Binder &setPosition (float x, float y, float z) noexcept;

    Binder &setColor (const float *color) noexcept;
    Binder &setColor (float r, float g, float b, float a = 1.f) noexcept;

    Binder &setBackgroundColor (const float *color) noexcept;
    Binder &setBackgroundColor (float r, float g, float b, float a = 1.f) noexcept;

    Binder &setSize (const float *size) noexcept;
    Binder &setSize (float width, float height) noexcept;

    Binder &draw (const Data &data) noexcept;

  private:
    friend class Renderer;
    Binder (const Renderer &renderer, const ShaderProgramBinderInterface &binderInterface) noexcept;

    const Renderer &mRenderer;
    const GL::ShaderProgramBinderInterface &mBinderInterface;
  };

private:
  bool mIsInitialized = false;
  std::optional<GL::ShaderProgram> mShaderProgram;
  std::unique_ptr<char[]> mError;

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
}  // namespace v130
}  // namespace ASCII
}  // namespace GL
