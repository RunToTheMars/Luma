#include "GL/ShaderProgram.h"
#include "GL/Shader.h"
#include "stdio.h"   /* for printing error */
#include <GL/glew.h> /* for OpenGL */
#include <stdexcept>
#include <vector>

namespace GL
{
//-----------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram (int id) : m_id (id) {}
ShaderProgram::~ShaderProgram () = default;

ShaderProgramBinder ShaderProgram::bind () { return ShaderProgramBinder (id ()); }

int ShaderProgram::uniform_location (const char *name) const { return glGetUniformLocation (m_id, name); }
int ShaderProgram::attribute_location (const char *name) const { return glGetAttribLocation (m_id, name); }

//-----------------------------------------------------------------------------------------

class ShaderProgramLinkerImpl
{
public:
  void add (const Shader &shader);
  std::optional<ShaderProgram> link ();

  const char *linkError () const { return m_linkError.get (); }

private:
  std::vector<int> m_shaders;
  std::unique_ptr<char[]> m_linkError;
};

void ShaderProgramLinkerImpl::add (const Shader &shader) { m_shaders.emplace_back (shader.id ()); }
std::optional<ShaderProgram> ShaderProgramLinkerImpl::link ()
{
  int id = glCreateProgram ();
  for (int shader_id : m_shaders)
    glAttachShader (id, shader_id);

  m_shaders.clear ();

  glLinkProgram (id);

  int success;
  glGetProgramiv (id, GL_LINK_STATUS, &success);
  if (!success)
    {
      constexpr int linkErrorBufSize = 512;
      if (!m_linkError)
        m_linkError = std::make_unique<char[]> (linkErrorBufSize);

      glGetProgramInfoLog (id, linkErrorBufSize, NULL, m_linkError.get ());
      glDeleteProgram (id);
      return std::nullopt;
    }

  return GL::ShaderProgram (id);
}

//-----------------------------------------------------------------------------------------

ShaderProgramLinker::ShaderProgramLinker () : m_pimpl (new ShaderProgramLinkerImpl) {}
ShaderProgramLinker::~ShaderProgramLinker () { delete m_pimpl; }

void ShaderProgramLinker::add (const Shader &shader) { m_pimpl->add (shader); }
std::optional<ShaderProgram> ShaderProgramLinker::link () { return m_pimpl->link (); }
const char *ShaderProgramLinker::linkError () const { return m_pimpl->linkError (); }

//-----------------------------------------------------------------------------------------

ShaderProgramBinder::ShaderProgramBinder(int id): m_id (id)
{
  glUseProgram (id);
}

ShaderProgramBinder::~ShaderProgramBinder()
{
  glUseProgram (0);
}

static int uniform_location_unsafe (int id, const char *name)
{
  int location = glGetUniformLocation (id, name);
  if (location <= -1)
    throw std::runtime_error ("Invalid uniform!");

  return location;
}

void ShaderProgramBinder::setUniform (int location, float x, float y) { glUniform2f (location, x, y); }
void ShaderProgramBinder::setUniform (const char *name, float x, float y) { glUniform2f (uniform_location_unsafe (m_id, name), x, y); }

void ShaderProgramBinder::setUniform (int location, float x, float y, float z) { glUniform3f (location, x, y, z); }
void ShaderProgramBinder::setUniform (const char *name, float x, float y, float z)
{
  glUniform3f (uniform_location_unsafe (m_id, name), x, y, z);
}

void ShaderProgramBinder::setUniform (int location, float x, float y, float z, float w) { glUniform4f (location, x, y, z, w); }
void ShaderProgramBinder::setUniform (const char *name, float x, float y, float z, float w)
{
  glUniform4f (uniform_location_unsafe (m_id, name), x, y, z, w);
}

void ShaderProgramBinder::setUniform (int location, int x) { glUniform1i (location, x); }
void ShaderProgramBinder::setUniform (const char *name, int x) { glUniform1i (uniform_location_unsafe (m_id, name), x); }

void ShaderProgramBinder::setUniform (int location, unsigned int x) { glUniform1ui (location, x); }
void ShaderProgramBinder::setUniform (const char *name, unsigned int x) { glUniform1ui (uniform_location_unsafe (m_id, name), x); }

void ShaderProgramBinder::setUniformMatrix4 (int location, const float *m)
{
  glUniformMatrix4fv (location, 1 /*count*/, true /*transpose*/, m);
}
void ShaderProgramBinder::setUniformMatrix4 (const char *name, const float *m)
{
  glUniformMatrix4fv (uniform_location_unsafe (m_id, name), 1 /*count*/, true /*transpose*/, m);
}

//-----------------------------------------------------------------------------------------
}


