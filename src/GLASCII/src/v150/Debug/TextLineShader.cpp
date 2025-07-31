#include "GLASCII/v150/Debug/TextLineShader.h"
#include "Utils.inl"
#include <GL/glew.h>
#include <cstring>
#include <stddef.h>

namespace GLASCII
{
namespace v150
{
namespace Debug
{
TextLineShader::TextLineShader () noexcept
{
}

TextLineShader::~TextLineShader () noexcept
{
  destroy ();
}

void TextLineShader::create () noexcept
{
  const char *vertexShaderSource = R"(#version 130

                                      attribute uint aAsciiCode;

                                      uniform vec3 uPosition;
                                      uniform vec2 uSize;

                                      varying uint vAsciiCode;

                                      void main ()
                                      {
                                        vAsciiCode = aAsciiCode;
                                        gl_Position = vec4 (uPosition, 1);
                                        gl_Position.x += gl_VertexID * uSize.x;
                                      }
                                    )";

  const char *geometryShaderSource = R"(#version 150

                                     layout (points) in;
                                     layout (triangle_strip, max_vertices = 4) out;

                                     in uint vAsciiCode[];

                                     uniform vec2 uSize;
                                     uniform mat4 uModelViewProjectionMatrix;

                                     flat   out uint pAsciiCode;
                                     smooth out vec2 fGlyphUV;

                                     #define GLYPH_WIDTH 8.
                                     #define GLYPH_HEIGHT 16.

                                     void emitVertex (in vec2 glyphOffset, in vec2 glyphUV);

                                     void main()
                                     {
                                         pAsciiCode = vAsciiCode[0];

                                         emitVertex (vec2 (0., 0.), vec2 (0., GLYPH_HEIGHT));
                                         emitVertex (vec2 (0., uSize.y), vec2 (0., 0.));
                                         emitVertex (vec2 (uSize.x, 0.), vec2 (GLYPH_WIDTH, GLYPH_HEIGHT));
                                         emitVertex (uSize, vec2 (GLYPH_WIDTH, 0.));

                                         EndPrimitive ();
                                     }

                                     void emitVertex (in vec2 glyphOffset, in vec2 glyphUV)
                                     {
                                         gl_Position = uModelViewProjectionMatrix * vec4 (gl_in[0].gl_Position.xy + glyphOffset, gl_in[0].gl_Position.zw);
                                         fGlyphUV = glyphUV;
                                         EmitVertex ();
                                     }
                                    )";

  const char *fragmentShaderSource = R"(#version 130

                                     flat   in uint pAsciiCode;  // primitive attribute: ascii code (belongs to [0, 255])
                                     smooth in vec2 fGlyphUV;    // fragment attribute: UV texels coords

                                     uniform vec4 uColor;            // color of word
                                     uniform vec4 uBackgroundColor; // color of word background

                                     out vec4 fragColor;

