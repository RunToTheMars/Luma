#include "Shader/Basic/v110/BasicShader.h"
#include "Shader/Utils.h"
#include <GL/glew.h>
#include <cstring>
#include <math.h>
#include <stddef.h>

namespace Shader::Basic::v110
{
BasicShader::BasicShader () noexcept
{
}

BasicShader::~BasicShader () noexcept
{
  destroy ();
}

void BasicShader::create () noexcept
{
  const char *vertexShaderSource = R"(#version 110

                                      attribute vec4 aPosition;
                                      attribute vec4 aColor;

                                      varying vec4 fColor;

                                      uniform mat4 uModelViewProjectionMatrix;

                                      void main ()
                                      {
                                        gl_Position = uModelViewProjectionMatrix * aPosition;
                                        fColor = aColor;
                                      }
                                    )";

  const char *fragmentShaderSource = R"(#version 110

                                     varying vec4 fColor;

                                     void main()
                                     {
                                         gl_FragColor = fColor;
                                     }
                                    )";

  mProgramId = Shader::Utils::createProgram (vertexShaderSource, nullptr, fragmentShaderSource, "Geometry");

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

bool BasicShader::isCreated () const noexcept
{
  return mProgramId;
}

void BasicShader::destroy () noexcept
{
  if (isCreated ())
    {
      glDeleteProgram (mProgramId);
      mProgramId = 0;
    }
}

const float *BasicShader::modelViewProjectionMatrix () const noexcept
{
  return mModelViewProjectionMatrix;
}

int BasicShader::positionAttributeLocation () const
{
  return mPositionAttributeLoc;
}

int BasicShader::colorAttributeLocation () const
{
  return mColorAttributeLoc;
}

void BasicShader::bind ()
{
  glUseProgram (mProgramId);
}

void BasicShader::unbind ()
{
  glUseProgram (0);
}

void BasicShader::setModelViewProjectionMatrix (const float *matrix) noexcept
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
