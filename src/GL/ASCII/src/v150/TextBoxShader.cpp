#include "GL/ASCII/v150/Debug/TextBoxShader.h"
#include <GL/glew.h> /* for OpenGL */
#include <stddef.h>

namespace GL
{
namespace ASCII
{
namespace v150
{
namespace Debug
{
TextBoxShader::TextBoxShader () noexcept
{
}

void TextBoxShader::init () noexcept
{
  const char *vertexShaderSource = R"(#version 130

                                      attribute uint a_ascii_code; // vanila vertex attribute: ascii code (belongs to [0, 255])

                                      uniform vec3 u_gl_position;    // position of word's left bottom
                                      uniform vec2 u_gl_glyph_size;  // gl size of each glyph
                                      uniform int  u_box_width;      // box width (glyph count)

                                      varying uint v_ascii_code;

                                      void main ()
                                      {
                                        v_ascii_code = a_ascii_code;
                                        gl_Position = vec4 (u_gl_position, 1);
                                        gl_Position.x += (gl_VertexID % u_box_width) * u_gl_glyph_size.x;
                                        gl_Position.y -= (gl_VertexID / u_box_width) * u_gl_glyph_size.y;
                                      }
                                    )";

  const char *geometryShaderSource = R"(#version 150

                                     layout (points) in;
                                     layout (triangle_strip, max_vertices = 4) out;

                                     in uint v_ascii_code[]; // vertex attribute: ascii code (belongs to [0, 255])

                                     uniform vec2  u_gl_glyph_size; // gl size of each glyph

                                     flat   out uint p_ascii_code; // primitive attribute: ascii code (belongs to [0, 255])
                                     smooth out vec2 f_glyph_uv;   // fragment attribute: UV texels coords

                                     #define GLYPH_WIDTH 8.
                                     #define GLYPH_HEIGHT 16.

                                     void emit_vertex (in vec2 glyph_offset, in vec2 glyph_uv);

                                     void main()
                                     {
                                         p_ascii_code = v_ascii_code[0];

                                         emit_vertex (vec2 (0., 0.), vec2 (0., GLYPH_HEIGHT));
                                         emit_vertex (vec2 (0., u_gl_glyph_size.y), vec2 (0., 0.));
                                         emit_vertex (vec2 (u_gl_glyph_size.x, 0.), vec2 (GLYPH_WIDTH, GLYPH_HEIGHT));
                                         emit_vertex (u_gl_glyph_size, vec2 (GLYPH_WIDTH, 0.));

                                         EndPrimitive ();
                                     }

                                     void emit_vertex (in vec2 glyph_offset, in vec2 glyph_uv)
                                     {
                                         gl_Position = vec4 (gl_in[0].gl_Position.xy + glyph_offset, gl_in[0].gl_Position.zw);
                                         f_glyph_uv = glyph_uv;
                                         EmitVertex ();
                                     }
                                    )";

  const char *fragmentShaderSource = R"(#version 130

                                     flat   in uint p_ascii_code;  // primitive attribute: ascii code (belongs to [0, 255])
                                     smooth in vec2 f_glyph_uv;    // fragment attribute: UV texels coords

                                     uniform vec4 u_color;            // color of word
                                     uniform vec4 u_background_color; // color of word background

                                     out vec4 FragColor;

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

                                     uint pixel_in_glyph (in vec2 glyph_uv, in uint ascii_code);
                                     void main()
                                     {
                                         FragColor = mix (u_background_color, u_color, pixel_in_glyph (f_glyph_uv, p_ascii_code));
                                     }

                                     uint __pixel_in_glyph (in uvec2 glyph_uv /*[0, 7] x [0, 15]*/, in uint ascii_code /*[0, 255]*/)
                                     {
                                         if (ascii_code < uint (0x20) || ascii_code > uint (0x7F))
                                             return 0u;

                                         uint glyph_four_lines = glyphs[ascii_code - uint (0x20)][glyph_uv.y / 4u];
                                         uint current_line = (glyph_four_lines >> (8u * (3u - (glyph_uv.y) % 4u))) & uint (0xff);
                                         return (current_line >> (7u - glyph_uv.x)) & uint (0x01);
                                     }

                                     uint pixel_in_glyph (in vec2 glyph_uv, in uint ascii_code)
                                     {
                                         return __pixel_in_glyph (uvec2 (floor (glyph_uv)), ascii_code);
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

  mUniformPositionLoc        = glGetUniformLocation (mProgramId, "u_gl_position");
  mUniformColorLoc           = glGetUniformLocation (mProgramId, "u_color");
  mUniformBackgroundColorLoc = glGetUniformLocation (mProgramId, "u_background_color");
  mUniformSizeLoc            = glGetUniformLocation (mProgramId, "u_gl_glyph_size");
  mUniformBoxWidthLoc        = glGetUniformLocation (mProgramId, "u_box_width");
  mAttributeCodeLoc          = glGetAttribLocation (mProgramId, "a_ascii_code");

  glDeleteShader (vertexShaderId);
  glDeleteShader (geomtryShaderId);
  glDeleteShader (fragmentShaderId);
}

TextBoxShader::~TextBoxShader () noexcept
{
  glDeleteProgram (mProgramId);
}

void TextBoxShader::bind ()
{
  glUseProgram (mProgramId);
}

void TextBoxShader::unbind ()
{
  glUseProgram (0);
}

const float *TextBoxShader::position () const noexcept
{
  return mPosition;
}

const float *TextBoxShader::color () const noexcept
{
  return mColor;
}

const float *TextBoxShader::backgroundColor () const noexcept
{
  return mBackgroundColor;
}

const float *TextBoxShader::size () const noexcept
{
  return mSize;
}

int TextBoxShader::boxWidth () const noexcept
{
  return mBoxWidth;
}

void TextBoxShader::setPosition (const float *pos) noexcept
{
  return setPosition (pos[0], pos[1], pos[2]);
}

void TextBoxShader::setPosition (float x, float y, float z) noexcept
{
  glUniform3f (mUniformPositionLoc, x, y, z);
}

void TextBoxShader::setColor (const float *color) noexcept
{
  return setColor (color[0], color[1], color[2], color[3]);
}

void TextBoxShader::setColor (float r, float g, float b, float a) noexcept
{
  glUniform4f (mUniformColorLoc, r, g, b, a);
}

void TextBoxShader::setBackgroundColor (const float *color) noexcept
{
  return setBackgroundColor (color[0], color[1], color[2], color[3]);
}

void TextBoxShader::setBackgroundColor (float r, float g, float b, float a) noexcept
{
  glUniform4f (mUniformBackgroundColorLoc, r, g, b, a);
}

void TextBoxShader::setSize (const float *size) noexcept
{
  return setSize (size[0], size[1]);
}

void TextBoxShader::setSize (float width, float height) noexcept
{
  glUniform2f (mUniformSizeLoc, width, height);
}

void TextBoxShader::setBoxWidth (int width) noexcept
{
  glUniform1i (mUniformBoxWidthLoc, width);
}

int TextBoxShader::attributeCodeLocation () const
{
  return mAttributeCodeLoc;
}
}
}
}
}
