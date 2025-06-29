#pragma once

#include <optional>
#include <memory>

namespace GL
{

class ShaderCompiler;
class ShaderProgramLinker;

class Shader
{
public:
  enum class Type
  {
    Vertex,
    Geometry,
    Fragment,
  };

  Shader (const Shader &) = delete;
  Shader (Shader &&) noexcept;
  ~Shader () noexcept;

  Shader &operator= (const Shader &) = delete;
  Shader &operator= (Shader &&) noexcept;

  Type type () const noexcept;

private:
  friend class GL::ShaderCompiler;
  friend class GL::ShaderProgramLinker;

  Shader (Type type, int id) noexcept;

  Type mType;
  int mId;
};

class ShaderCompiler
{
public:
  ShaderCompiler () noexcept;
  ShaderCompiler (const ShaderCompiler &) = delete;
  ShaderCompiler (ShaderCompiler &&) noexcept;
  ~ShaderCompiler () noexcept;

  ShaderCompiler &operator= (const ShaderCompiler &) = delete;
  ShaderCompiler &operator= (ShaderCompiler &&) noexcept;

  std::optional<Shader> compileSource (const char *source, Shader::Type type) noexcept;

  const std::unique_ptr<char[]> &compileError () const noexcept;
  std::unique_ptr<char[]> &&takeCompileError () noexcept;

private:
  std::unique_ptr<char[]> mCompileError;
};
}  // namespace GL
