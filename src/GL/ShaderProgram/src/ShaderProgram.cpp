#include "GL/ShaderProgram.h"
#include "GL/Shader.h"
#include "stdio.h"   /* for printing error */
#include <GL/glew.h> /* for OpenGL */
#include <stdexcept>
#include <vector>

namespace GL
{
//-----------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram (int id) noexcept : mId (id) {}
ShaderProgram::ShaderProgram (ShaderProgram &&other) noexcept : mId (other.mId)
{
  other.mId = 0;
}
ShaderProgram::~ShaderProgram () noexcept
{
  if (mId)
    glDeleteProgram (mId);
}

ShaderProgram &ShaderProgram::operator= (ShaderProgram &&rhs) noexcept
{
  if (mId)
    glDeleteProgram (mId);

  mId = std::move (rhs).id ();
  return *this;
}

int ShaderProgram::id () const & noexcept
{
  return mId;
}

int ShaderProgram::id () && noexcept
{
  int idCopy = mId;
  mId = 0;
  return idCopy;
}

int ShaderProgram::uniformLocation (const char *name) const noexcept
{
  return glGetUniformLocation (mId, name);
}

int ShaderProgram::attributeLocation (const char *name) const noexcept
{
  return glGetAttribLocation (mId, name);
}

ShaderProgramBinder ShaderProgram::bind () const noexcept
{
  return ShaderProgramBinder (id ());
}

//-----------------------------------------------------------------------------------------

ShaderProgramLinker::ShaderProgramLinker () noexcept
{
}

ShaderProgramLinker::~ShaderProgramLinker () noexcept
{
}

namespace
{
std::optional<ShaderProgram> tryLink (int id, std::unique_ptr<char[]> &linkError) noexcept
{
  ShaderProgram shaderProgram (id);
  glLinkProgram (id);

  int success;
  glGetProgramiv (id, GL_LINK_STATUS, &success);
  if (!success)
    {
      constexpr int linkErrorBufSize = 512;
      if (!linkError)
        linkError = std::make_unique<char[]> (linkErrorBufSize);

      glGetProgramInfoLog (id, linkErrorBufSize, NULL, linkError.get ());
      return std::nullopt;
    }

  return shaderProgram;
}
}

std::optional<ShaderProgram> ShaderProgramLinker::link (std::initializer_list<int> shaderIds) noexcept
{
  int id = glCreateProgram ();
  for (int shaderId : shaderIds)
    glAttachShader (id, shaderId);

  return tryLink (id, mLinkError);
}

std::optional<ShaderProgram> ShaderProgramLinker::link (std::initializer_list<Shader> shaders) noexcept
{
  int id = glCreateProgram ();
  for (const Shader &shader : shaders)
    glAttachShader (id, shader.id ());

  return tryLink (id, mLinkError);
}

const char *ShaderProgramLinker::linkError () const noexcept
{
  return mLinkError.get ();
}

//-----------------------------------------------------------------------------------------

ShaderProgramBinder::ShaderProgramBinder(int id) noexcept : mId (id)
{
  glUseProgram (mId);
}

ShaderProgramBinder::ShaderProgramBinder(const ShaderProgram &shaderProgram) noexcept: ShaderProgramBinder (shaderProgram.id ())
{
}

ShaderProgramBinder::~ShaderProgramBinder() noexcept
{
  glUseProgram (0);
}

void ShaderProgramBinder::setUniform (int location, float x) noexcept
{
  glUniform1f (location, x);
}

void ShaderProgramBinder::setUniform (int location, float x, float y) noexcept
{
  glUniform2f (location, x, y);
}

void ShaderProgramBinder::setUniform (int location, float x, float y, float z) noexcept
{
  glUniform3f (location, x, y, z);
}

void ShaderProgramBinder::setUniform (int location, float x, float y, float z, float w) noexcept
{
  glUniform4f (location, x, y, z, w);
}

void ShaderProgramBinder::setUniform (int location, int x) noexcept
{
  glUniform1i (location, x);
}

void ShaderProgramBinder::setUniform (int location, int x, int y) noexcept
{
  glUniform2i (location, x, y);
}

void ShaderProgramBinder::setUniform (int location, int x, int y, int z) noexcept
{
  glUniform3i (location, x, y, z);
}

void ShaderProgramBinder::setUniform (int location, int x, int y, int z, int w) noexcept
{
  glUniform4i (location, x, y, z, w);
}

void ShaderProgramBinder::setUniform (int location, unsigned int x) noexcept
{
  glUniform1ui (location, x);
}

void ShaderProgramBinder::setUniform (int location, unsigned int x, unsigned int y) noexcept
{
  glUniform2ui (location, x, y);
}

void ShaderProgramBinder::setUniform (int location, unsigned int x, unsigned int y, unsigned int z) noexcept
{
  glUniform3ui (location, x, y, z);
}

void ShaderProgramBinder::setUniform (int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) noexcept
{
  glUniform4ui (location, x, y, z, w);
}

void ShaderProgramBinder::setUniformMatrix2 (int location, const float *m, bool transpose, int count) noexcept
{
  glUniformMatrix2fv (location, count, transpose, m);
}

void ShaderProgramBinder::setUniformMatrix3 (int location, const float *m, bool transpose, int count) noexcept
{
  glUniformMatrix3fv (location, count, transpose, m);
}

void ShaderProgramBinder::setUniformMatrix4 (int location, const float *m, bool transpose, int count) noexcept
{
  glUniformMatrix4fv (location, count, transpose, m);
}

//-----------------------------------------------------------------------------------------
}