                                     const uvec4 glyphs[96] = uvec4[96](
                                         uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00000000), uint (0x00000000) ), // 0x20: SPACE
                                         uvec4 ( uint (0x00001010), uint (0x10101010), uint (0x00001010), uint (0x00000000) ), // 0x21: '!'
                                         uvec4 ( uint (0x00242424), uint (0x24000000), uint (0x00000000), uint (0x00000000) ), // 0x22: '\'
                                         uvec4 ( uint (0x00000024), uint (0x247E2424), uint (0x247E2424), uint (0x00000000) ), // 0x23: '#'
                                         uvec4 ( uint (0x00000808), uint (0x1E20201C), uint (0x02023C08), uint (0x08000000) ), // ...
                                         uvec4 ( uint (0x00000030), uint (0x494A3408), uint (0x16294906), uint (0x00000000) ),
                                         uvec4 ( uint (0x00003048), uint (0x48483031), uint (0x49464639), uint (0x00000000) ),
                                         uvec4 ( uint (0x00101010), uint (0x10000000), uint (0x00000000), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000408), uint (0x08101010), uint (0x10101008), uint (0x08040000) ),
                                         uvec4 ( uint (0x00002010), uint (0x10080808), uint (0x08080810), uint (0x10200000) ),
                                         uvec4 ( uint (0x00000000), uint (0x0024187E), uint (0x18240000), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x0808087F), uint (0x08080800), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00001818), uint (0x08081000) ),
                                         uvec4 ( uint (0x00000000), uint (0x0000007E), uint (0x00000000), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00001818), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000202), uint (0x04040808), uint (0x10102020), uint (0x40400000) ),
                                         uvec4 ( uint (0x0000003C), uint (0x42464A52), uint (0x6242423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000008), uint (0x18280808), uint (0x0808083E), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000003C), uint (0x42020204), uint (0x0810207E), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007E), uint (0x04081C02), uint (0x0202423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000004), uint (0x0C142444), uint (0x7E040404), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007E), uint (0x40407C02), uint (0x0202423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000001C), uint (0x2040407C), uint (0x4242423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007E), uint (0x02040408), uint (0x08101010), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000003C), uint (0x4242423C), uint (0x4242423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000003C), uint (0x4242423E), uint (0x02020438), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00181800), uint (0x00001818), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00181800), uint (0x00001818), uint (0x08081000) ),
                                         uvec4 ( uint (0x00000004), uint (0x08102040), uint (0x20100804), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00007E00), uint (0x007E0000), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000020), uint (0x10080402), uint (0x04081020), uint (0x00000000) ),
                                         uvec4 ( uint (0x00003C42), uint (0x02040810), uint (0x00001010), uint (0x00000000) ),
                                         uvec4 ( uint (0x00001C22), uint (0x414F5151), uint (0x51534D40), uint (0x201F0000) ),
                                         uvec4 ( uint (0x00000018), uint (0x24424242), uint (0x7E424242), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007C), uint (0x4242427C), uint (0x4242427C), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000001E), uint (0x20404040), uint (0x4040201E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000078), uint (0x44424242), uint (0x42424478), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007E), uint (0x4040407C), uint (0x4040407E), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007E), uint (0x4040407C), uint (0x40404040), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000001E), uint (0x20404046), uint (0x4242221E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000042), uint (0x4242427E), uint (0x42424242), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000003E), uint (0x08080808), uint (0x0808083E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000002), uint (0x02020202), uint (0x0242423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000042), uint (0x44485060), uint (0x50484442), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000040), uint (0x40404040), uint (0x4040407E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000041), uint (0x63554949), uint (0x41414141), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000042), uint (0x62524A46), uint (0x42424242), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000003C), uint (0x42424242), uint (0x4242423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007C), uint (0x4242427C), uint (0x40404040), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000003C), uint (0x42424242), uint (0x4242423C), uint (0x04020000) ),
                                         uvec4 ( uint (0x0000007C), uint (0x4242427C), uint (0x48444242), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000003E), uint (0x40402018), uint (0x0402027C), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007F), uint (0x08080808), uint (0x08080808), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000042), uint (0x42424242), uint (0x4242423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000042), uint (0x42424242), uint (0x24241818), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000041), uint (0x41414149), uint (0x49495563), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000041), uint (0x41221408), uint (0x14224141), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000041), uint (0x41221408), uint (0x08080808), uint (0x00000000) ),
                                         uvec4 ( uint (0x0000007E), uint (0x04080810), uint (0x1020207E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00001E10), uint (0x10101010), uint (0x10101010), uint (0x101E0000) ),
                                         uvec4 ( uint (0x00004040), uint (0x20201010), uint (0x08080404), uint (0x02020000) ),
                                         uvec4 ( uint (0x00007808), uint (0x08080808), uint (0x08080808), uint (0x08780000) ),
                                         uvec4 ( uint (0x00001028), uint (0x44000000), uint (0x00000000), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00000000), uint (0x00FF0000) ),
                                         uvec4 ( uint (0x00201008), uint (0x04000000), uint (0x00000000), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x003C0202), uint (0x3E42423E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00004040), uint (0x407C4242), uint (0x4242427C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x003C4240), uint (0x4040423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000202), uint (0x023E4242), uint (0x4242423E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x003C4242), uint (0x7E40403E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000E10), uint (0x107E1010), uint (0x10101010), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x003E4242), uint (0x4242423E), uint (0x02023C00) ),
                                         uvec4 ( uint (0x00004040), uint (0x407C4242), uint (0x42424242), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000808), uint (0x00380808), uint (0x0808083E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000404), uint (0x001C0404), uint (0x04040404), uint (0x04043800) ),
                                         uvec4 ( uint (0x00004040), uint (0x40444850), uint (0x70484442), uint (0x00000000) ),
                                         uvec4 ( uint (0x00003808), uint (0x08080808), uint (0x0808083E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00774949), uint (0x49494949), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x007C4242), uint (0x42424242), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x003C4242), uint (0x4242423C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x007C4242), uint (0x4242427C), uint (0x40404000) ),
                                         uvec4 ( uint (0x00000000), uint (0x003E4242), uint (0x4242423E), uint (0x02020200) ),
                                         uvec4 ( uint (0x00000000), uint (0x002E3020), uint (0x20202020), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x003E4020), uint (0x1804027C), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000010), uint (0x107E1010), uint (0x1010100E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00424242), uint (0x4242423E), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00424242), uint (0x24241818), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00414141), uint (0x49495563), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00412214), uint (0x08142241), uint (0x00000000) ),
                                         uvec4 ( uint (0x00000000), uint (0x00424242), uint (0x4242423E), uint (0x02023C00) ),
                                         uvec4 ( uint (0x00000000), uint (0x007E0408), uint (0x1020407E), uint (0x00000000) ),
                                         uvec4 ( uint (0x000E1010), uint (0x101010E0), uint (0x10101010), uint (0x100E0000) ),
                                         uvec4 ( uint (0x00080808), uint (0x08080808), uint (0x08080808), uint (0x08080000) ),
                                         uvec4 ( uint (0x00700808), uint (0x08080807), uint (0x08080808), uint (0x08700000) ),
                                         uvec4 ( uint (0x00003149), uint (0x46000000), uint (0x00000000), uint (0x00000000) ), // 0x7E: '~'
                                         uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00000000), uint (0x00000000) )  // 0x7F: BACKSPACE
                                         );

                                     uint pixeInGlyph (in vec2 glyphUV, in uint AsciiCode);
                                     void main()
                                     {
                                         fragColor = mix (uBackgroundColor, uColor, pixeInGlyph (fGlyphUV, pAsciiCode));
                                     }

                                     uint __pixeInGlyph (in uvec2 glyphUV /*[0, 7] x [0, 15]*/, in uint AsciiCode /*[0, 255]*/)
                                     {
                                         if (AsciiCode < uint (0x20) || AsciiCode > uint (0x7F))
                                             return 0u;

                                         uint glyphFourLines = glyphs[AsciiCode - uint (0x20)][glyphUV.y / 4u];
                                         uint glyphCurrentLine = (glyphFourLines >> (8u * (3u - (glyphUV.y) % 4u))) & uint (0xff);
                                         return (glyphCurrentLine >> (7u - glyphUV.x)) & uint (0x01);
                                     }

                                     uint pixeInGlyph (in vec2 glyphUV, in uint AsciiCode)
                                     {
                                         return __pixeInGlyph (uvec2 (floor (glyphUV)), AsciiCode);
                                     }
                                    )";

  int vertexShaderId = glCreateShader (GL_VERTEX_SHADER);
  int geomtryShaderId = glCreateShader (GL_GEOMETRY_SHADER);
  int fragmentShaderId = glCreateShader (GL_FRAGMENT_SHADER);

  glShaderSource (vertexShaderId, 1, &vertexShaderSource, NULL);
  glCompileShader (vertexShaderId);

  glShaderSource (geomtryShaderId, 1, &geometryShaderSource, NULL);
  glCompileShader (geomtryShaderId);

  glShaderSource (fragmentShaderId, 1, &fragmentShaderSource, NULL);
  glCompileShader (fragmentShaderId);

  mProgramId = glCreateProgram ();

  glAttachShader (mProgramId, vertexShaderId);
  glAttachShader (mProgramId, geomtryShaderId);
  glAttachShader (mProgramId, fragmentShaderId);

  glLinkProgram (mProgramId);

  glDeleteShader (vertexShaderId);
  glDeleteShader (geomtryShaderId);
  glDeleteShader (fragmentShaderId);

  mUniformPositionLoc                  = glGetUniformLocation (mProgramId, "uPosition");
  mUniformColorLoc                     = glGetUniformLocation (mProgramId, "uColor");
  mUniformBackgroundColorLoc           = glGetUniformLocation (mProgramId, "uBackgroundColor");
  mUniformSizeLoc                      = glGetUniformLocation (mProgramId, "uSize");
  mUniformModelViewProjectionMatrixLoc = glGetUniformLocation (mProgramId, "uModelViewProjectionMatrix");

  mTextAttributeLoc = glGetAttribLocation (mProgramId, "aAsciiCode");

  if (isCreated ())
    {
      bind ();
      glUniformMatrix4fv (mUniformModelViewProjectionMatrixLoc, 1, GL_FALSE, mModelViewProjectionMatrix);
      unbind ();
    }
}


