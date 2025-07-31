#include "Shader/Triangle/v150/TriangleShader.h"
#include "Shader/Utils.h"
#include <GL/glew.h>
#include <cstring>
#include <math.h>
#include <stddef.h>

namespace
{
bool almostEqual (float a, float b, float eps = 1e-6)
{
  return std::fabs (a - b) < eps;
}
}

namespace Shader::Triangle::v150
{
TriangleShader::TriangleShader () noexcept
{
}

TriangleShader::~TriangleShader () noexcept
{
  destroy ();
}

void TriangleShader::create () noexcept
{
  const char *vertexShaderSource = R"(#version 150

                                      in vec4 aPosition;
                                      in vec4 aColor;

                                      smooth out vec4 fColor;

                                      uniform mat4 uModelViewProjectionMatrix;

                                      void main ()
                                      {
                                        gl_Position = uModelViewProjectionMatrix * aPosition;
                                        fColor = aColor;
                                      }
                                    )";

  const char *fragmentShaderSource = R"(#version 130
                                     smooth in vec4 fColor;

                                     void main()
                                     {
                                         fragColor = fColor;
                                     }
                                    )";

  int vertexShaderId = glCreateShader (GL_VERTEX_SHADER);
  int fragmentShaderId = glCreateShader (GL_FRAGMENT_SHADER);

  glShaderSource (vertexShaderId, 1, &vertexShaderSource, NULL);
  glCompileShader (vertexShaderId);

  glShaderSource (fragmentShaderId, 1, &fragmentShaderSource, NULL);
  glCompileShader (fragmentShaderId);

  mProgramId = glCreateProgram ();

  glAttachShader (mProgramId, vertexShaderId);
  glAttachShader (mProgramId, fragmentShaderId);

  glLinkProgram (mProgramId);

  glDeleteShader (vertexShaderId);
  glDeleteShader (fragmentShaderId);

  mUniformModelViewProjectionMatrixLoc = glGetUniformLocation (mProgramId, "uModelViewProjectionMatrix");

  mPositionAttributeLoc = glGetAttribLocation (mProgramId, "aPosition");
  mColorAttributeLoc    = glGetAttribLocation (mProgramId, "aColor");

  if (isCreated ())
    {
      bind ();
      glUniformMatrix4fv (mUniformModelViewProjectionMatrixLoc, 1, GL_FALSE, mModelViewProjectionMatrix);
      unbind ();
    }
}

bool TriangleShader::isCreated () const noexcept
{
  return mProgramId;
}

void TriangleShader::destroy () noexcept
{
  if (isCreated ())
    {
      glDeleteProgram (mProgramId);
      mProgramId = 0;
    }
}

const float *TriangleShader::modelViewProjectionMatrix () const noexcept
{
  return mModelViewProjectionMatrix;
}

int TriangleShader::positionAttributeLocation () const
{
  return mPositionAttributeLoc;
}

int TriangleShader::colorAttributeLocation () const
{
  return mColorAttributeLoc;
}

void TriangleShader::bind ()
{
  glUseProgram (mProgramId);
}

void TriangleShader::unbind ()
{
  glUseProgram (0);
}

void TriangleShader::setModelViewProjectionMatrix (const float *matrix) noexcept
{

  if (   Utils::almostEqual (matrix[0 ], mModelViewProjectionMatrix[0 ])
      && Utils::almostEqual (matrix[1 ], mModelViewProjectionMatrix[1 ])
      && Utils::almostEqual (matrix[2 ], mModelViewProjectionMatrix[2 ])
      && Utils::almostEqual (matrix[3 ], mModelViewProjectionMatrix[3 ])
      && Utils::almostEqual (matrix[4 ], mModelViewProjectionMatrix[4 ])
      && Utils::almostEqual (matrix[5 ], mModelViewProjectionMatrix[5 ])
      && Utils::almostEqual (matrix[6 ], mModelViewProjectionMatrix[6 ])
      && Utils::almostEqual (matrix[7 ], mModelViewProjectionMatrix[7 ])
      && Utils::almostEqual (matrix[8 ], mModelViewProjectionMatrix[8 ])
      && Utils::almostEqual (matrix[9 ], mModelViewProjectionMatrix[9 ])
      && Utils::almostEqual (matrix[10], mModelViewProjectionMatrix[10])
      && Utils::almostEqual (matrix[11], mModelViewProjectionMatrix[11])
      && Utils::almostEqual (matrix[12], mModelViewProjectionMatrix[12])
      && Utils::almostEqual (matrix[13], mModelViewProjectionMatrix[13])
      && Utils::almostEqual (matrix[14], mModelViewProjectionMatrix[14])
      && Utils::almostEqual (matrix[15], mModelViewProjectionMatrix[15]))
    return;

  memcpy (mModelViewProjectionMatrix, matrix, 16 * sizeof (float));

  glUniformMatrix4fv (mUniformModelViewProjectionMatrixLoc, 1, GL_FALSE, mModelViewProjectionMatrix);
}
}
