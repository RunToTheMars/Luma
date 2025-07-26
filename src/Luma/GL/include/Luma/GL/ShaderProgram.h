#pragma once

namespace Luma::GL
{

class Shader
{
public:
  enum class Type
  {
    Vertex   = 0x8B31, // GL_VERTEX_SHADER
    Geometry = 0x8DD9, // GL_GEOMETRY_SHADER
    Fragment = 0x8B30, // GL_FRAGMENT_SHADER
  };

  Shader (Type type) noexcept;
  Shader (const Shader &) = delete;
  Shader (Shader &&) noexcept;
  ~Shader () noexcept;

  Shader &operator= (const Shader &) = delete;
  Shader &operator= (Shader &&) noexcept;

  Type type () const noexcept;

  void create () noexcept;
  bool isCreated () const noexcept;
  void destroy () noexcept;

  /// -------------------------------
  /// \note Only for Created
  void compileSource (const char *source) noexcept;

  int id () const noexcept;
  /// -------------------------------

  /// -------------------------------
  /// \note Only for Compiled
  bool hasCompileError () const noexcept;
  int compileErrorSize () const noexcept;
  void compileError (char *error, int size) const noexcept;

  int sourceSize () const noexcept;
  void source (char *source, int size) const noexcept;
  /// -------------------------------

private:
  Type mType;
  int mId = 0;
};

class ShaderProgram
{
public:
  ShaderProgram () noexcept;
  ShaderProgram (const ShaderProgram &) = delete;
  ShaderProgram (ShaderProgram &&) noexcept;
  ~ShaderProgram () noexcept;

  ShaderProgram &operator= (const ShaderProgram &) = delete;
  ShaderProgram &operator= (ShaderProgram &&) noexcept;

  void create () noexcept;
  bool isCreated () const noexcept;
  void destroy () noexcept;

  /// -------------------------------
  /// \note Only for Created
  void attach (int shaderId) noexcept;
  void detach (int shaderId) noexcept;

  void link () noexcept;

  bool hasLinkError () const noexcept;
  int linkErrorSize () const noexcept;
  void linkError (char *error, int size) const noexcept;

  int uniformLocation (const char *name) const noexcept;
  int attributeLocation (const char *name) const noexcept;

  void bind () noexcept;
  void unbind () noexcept;
  /// -------------------------------

  /// -------------------------------
  /// \note Only for Binded
  void setUniform (int location, float x) const noexcept;
  void setUniform (int location, float x, float y) const noexcept;
  void setUniform (int location, float x, float y, float z) const noexcept;
  void setUniform (int location, float x, float y, float z, float w) const noexcept;

  void setUniform (int location, int x) const noexcept;
  void setUniform (int location, int x, int y) const noexcept;
  void setUniform (int location, int x, int y, int z) const noexcept;
  void setUniform (int location, int x, int y, int z, int w) const noexcept;

  void setUniform (int location, unsigned int x) const noexcept;
  void setUniform (int location, unsigned int x, unsigned int y) const noexcept;
  void setUniform (int location, unsigned int x, unsigned int y, unsigned int z) const noexcept;
  void setUniform (int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const noexcept;

  void setUniformMatrix2 (int location, const float *m, bool transpose = true, int count = 1) const noexcept;
  void setUniformMatrix3 (int location, const float *m, bool transpose = true, int count = 1) const noexcept;
  void setUniformMatrix4 (int location, const float *m, bool transpose = true, int count = 1) const noexcept;
  /// -------------------------------

private:
  int mId = 0;
};
}
