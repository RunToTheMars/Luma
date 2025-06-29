#include "GL/ShaderProgram.h"
#include <GL/glew.h> /* for OpenGL */

namespace GL
{

Shader::Shader (Shader::Type type) noexcept : mType (type)
{
}

Shader::Shader (Shader &&shader) noexcept : mType (shader.mType), mId (shader.mId)
{
  shader.mId = 0;
}

Shader::~Shader () noexcept
{
    if (isCreated ())
        glDeleteShader (mId);
}

Shader &Shader::operator= (Shader &&rhs) noexcept
{
  if (mId)
    glDeleteShader (mId);

  mType = rhs.mType;
  mId = rhs.mId;
  rhs.mId = 0;

  return *this;
}

Shader::Type Shader::type () const noexcept
{
  return mType;
}

void Shader::create () noexcept
{
  if (!isCreated ())
    mId = glCreateShader (static_cast<unsigned int> (mType));
}

bool Shader::isCreated () const noexcept
{
    return mId;
}

void Shader::destroy () noexcept
{
  if (isCreated ())
    {
      glDeleteShader (mId);
      mId = 0;
    }
}

void Shader::compileSource (const char *source) noexcept
{
  glShaderSource (mId, 1, &source, nullptr);
  glCompileShader (mId);
}

int Shader::id () const noexcept
{
  return mId;
}

bool Shader::hasCompileError () const noexcept
{
  int success;
  glGetShaderiv (mId, GL_COMPILE_STATUS, &success);
  return !success;
}

int Shader::compileErrorSize () const noexcept
{
  int size;
  glGetShaderiv (mId, GL_INFO_LOG_LENGTH, &size);
  return size;
}

void Shader::compileError (char *error, int size) const noexcept
{
  glGetShaderInfoLog (mId, size, nullptr, error);
}

int Shader::sourceSize () const noexcept
{
  int size;
  glGetShaderiv (mId, GL_SHADER_SOURCE_LENGTH, &size);
  return size;
}

void Shader::source (char *source, int size) const noexcept
{
  glGetShaderSource (mId, size, nullptr, source);
}

//-----------------------------------------------------------------------------------------

ShaderProgram::ShaderProgram () noexcept
{
}

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

void ShaderProgram::create () noexcept
{
  if (!isCreated ())
    mId = glCreateProgram ();
}

bool ShaderProgram::isCreated () const noexcept
{
  return mId;
}

void ShaderProgram::destroy () noexcept
{
  if (isCreated ())
    {
      glDeleteProgram (mId);
      mId = 0;
    }
}

void ShaderProgram::attach (int shaderId) noexcept
{
  glAttachShader (mId, shaderId);
}

void ShaderProgram::detach (int shaderId) noexcept
{
  glDetachShader (mId, shaderId);
}

void ShaderProgram::link () noexcept
{
  glLinkProgram (mId);
}

bool ShaderProgram::hasLinkError () const noexcept
{
  int success;
  glGetProgramiv (mId, GL_LINK_STATUS, &success);
  return !success;
}

int ShaderProgram::linkErrorSize () const noexcept
{
  int size;
  glGetProgramiv (mId, GL_INFO_LOG_LENGTH, &size);
  return size;
}

void ShaderProgram::linkError (char *error, int size) const noexcept
{
  glGetProgramInfoLog (mId, size, nullptr, error);
}

int ShaderProgram::uniformLocation (const char *name) const noexcept
{
  return glGetUniformLocation (mId, name);
}

int ShaderProgram::attributeLocation (const char *name) const noexcept
{
  return glGetAttribLocation (mId, name);
}

void ShaderProgram::bind () noexcept
{
  glUseProgram (mId);
}

void ShaderProgram::unbind () noexcept
{
  glUseProgram (0);
}

void ShaderProgram::setUniform (int location, float x) const noexcept
{
  glUniform1f (location, x);
}

void ShaderProgram::setUniform (int location, float x, float y) const noexcept
{
  glUniform2f (location, x, y);
}

void ShaderProgram::setUniform (int location, float x, float y, float z) const noexcept
{
  glUniform3f (location, x, y, z);
}

void ShaderProgram::setUniform (int location, float x, float y, float z, float w) const noexcept
{
  glUniform4f (location, x, y, z, w);
}

void ShaderProgram::setUniform (int location, int x) const noexcept
{
  glUniform1i (location, x);
}

void ShaderProgram::setUniform (int location, int x, int y) const noexcept
{
  glUniform2i (location, x, y);
}

void ShaderProgram::setUniform (int location, int x, int y, int z) const noexcept
{
  glUniform3i (location, x, y, z);
}

void ShaderProgram::setUniform (int location, int x, int y, int z, int w) const noexcept
{
  glUniform4i (location, x, y, z, w);
}

void ShaderProgram::setUniform (int location, unsigned int x) const noexcept
{
  glUniform1ui (location, x);
}

void ShaderProgram::setUniform (int location, unsigned int x, unsigned int y) const noexcept
{
  glUniform2ui (location, x, y);
}

void ShaderProgram::setUniform (int location, unsigned int x, unsigned int y, unsigned int z) const noexcept
{
  glUniform3ui (location, x, y, z);
}

void ShaderProgram::setUniform (int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const noexcept
{
  glUniform4ui (location, x, y, z, w);
}

void ShaderProgram::setUniformMatrix2 (int location, const float *m, bool transpose, int count) const noexcept
{
  glUniformMatrix2fv (location, count, transpose, m);
}

void ShaderProgram::setUniformMatrix3 (int location, const float *m, bool transpose, int count) const noexcept
{
  glUniformMatrix3fv (location, count, transpose, m);
}

void ShaderProgram::setUniformMatrix4 (int location, const float *m, bool transpose, int count) const noexcept
{
  glUniformMatrix4fv (location, count, transpose, m);
}
}


