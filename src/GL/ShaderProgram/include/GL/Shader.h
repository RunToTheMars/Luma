#pragma once

#include <optional>
#include <memory>

namespace GL
{

enum class ShaderType
{
  Vertex,
  Geometry,
  Fragment,
};

class Shader
{
public:
  Shader (ShaderType type, int id) noexcept;
  Shader (const Shader &) = delete;
  Shader (Shader &&) noexcept;
  ~Shader () noexcept;

  Shader &operator= (const Shader &) = delete;
  Shader &operator= (Shader &&) noexcept;

  int id () const & noexcept;
  int id () && noexcept;

  ShaderType type () const noexcept;

private:
  ShaderType mType;
  int mId = 0;
};

class ShaderCompilerImpl;
class ShaderCompiler
{
public:
  ShaderCompiler () noexcept;
  ShaderCompiler (const ShaderCompiler &) = delete;
  ShaderCompiler (ShaderCompiler &&) noexcept;
  ~ShaderCompiler () noexcept;

  ShaderCompiler &operator= (const ShaderCompiler &) = delete;
  ShaderCompiler &operator= (ShaderCompiler &&) noexcept;

  std::optional<Shader> compileCode (const char *code, ShaderType type) noexcept;
  const char *compileError () const noexcept;

private:
  std::unique_ptr<char[]> mCompileError;
};
}  // namespace GL
