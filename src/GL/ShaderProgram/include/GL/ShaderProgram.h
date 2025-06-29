#pragma once

#include <memory>
#include <optional>

namespace GL
{
class ShaderProgramLinker;
class ShaderProgramBinderInterface;

class ShaderProgram
{
public:
  ShaderProgram (const ShaderProgram &) = delete;
  ShaderProgram (ShaderProgram &&) noexcept;
  ~ShaderProgram () noexcept;

  ShaderProgram &operator= (const ShaderProgram &) = delete;
  ShaderProgram &operator= (ShaderProgram &&) noexcept;

  int uniformLocation (const char *name) const noexcept;
  int attributeLocation (const char *name) const noexcept;

private:
  friend class GL::ShaderProgramLinker;
  friend class GL::ShaderProgramBinderInterface;

  ShaderProgram (int id) noexcept;

  int mId;
};

class Shader;
class ShaderProgramLinker
{
public:
  ShaderProgramLinker () noexcept;
  ~ShaderProgramLinker () noexcept;

  std::optional<ShaderProgram> link (std::initializer_list<const Shader *> shaders) noexcept;

  const std::unique_ptr<char[]> &linkError () const noexcept;
  std::unique_ptr<char[]> &&takeLinkError () noexcept;

private:
  std::unique_ptr<char[]> mLinkError;
};

class ShaderProgramBinderInterface
{
protected:
  ShaderProgramBinderInterface () noexcept;
  ~ShaderProgramBinderInterface () noexcept;

public:
  const ShaderProgramBinderInterface &bind (const ShaderProgram &) const noexcept;

  const ShaderProgramBinderInterface &setUniform (int location, float x) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, float x, float y) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, float x, float y, float z) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, float x, float y, float z, float w) const noexcept;

  const ShaderProgramBinderInterface &setUniform (int location, int x) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, int x, int y) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, int x, int y, int z) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, int x, int y, int z, int w) const noexcept;

  const ShaderProgramBinderInterface &setUniform (int location, unsigned int x) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, unsigned int x, unsigned int y) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, unsigned int x, unsigned int y, unsigned int z) const noexcept;
  const ShaderProgramBinderInterface &setUniform (int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const noexcept;

  const ShaderProgramBinderInterface &setUniformMatrix2 (int location, const float *m, bool transpose = true, int count = 1) const noexcept;
  const ShaderProgramBinderInterface &setUniformMatrix3 (int location, const float *m, bool transpose = true, int count = 1) const noexcept;
  const ShaderProgramBinderInterface &setUniformMatrix4 (int location, const float *m, bool transpose = true, int count = 1) const noexcept;
};

class ShaderProgramBinderRestore : public ShaderProgramBinderInterface
{
public:
  ShaderProgramBinderRestore () noexcept;
  ShaderProgramBinderRestore (const ShaderProgram &shaderProgram) noexcept;
  ShaderProgramBinderRestore (const ShaderProgramBinderRestore &) = delete;
  ShaderProgramBinderRestore (ShaderProgramBinderRestore &&) = delete;
  ~ShaderProgramBinderRestore () noexcept;

  ShaderProgramBinderRestore &operator= (const ShaderProgramBinderRestore &) = delete;
  ShaderProgramBinderRestore &operator= (ShaderProgramBinderRestore &&) = delete;

private:
  int mRestoreId;
};

class ShaderProgramBinderUnsafe : public ShaderProgramBinderInterface
{
public:
  ShaderProgramBinderUnsafe () noexcept;
  ShaderProgramBinderUnsafe (const ShaderProgram &shaderProgram) noexcept;
  ShaderProgramBinderUnsafe (const ShaderProgramBinderUnsafe &) = delete;
  ShaderProgramBinderUnsafe (ShaderProgramBinderUnsafe &&) = delete;
  ~ShaderProgramBinderUnsafe () noexcept;

  ShaderProgramBinderUnsafe &operator= (const ShaderProgramBinderUnsafe &) = delete;
  ShaderProgramBinderUnsafe &operator= (ShaderProgramBinderUnsafe &&) = delete;
};
}
