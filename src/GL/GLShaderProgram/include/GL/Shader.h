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
  Shader (const Shader &) = delete;
  Shader (Shader &&);

  Shader &operator= (const Shader &) = delete;
  Shader &operator= (Shader &&);

  ~Shader ();

  int id () const { return m_id; }
  ShaderType type () const { return m_type; }

  bool isValid () const;

private:
  friend class ShaderCompilerImpl;
  Shader (ShaderType type, int id);

  ShaderType m_type;
  int m_id = 0;
};

class ShaderCompilerImpl;
class ShaderCompiler
{
public:
  ShaderCompiler ();
  ~ShaderCompiler ();

  std::optional<Shader> compileCode (const char *code, ShaderType type);
  std::optional<Shader> compileFile (const char *path, ShaderType type);

  const char *compileError () const;

private:
  std::unique_ptr<ShaderCompilerImpl> m_pimpl;
};
}  // namespace GL
