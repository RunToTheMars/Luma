#include "GL/Shader.h"
#include <GL/glew.h> /* for OpenGL */
#include "stdio.h" /* for printing error */
#include <string>  /* for reading shader program from disk */
#include <fstream> /* for reading shader program from disk */
#include <sstream> /* for reading shader program from disk */

namespace
{
int to_gl_enum (GL::ShaderType type)
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
Shader::Shader (ShaderType _type, int _id) : m_type (_type), m_id (_id) {}
Shader::Shader (Shader &&shader) : m_type (shader.m_type), m_id (shader.m_id) { shader.m_id = 0; }
Shader &Shader::operator= (Shader &&rhs)
{
  m_type = rhs.m_type;
  m_id = rhs.m_id;
  rhs.m_id = 0;
  return *this;
}
Shader::~Shader ()
{
  if (m_id)
    glDeleteShader (m_id);
}
//-----------------------------------------------------------------------------------------

class ShaderCompilerImpl
{
public:
  std::optional<Shader> compileCode (const char *code, ShaderType type);
  std::optional<Shader> compileFile (const char *path, ShaderType type);

  const char *compileError () const { return m_compileError.get (); }

private:
  std::unique_ptr<char[]> m_compileError;
};

std::optional<Shader> ShaderCompilerImpl::compileCode (const char* code, ShaderType type)
{
  int id = glCreateShader (to_gl_enum (type));
  glShaderSource (id, 1, &code, NULL);
  glCompileShader (id);

  int success;
  glGetShaderiv (id, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      constexpr int compileErrorBufSize = 512;
      if (!m_compileError)
        m_compileError = std::make_unique<char[]> (compileErrorBufSize);

      glGetShaderInfoLog (id, compileErrorBufSize, NULL, m_compileError.get ());
      glDeleteShader (id);
      return std::nullopt;
    }

  return GL::Shader (type, id);
}

std::optional<Shader> ShaderCompilerImpl::compileFile (const char* path, ShaderType type)
{
  std::ifstream ifstream;
  ifstream.open (path);

  if (!ifstream.good ())
    throw std::runtime_error ("\"" + std::string (path) + "\"" +  " is invalid path of shader!");

  std::stringstream sstream;
  sstream << ifstream.rdbuf ();
  std::string code = std::move (sstream).str ();
  return compileCode (code.c_str (), type);
}

//-----------------------------------------------------------------------------------------

ShaderCompiler::ShaderCompiler () : m_pimpl (std::make_unique<ShaderCompilerImpl> ()) {}
ShaderCompiler::~ShaderCompiler () = default;
std::optional<Shader> ShaderCompiler::compileCode (const char *code, ShaderType type) { return m_pimpl->compileCode (code, type); }
std::optional<Shader> ShaderCompiler::compileFile (const char *path, ShaderType type) { return m_pimpl->compileFile (path, type); }
const char *ShaderCompiler::compileError () const { return m_pimpl->compileError (); }

//-----------------------------------------------------------------------------------------
}  // namespace GL
