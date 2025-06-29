#include "GL/Shader.h"
#include <GL/glew.h> /* for OpenGL */
#include "stdio.h" /* for printing error */
#include <stdexcept>

namespace
{
int toGLEnum (GL::Shader::Type type)
{
  switch (type)
    {
      case GL::Shader::Type::Vertex  : return GL_VERTEX_SHADER;
      case GL::Shader::Type::Geometry: return GL_GEOMETRY_SHADER;
      case GL::Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
    }

  throw std::runtime_error ("Invalid shader type");
  return GL_VERTEX_SHADER;
}
}

namespace GL
{
//-----------------------------------------------------------------------------------------
Shader::Shader (Shader::Type type, int _id) noexcept : mType (type), mId (_id)
{
}

Shader::Shader (Shader &&shader) noexcept : mType (shader.mType), mId (shader.mId)
{
  shader.mId = 0;
}

Shader::~Shader () noexcept
{
    if (mId)
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

//-----------------------------------------------------------------------------------------

ShaderCompiler::ShaderCompiler () noexcept = default;
ShaderCompiler::~ShaderCompiler () noexcept = default;

std::optional<Shader> ShaderCompiler::compileSource (const char *source, Shader::Type type) noexcept
{
  int id = glCreateShader (toGLEnum (type));
  glShaderSource (id, 1, &source, NULL);
  glCompileShader (id);

  int success;
  glGetShaderiv (id, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      constexpr int compileErrorBufSize = 512;
      if (!mCompileError)
        mCompileError = std::make_unique<char[]> (compileErrorBufSize);

      glGetShaderInfoLog (id, compileErrorBufSize, NULL, mCompileError.get ());
      glDeleteShader (id);
      return std::nullopt;
    }

  return GL::Shader (type, id);
}

const std::unique_ptr<char[]> &ShaderCompiler::compileError () const noexcept
{
  return mCompileError;
}

std::unique_ptr<char[]> &&ShaderCompiler::takeCompileError () noexcept
{
  return std::move (mCompileError);
}


//-----------------------------------------------------------------------------------------
}  // namespace GL