bool TextLineShader::isCreated () const noexcept
{
  return mProgramId;
}

void TextLineShader::destroy () noexcept
{
  if (isCreated ())
    {
      glDeleteProgram (mProgramId);
      mProgramId = 0;
    }
}

int TextLineShader::textAttributeLocation () const
{
  return mTextAttributeLoc;
}

void TextLineShader::bind ()
{
  glUseProgram (mProgramId);
}

void TextLineShader::unbind ()
{
  glUseProgram (0);
}

const float *TextLineShader::position () const noexcept
{
  return mPosition;
}

const float *TextLineShader::color () const noexcept
{
  return mColor;
}

const float *TextLineShader::backgroundColor () const noexcept
{
  return mBackgroundColor;
}

const float *TextLineShader::size () const noexcept
{
  return mSize;
}

const float *TextLineShader::modelViewProjectionMatrix () const noexcept
{
  return mModelViewProjectionMatrix;
}

void TextLineShader::setPosition (const float *pos) noexcept
{
  return setPosition (pos[0], pos[1], pos[2]);
}

void TextLineShader::setPosition (float x, float y, float z) noexcept
{
  if (   almostEqual (x, mPosition[0])
      && almostEqual (y, mPosition[1])
      && almostEqual (z, mPosition[2]))
    return;

  mPosition[0] = x;
  mPosition[1] = y;
  mPosition[2] = z;

  glUniform3f (mUniformPositionLoc, x, y, z);
}

