#include "GL/ShaderProgram.h"
#include "GL/Shader.h"
#include "stdio.h"   /* for printing error */
#include <GL/glew.h> /* for OpenGL */

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

  mId = rhs.mId;
  rhs.mId = 0;
  return *this;
}

int ShaderProgram::uniformLocation (const char *name) const noexcept
{
  return glGetUniformLocation (mId, name);
}

int ShaderProgram::attributeLocation (const char *name) const noexcept
{
  return glGetAttribLocation (mId, name);
}

//-----------------------------------------------------------------------------------------

ShaderProgramLinker::ShaderProgramLinker () noexcept
{
}

ShaderProgramLinker::~ShaderProgramLinker () noexcept
{
}

std::optional<ShaderProgram> ShaderProgramLinker::link (std::initializer_list<const Shader *> shaders) noexcept
{
  ShaderProgram shaderProgram (glCreateProgram ());

  for (const Shader *shader : shaders)
    glAttachShader (shaderProgram.mId, shader->mId);

  glLinkProgram (shaderProgram.mId);

  int success;
  glGetProgramiv (shaderProgram.mId, GL_LINK_STATUS, &success);
  if (!success)
    {
      constexpr int linkErrorBufSize = 512;
      if (!mLinkError)
        mLinkError = std::make_unique<char[]> (linkErrorBufSize);

      glGetProgramInfoLog (shaderProgram.mId, linkErrorBufSize, NULL, mLinkError.get ());
      return std::nullopt;
    }

  return shaderProgram;
}

const std::unique_ptr<char[]> &ShaderProgramLinker::linkError () const noexcept
{
  return mLinkError;
}

std::unique_ptr<char[]> &&ShaderProgramLinker::takeLinkError () noexcept
{
  return std::move (mLinkError);
}

//-----------------------------------------------------------------------------------------

ShaderProgramBinderInterface::ShaderProgramBinderInterface () noexcept
{
}

ShaderProgramBinderInterface::~ShaderProgramBinderInterface () noexcept
{
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::bind (const ShaderProgram &shaderProgram) const noexcept
{
    glUseProgram (shaderProgram.mId);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, float x) const noexcept
{
    glUniform1f (location, x);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, float x, float y) const noexcept
{
    glUniform2f (location, x, y);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, float x, float y, float z) const noexcept
{
    glUniform3f (location, x, y, z);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, float x, float y, float z, float w) const noexcept
{
    glUniform4f (location, x, y, z, w);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform(int location, int x) const noexcept
{
    glUniform1i (location, x);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, int x, int y) const noexcept
{
    glUniform2i (location, x, y);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, int x, int y, int z) const noexcept
{
    glUniform3i (location, x, y, z);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, int x, int y, int z, int w) const noexcept
{
    glUniform4i (location, x, y, z, w);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, unsigned int x) const noexcept
{
    glUniform1ui (location, x);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, unsigned int x, unsigned int y) const noexcept
{
    glUniform2ui (location, x, y);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, unsigned int x, unsigned int y, unsigned int z) const noexcept
{
    glUniform3ui (location, x, y, z);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniform (int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const noexcept
{
    glUniform4ui (location, x, y, z, w);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniformMatrix2 (int location, const float *m, bool transpose, int count) const noexcept
{
    glUniformMatrix2fv (location, count, transpose, m);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniformMatrix3 (int location, const float *m, bool transpose, int count) const noexcept
{
    glUniformMatrix3fv (location, count, transpose, m);
    return *this;
}

const ShaderProgramBinderInterface &ShaderProgramBinderInterface::setUniformMatrix4 (int location, const float *m, bool transpose, int count) const noexcept
{
    glUniformMatrix4fv (location, count, transpose, m);
    return *this;
}

//-----------------------------------------------------------------------------------------

ShaderProgramBinderRestore::ShaderProgramBinderRestore () noexcept
{
  glGetIntegerv (GL_CURRENT_PROGRAM, &mRestoreId);
}

ShaderProgramBinderRestore::ShaderProgramBinderRestore (const ShaderProgram &shaderProgram) noexcept: ShaderProgramBinderRestore ()
{
  bind (shaderProgram);
}

ShaderProgramBinderRestore::~ShaderProgramBinderRestore () noexcept
{
  glUseProgram (mRestoreId);
}

//-----------------------------------------------------------------------------------------

ShaderProgramBinderUnsafe::ShaderProgramBinderUnsafe () noexcept
{
}

ShaderProgramBinderUnsafe::ShaderProgramBinderUnsafe (const ShaderProgram &shaderProgram) noexcept
{
  bind (shaderProgram);
}

//-----------------------------------------------------------------------------------------
}


