#include <GL/glew.h>
#include <math.h>
#include <memory>
#include <stdio.h>

namespace
{

enum class ShaderType
{
  VertexShader   = GL_VERTEX_SHADER,
  GeometryShader = GL_GEOMETRY_SHADER,
  FragmentShader = GL_FRAGMENT_SHADER,
};

const char *toString (ShaderType type)
{
  switch (type)
    {
      case ShaderType::VertexShader  : return "Vertex Shader";
      case ShaderType::GeometryShader: return "Geometry Shader";
      case ShaderType::FragmentShader: return "Fragment Shader";
    }

  return "Unknown Shader";
}

int createShader (const char *source, ShaderType type, const char *programName)
{
  int shaderID = glCreateShader (static_cast<int> (type));
  glShaderSource (shaderID, 1, &source, NULL);
  glCompileShader (shaderID);

  int success;
  glGetShaderiv (shaderID, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      int size;
      glGetShaderiv (shaderID, GL_INFO_LOG_LENGTH, &size);
      std::unique_ptr<char[]> error = std::make_unique<char[]> (size);
      glGetShaderInfoLog (shaderID, size, nullptr, error.get ());

      if (programName)
        fprintf (stderr, "%s of \"%s\" compilation problem:\n%s\n", toString (type), programName, error.get ());
      else
        fprintf (stderr, "%s compilation problem:\n%s\n", toString (type), error.get ());
    }
  return shaderID;
}
}

namespace Shader::Utils
{
bool almostEqual (float a, float b, float eps)
{
  return std::fabs (a - b) < eps;
}

int createProgram (const char *vertexShader, const char *geometryShader, const char *fragmentShader, const char *programName)
{
  int programID = glCreateProgram ();

  int vertexShaderID = -1;
  if (vertexShader)
    {
      vertexShaderID = createShader (vertexShader, ShaderType::VertexShader, programName);
      glAttachShader (programID, vertexShaderID);
    }

  int geometryShaderID = -1;
  if (geometryShader)
    {
      geometryShaderID = createShader (geometryShader, ShaderType::GeometryShader, programName);
      glAttachShader (programID, geometryShaderID);
    }

  int fragmentShaderID = -1;
  if (fragmentShader)
    {
      fragmentShaderID = createShader (fragmentShader, ShaderType::FragmentShader, programName);
      glAttachShader (programID, fragmentShaderID);
    }

  glLinkProgram (programID);
  int success;
  glGetProgramiv (programID, GL_LINK_STATUS, &success);
  if (!success)
    {
      int size;
      glGetProgramiv (programID, GL_INFO_LOG_LENGTH, &size);
      std::unique_ptr<char[]> error = std::make_unique<char[]> (size);
      glGetProgramInfoLog (programID, size, nullptr, error.get ());

      if (programName)
        fprintf (stderr, "Program \"%s\" link problem:\n%s\n", programName, error.get ());
      else
        fprintf (stderr, "Program link problem:\n%s\n", error.get ());
    }

  if (vertexShader)
    glDeleteShader (vertexShaderID);

  if (geometryShader)
    glDeleteShader (geometryShaderID);

  if (fragmentShader)
    glDeleteShader (fragmentShaderID);

  return programID;
}
}
