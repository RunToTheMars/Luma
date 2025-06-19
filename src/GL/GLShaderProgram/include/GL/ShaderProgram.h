#pragma once

#include <optional>

namespace GL
{
class Shader;
class ShaderProgramBinder;

class ShaderProgram
{
public:
  ~ShaderProgram ();

  int id () const { return m_id; }

  ShaderProgramBinder bind ();

  int uniform_location (const char *name) const;
  int attribute_location (const char *name) const;

private:
  friend class ShaderProgramLinkerImpl;

  ShaderProgram (int id);
  int m_id;
};

class ShaderProgramLinkerImpl;
class ShaderProgramLinker
{
public:
  ShaderProgramLinker ();
  ~ShaderProgramLinker ();

  void add (const Shader &shader);
  std::optional<ShaderProgram> link ();

  const char *linkError () const;

private:
  ShaderProgramLinkerImpl *m_pimpl = nullptr;
};

class ShaderProgramBinder
{
public:
  ~ShaderProgramBinder ();

  ShaderProgramBinder (const ShaderProgramBinder &) = delete;
  ShaderProgramBinder &operator= (const ShaderProgramBinder &) = delete;

  ShaderProgramBinder (ShaderProgramBinder &&) = delete;
  ShaderProgramBinder &operator= (ShaderProgramBinder &&) = delete;

  int uniform_location (const char *name) const;

  void setUniform (int location, float x, float y);
  void setUniform (const char *name, float x, float y);

  void setUniform (int location, float x, float y, float z);
  void setUniform (const char *name, float x, float y, float z);

  void setUniform (int location, float x, float y, float z, float w);
  void setUniform (const char *name, float x, float y, float z, float w);

  void setUniform (int location, int x);
  void setUniform (const char *name, int x);

  void setUniform (int location, unsigned int x);
  void setUniform (const char *name, unsigned int x);

  void setUniformMatrix4(int location, const float *m);
  void setUniformMatrix4 (const char *name, const float *m);

  int attribute_location (const char *name) const;

private:
  friend class ShaderProgram;
  ShaderProgramBinder (int id);

  int m_id;
};
}
