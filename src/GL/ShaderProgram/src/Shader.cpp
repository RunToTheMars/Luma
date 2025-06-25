#include "GL/Shader.h"
#include <GL/glew.h> /* for OpenGL */
#include "stdio.h" /* for printing error */
#include <stdexcept>

namespace
{
int toGLEnum (GL::ShaderType type)
{
  switch (type)
    {
      case GL::ShaderType::Vertex  : return GL_VERTEX_SHADER;
      case GL::ShaderType::Geometry: return GL_GEOMETRY_SHADER;
      case GL::ShaderType::Fragment: return GL_FRAGMENT_SHADER;
    }

  throw std::runtime_error ("Invalid shader type");
  return GL_VERTEX_SHADER;
}
}

namespace GL
{
//-----------------------------------------------------------------------------------------
Shader::Shader (ShaderType _type, int _id) noexcept : mType (_type), mId (_id)
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
  mId = std::move (rhs).id ();

  return *this;
}


int Shader::id () const & noexcept
{
  return mId;
}

int Shader::id () && noexcept
{
  int idCopy = mId;
  mId = 0;
  return idCopy;
}

ShaderType Shader::type () const noexcept
{
  return mType;
}

//-----------------------------------------------------------------------------------------

ShaderCompiler::ShaderCompiler () noexcept = default;
ShaderCompiler::~ShaderCompiler () noexcept = default;

std::optional<Shader> ShaderCompiler::compileCode (const char *code, ShaderType type) noexcept
{
  int id = glCreateShader (toGLEnum (type));
  glShaderSource (id, 1, &code, NULL);
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

const char *ShaderCompiler::compileError () const noexcept
{
  return mCompileError.get ();
}

//-----------------------------------------------------------------------------------------
}  // namespace GL
