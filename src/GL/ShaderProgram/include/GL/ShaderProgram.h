#pragma once

#include <initializer_list>
#include <memory>
#include <optional>

namespace GL
{
class ShaderProgramBinder;
class ShaderProgram
{
public:
  ShaderProgram (int id) noexcept;
  ShaderProgram (const ShaderProgram &) = delete;
  ShaderProgram (ShaderProgram &&) noexcept;
  ~ShaderProgram () noexcept;

  ShaderProgram &operator= (const ShaderProgram &) = delete;
  ShaderProgram &operator= (ShaderProgram &&) noexcept;

  int id () const & noexcept;
  int id () && noexcept;

  int uniformLocation (const char *name) const noexcept;
  int attributeLocation (const char *name) const noexcept;

  ShaderProgramBinder bind () const noexcept;

private:   
  int mId;
};

class Shader;
class ShaderProgramLinker
{
public:
  ShaderProgramLinker () noexcept;
  ~ShaderProgramLinker () noexcept;

  std::optional<ShaderProgram> link (std::initializer_list<int> shaderIds) noexcept;
  const char *linkError () const noexcept;

private:
  std::unique_ptr<char[]> mLinkError;
};

class ShaderProgramBinder
{
public:
  ShaderProgramBinder (int id) noexcept;
  ShaderProgramBinder (const ShaderProgram &shaderProgram) noexcept;
  ShaderProgramBinder (const ShaderProgramBinder &) = delete;
  ShaderProgramBinder (ShaderProgramBinder &&) = delete;
  ~ShaderProgramBinder () noexcept;

  ShaderProgramBinder &operator= (const ShaderProgramBinder &) = delete;
  ShaderProgramBinder &operator= (ShaderProgramBinder &&) = delete;

  ShaderProgramBinder &setUniform (int location, float x) noexcept;
  ShaderProgramBinder &setUniform (int location, float x, float y) noexcept;
  ShaderProgramBinder &setUniform (int location, float x, float y, float z) noexcept;
  ShaderProgramBinder &setUniform (int location, float x, float y, float z, float w) noexcept;

  ShaderProgramBinder &setUniform (int location, int x) noexcept;
  ShaderProgramBinder &setUniform (int location, int x, int y) noexcept;
  ShaderProgramBinder &setUniform (int location, int x, int y, int z) noexcept;
  ShaderProgramBinder &setUniform (int location, int x, int y, int z, int w) noexcept;

  ShaderProgramBinder &setUniform (int location, unsigned int x) noexcept;
  ShaderProgramBinder &setUniform (int location, unsigned int x, unsigned int y) noexcept;
  ShaderProgramBinder &setUniform (int location, unsigned int x, unsigned int y, unsigned int z) noexcept;
  ShaderProgramBinder &setUniform (int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) noexcept;

  ShaderProgramBinder &setUniformMatrix2 (int location, const float *m, bool transpose = true, int count = 1) noexcept;
  ShaderProgramBinder &setUniformMatrix3 (int location, const float *m, bool transpose = true, int count = 1) noexcept;
  ShaderProgramBinder &setUniformMatrix4 (int location, const float *m, bool transpose = true, int count = 1) noexcept;

private:
  int mId;
};
}