void TextLineShader::setColor (const float *color) noexcept
{
  return setColor (color[0], color[1], color[2], color[3]);
}

void TextLineShader::setColor (float r, float g, float b, float a) noexcept
{
  if (   almostEqual (r, mColor[0])
      && almostEqual (g, mColor[1])
      && almostEqual (b, mColor[2])
      && almostEqual (b, mColor[3]))
    return;

  mColor[0] = r;
  mColor[1] = g;
  mColor[2] = b;
  mColor[3] = a;

  glUniform4f (mUniformColorLoc, r, g, b, a);
}

void TextLineShader::setBackgroundColor (const float *color) noexcept
{
  return setBackgroundColor (color[0], color[1], color[2], color[3]);
}

void TextLineShader::setBackgroundColor (float r, float g, float b, float a) noexcept
{
  if (   almostEqual (r, mBackgroundColor[0])
      && almostEqual (g, mBackgroundColor[1])
      && almostEqual (b, mBackgroundColor[2])
      && almostEqual (b, mBackgroundColor[3]))
    return;

  mBackgroundColor[0] = r;
  mBackgroundColor[1] = g;
  mBackgroundColor[2] = b;
  mBackgroundColor[3] = a;

  glUniform4f (mUniformBackgroundColorLoc, r, g, b, a);
}

void TextLineShader::setSize (const float *size) noexcept
{
  return setSize (size[0], size[1]);
}

void TextLineShader::setSize (float width, float height) noexcept
{
  if (   almostEqual (width , mSize[0])
      && almostEqual (height, mSize[1]))
    return;

  mSize[0] = width;
  mSize[1] = height;

  glUniform2f (mUniformSizeLoc, width, height);
}

void TextLineShader::setModelViewProjectionMatrix (const float *matrix) noexcept
{

  if (   almostEqual (matrix[0 ], mModelViewProjectionMatrix[0 ])
      && almostEqual (matrix[1 ], mModelViewProjectionMatrix[1 ])
      && almostEqual (matrix[2 ], mModelViewProjectionMatrix[2 ])
      && almostEqual (matrix[3 ], mModelViewProjectionMatrix[3 ])
      && almostEqual (matrix[4 ], mModelViewProjectionMatrix[4 ])
      && almostEqual (matrix[5 ], mModelViewProjectionMatrix[5 ])
      && almostEqual (matrix[6 ], mModelViewProjectionMatrix[6 ])
      && almostEqual (matrix[7 ], mModelViewProjectionMatrix[7 ])
      && almostEqual (matrix[8 ], mModelViewProjectionMatrix[8 ])
      && almostEqual (matrix[9 ], mModelViewProjectionMatrix[9 ])
      && almostEqual (matrix[10], mModelViewProjectionMatrix[10])
      && almostEqual (matrix[11], mModelViewProjectionMatrix[11])
      && almostEqual (matrix[12], mModelViewProjectionMatrix[12])
      && almostEqual (matrix[13], mModelViewProjectionMatrix[13])
      && almostEqual (matrix[14], mModelViewProjectionMatrix[14])
      && almostEqual (matrix[15], mModelViewProjectionMatrix[15]))
    return;

  memcpy (mModelViewProjectionMatrix, matrix, 16 * sizeof (float));

  glUniformMatrix4fv (mUniformModelViewProjectionMatrixLoc, 1, GL_FALSE, mModelViewProjectionMatrix);
}
}
}
}
