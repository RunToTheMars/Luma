#include "GLASCII/impl/v150/Word.h"
#include "GL/Shader.h"
#include "GL/ShaderProgram.h"
#include <GL/glew.h> /* for OpenGL */
#include <cstring>
#include <stdio.h>

namespace
{
class WordImpl
{
public:
  static WordImpl &get_instance ();

  bool isEnabled () const { return m_program.has_value (); }
  GL::ShaderProgram &program () { return *m_program; }

  int uniformGLPositionLoc      () const { return m_uniformGLPositionLoc; }
  int uniformColorLoc           () const { return m_uniformColorLoc; }
  int uniformBackgroundColorLoc () const { return m_uniformBackgroundColorLoc; }
  int uniformGLSizeLoc          () const { return m_uniformGLSizeLoc; }

private:
  WordImpl ();

  std::optional<GL::ShaderProgram> m_program;
  int m_uniformGLPositionLoc = 0;
  int m_uniformColorLoc = 0;
  int m_uniformBackgroundColorLoc = 0;
  int m_uniformGLSizeLoc = 0;

public:
  float m_GLPosition[3] = {0.f, 0.f, 0.f};
  float m_color[4] = {0.f, 0.f, 0.f, 1.f};            /// black
  float m_backgroundColor[4] = {0.f, 0.f, 0.f, 0.f};  /// black transparent
  float m_GLSize[2] = {0.1f, 0.1f};
};

WordImpl &WordImpl::get_instance ()
{
    static WordImpl impl;
    return impl;
}

std::optional<GL::Shader> createShader (const char *code, GL::ShaderType type)
{
  GL::ShaderCompiler compiler;
  std::optional<GL::Shader> shader = compiler.compileCode (code, type);

  if (!shader)
    fprintf (stderr, "Vertex Shader compilation problem:\n%s", compiler.compileError ());

  return shader;
}

std::optional<GL::ShaderProgram> createShaderProgram ()
{
  std::optional<GL::Shader> vShader = createShader (
      "#version 130                                                                               \n"
      "                                                                                           \n"
      "attribute uint a_ascii_code; // vanila vertex attribute: ascii code (belongs to [0, 255])  \n"
      "                                                                                           \n"
      "uniform vec3 u_gl_position;       // position of word's left bottom                        \n"
      "uniform vec2 u_gl_glyph_size;  // gl size of each glyph                                    \n"
      "                                                                                           \n"
      "varying uint v_ascii_code;                                                                 \n"
      "                                                                                           \n"
      "void main ()                                                                               \n"
      "{                                                                                          \n"
      "  v_ascii_code = a_ascii_code;                                                             \n"
      "  gl_Position = vec4 (u_gl_position, 1);                                                   \n"
      "  gl_Position.x += gl_VertexID * u_gl_glyph_size.x;                                        \n"
      "}                                                                                          \n"
      "                                                                                           \n",
      GL::ShaderType::Vertex);

  if (!vShader)
    return std::nullopt;

  std::optional<GL::Shader> gShader = createShader (
      "#version 150                                                                             \n"
      "                                                                                         \n"
      "layout (points) in;                                                                      \n"
      "layout (triangle_strip, max_vertices = 4) out;                                           \n"
      "                                                                                         \n"
      "in uint v_ascii_code[]; // vertex attribute: ascii code (belongs to [0, 255])            \n"
      "                                                                                         \n"
      "uniform vec2  u_gl_glyph_size; // gl size of each glyph                                  \n"
      "                                                                                         \n"
      "flat   out uint p_ascii_code; // primitive attribute: ascii code (belongs to [0, 255])   \n"
      "smooth out vec2 f_glyph_uv;   // fragment attribute: UV texels coords                    \n"
      "                                                                                         \n"
      "#define GLYPH_WIDTH 8.                                                                   \n"
      "#define GLYPH_HEIGHT 16.                                                                 \n"
      "                                                                                         \n"
      "void emit_vertex (in vec2 glyph_offset, in vec2 glyph_uv);                               \n"
      "                                                                                         \n"
      "void main()                                                                              \n"
      "{                                                                                        \n"
      "    p_ascii_code = v_ascii_code[0];                                                      \n"
      "                                                                                         \n"
      "    emit_vertex (vec2 (0., 0.), vec2 (0., GLYPH_HEIGHT));                                \n"
      "    emit_vertex (vec2 (0., u_gl_glyph_size.y), vec2 (0., 0.));                           \n"
      "    emit_vertex (vec2 (u_gl_glyph_size.x, 0.), vec2 (GLYPH_WIDTH, GLYPH_HEIGHT));        \n"
      "    emit_vertex (u_gl_glyph_size, vec2 (GLYPH_WIDTH, 0.));                               \n"
      "                                                                                         \n"
      "    EndPrimitive ();                                                                     \n"
      "}                                                                                        \n"
      "                                                                                         \n"
      "void emit_vertex (in vec2 glyph_offset, in vec2 glyph_uv)                                \n"
      "{                                                                                        \n"
      "    gl_Position = vec4 (gl_in[0].gl_Position.xy + glyph_offset, gl_in[0].gl_Position.zw);\n"
      "    f_glyph_uv = glyph_uv;                                                               \n"
      "    EmitVertex ();                                                                       \n"
      "}                                                                                        \n",
      GL::ShaderType::Geometry);

  if (!gShader)
    return std::nullopt;

  std::optional<GL::Shader> fShader = createShader (
      "#version 130                                                                                                 \n"
      "                                                                                                             \n"
      "flat   in uint p_ascii_code;  // primitive attribute: ascii code (belongs to [0, 255])                       \n"
      "smooth in vec2 f_glyph_uv;    // fragment attribute: UV texels coords                                        \n"
      "                                                                                                             \n"
      "uniform vec4 u_color;            // color of word                                                            \n"
      "uniform vec4 u_background_color; // color of word background                                                 \n"
      "                                                                                                             \n"
      "out vec4 FragColor;                                                                                          \n"
      "                                                                                                             \n"
      "const uvec4 glyphs[96] = uvec4[96](                                                                          \n"
      "    uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00000000), uint (0x00000000) ), // 0x20: SPACE     \n"
      "    uvec4 ( uint (0x00001010), uint (0x10101010), uint (0x00001010), uint (0x00000000) ), // 0x21: '!'       \n"
      "    uvec4 ( uint (0x00242424), uint (0x24000000), uint (0x00000000), uint (0x00000000) ), // 0x22: '\'       \n"
      "    uvec4 ( uint (0x00000024), uint (0x247E2424), uint (0x247E2424), uint (0x00000000) ), // 0x23: '#'       \n"
      "    uvec4 ( uint (0x00000808), uint (0x1E20201C), uint (0x02023C08), uint (0x08000000) ), // ...             \n"
      "    uvec4 ( uint (0x00000030), uint (0x494A3408), uint (0x16294906), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00003048), uint (0x48483031), uint (0x49464639), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00101010), uint (0x10000000), uint (0x00000000), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000408), uint (0x08101010), uint (0x10101008), uint (0x08040000) ),                    \n"
      "    uvec4 ( uint (0x00002010), uint (0x10080808), uint (0x08080810), uint (0x10200000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x0024187E), uint (0x18240000), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x0808087F), uint (0x08080800), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00001818), uint (0x08081000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x0000007E), uint (0x00000000), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00001818), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000202), uint (0x04040808), uint (0x10102020), uint (0x40400000) ),                    \n"
      "    uvec4 ( uint (0x0000003C), uint (0x42464A52), uint (0x6242423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000008), uint (0x18280808), uint (0x0808083E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000003C), uint (0x42020204), uint (0x0810207E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007E), uint (0x04081C02), uint (0x0202423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000004), uint (0x0C142444), uint (0x7E040404), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007E), uint (0x40407C02), uint (0x0202423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000001C), uint (0x2040407C), uint (0x4242423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007E), uint (0x02040408), uint (0x08101010), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000003C), uint (0x4242423C), uint (0x4242423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000003C), uint (0x4242423E), uint (0x02020438), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00181800), uint (0x00001818), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00181800), uint (0x00001818), uint (0x08081000) ),                    \n"
      "    uvec4 ( uint (0x00000004), uint (0x08102040), uint (0x20100804), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00007E00), uint (0x007E0000), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000020), uint (0x10080402), uint (0x04081020), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00003C42), uint (0x02040810), uint (0x00001010), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00001C22), uint (0x414F5151), uint (0x51534D40), uint (0x201F0000) ),                    \n"
      "    uvec4 ( uint (0x00000018), uint (0x24424242), uint (0x7E424242), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007C), uint (0x4242427C), uint (0x4242427C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000001E), uint (0x20404040), uint (0x4040201E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000078), uint (0x44424242), uint (0x42424478), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007E), uint (0x4040407C), uint (0x4040407E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007E), uint (0x4040407C), uint (0x40404040), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000001E), uint (0x20404046), uint (0x4242221E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000042), uint (0x4242427E), uint (0x42424242), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000003E), uint (0x08080808), uint (0x0808083E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000002), uint (0x02020202), uint (0x0242423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000042), uint (0x44485060), uint (0x50484442), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000040), uint (0x40404040), uint (0x4040407E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000041), uint (0x63554949), uint (0x41414141), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000042), uint (0x62524A46), uint (0x42424242), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000003C), uint (0x42424242), uint (0x4242423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007C), uint (0x4242427C), uint (0x40404040), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000003C), uint (0x42424242), uint (0x4242423C), uint (0x04020000) ),                    \n"
      "    uvec4 ( uint (0x0000007C), uint (0x4242427C), uint (0x48444242), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000003E), uint (0x40402018), uint (0x0402027C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007F), uint (0x08080808), uint (0x08080808), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000042), uint (0x42424242), uint (0x4242423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000042), uint (0x42424242), uint (0x24241818), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000041), uint (0x41414149), uint (0x49495563), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000041), uint (0x41221408), uint (0x14224141), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000041), uint (0x41221408), uint (0x08080808), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x0000007E), uint (0x04080810), uint (0x1020207E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00001E10), uint (0x10101010), uint (0x10101010), uint (0x101E0000) ),                    \n"
      "    uvec4 ( uint (0x00004040), uint (0x20201010), uint (0x08080404), uint (0x02020000) ),                    \n"
      "    uvec4 ( uint (0x00007808), uint (0x08080808), uint (0x08080808), uint (0x08780000) ),                    \n"
      "    uvec4 ( uint (0x00001028), uint (0x44000000), uint (0x00000000), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00000000), uint (0x00FF0000) ),                    \n"
      "    uvec4 ( uint (0x00201008), uint (0x04000000), uint (0x00000000), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x003C0202), uint (0x3E42423E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00004040), uint (0x407C4242), uint (0x4242427C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x003C4240), uint (0x4040423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000202), uint (0x023E4242), uint (0x4242423E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x003C4242), uint (0x7E40403E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000E10), uint (0x107E1010), uint (0x10101010), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x003E4242), uint (0x4242423E), uint (0x02023C00) ),                    \n"
      "    uvec4 ( uint (0x00004040), uint (0x407C4242), uint (0x42424242), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000808), uint (0x00380808), uint (0x0808083E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000404), uint (0x001C0404), uint (0x04040404), uint (0x04043800) ),                    \n"
      "    uvec4 ( uint (0x00004040), uint (0x40444850), uint (0x70484442), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00003808), uint (0x08080808), uint (0x0808083E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00774949), uint (0x49494949), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x007C4242), uint (0x42424242), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x003C4242), uint (0x4242423C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x007C4242), uint (0x4242427C), uint (0x40404000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x003E4242), uint (0x4242423E), uint (0x02020200) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x002E3020), uint (0x20202020), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x003E4020), uint (0x1804027C), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000010), uint (0x107E1010), uint (0x1010100E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00424242), uint (0x4242423E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00424242), uint (0x24241818), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00414141), uint (0x49495563), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00412214), uint (0x08142241), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x00424242), uint (0x4242423E), uint (0x02023C00) ),                    \n"
      "    uvec4 ( uint (0x00000000), uint (0x007E0408), uint (0x1020407E), uint (0x00000000) ),                    \n"
      "    uvec4 ( uint (0x000E1010), uint (0x101010E0), uint (0x10101010), uint (0x100E0000) ),                    \n"
      "    uvec4 ( uint (0x00080808), uint (0x08080808), uint (0x08080808), uint (0x08080000) ),                    \n"
      "    uvec4 ( uint (0x00700808), uint (0x08080807), uint (0x08080808), uint (0x08700000) ),                    \n"
      "    uvec4 ( uint (0x00003149), uint (0x46000000), uint (0x00000000), uint (0x00000000) ), // 0x7E: '~'       \n"
      "    uvec4 ( uint (0x00000000), uint (0x00000000), uint (0x00000000), uint (0x00000000) )  // 0x7F: BACKSPACE \n"
      "    );                                                                                                       \n"
      "                                                                                                             \n"
      "uint pixel_in_glyph (in vec2 glyph_uv, in uint ascii_code);                                                  \n"
      "void main()                                                                                                  \n"
      "{                                                                                                            \n"
      "    FragColor = mix (u_background_color, u_color, pixel_in_glyph (f_glyph_uv, p_ascii_code));                \n"
      "}                                                                                                            \n"
      "                                                                                                             \n"
      "uint __pixel_in_glyph (in uvec2 glyph_uv /*[0, 7] x [0, 15]*/, in uint ascii_code /*[0, 255]*/)              \n"
      "{                                                                                                            \n"
      "    if (ascii_code < uint (0x20) || ascii_code > uint (0x7F))                                                \n"
      "        return 0u;                                                                                           \n"
      "                                                                                                             \n"
      "    uint glyph_four_lines = glyphs[ascii_code - uint (0x20)][glyph_uv.y / 4u];                               \n"
      "    uint current_line = (glyph_four_lines >> (8u * (3u - (glyph_uv.y) % 4u))) & uint (0xff);                 \n"
      "    return (current_line >> (7u - glyph_uv.x)) & uint (0x01);                                                \n"
      "}                                                                                                            \n"
      "                                                                                                             \n"
      "uint pixel_in_glyph (in vec2 glyph_uv, in uint ascii_code)                                                   \n"
      "{                                                                                                            \n"
      "    return __pixel_in_glyph (uvec2 (floor (glyph_uv)), ascii_code);                                          \n"
      "}                                                                                                            \n",
      GL::ShaderType::Fragment);

  if (!fShader)
    return std::nullopt;

  GL::ShaderProgramLinker linker;
  linker.add (*vShader);
  linker.add (*gShader);
  linker.add (*fShader);
  std::optional<GL::ShaderProgram> program = linker.link ();

  if (!program)
      fprintf (stderr, "Shader Program link problem:\n%s", linker.linkError ());

  return program;
}

WordImpl::WordImpl ()
{
  m_program = createShaderProgram ();
  if (m_program)
  {
      m_uniformGLPositionLoc      = m_program->uniform_location ("u_gl_position");
      m_uniformColorLoc           = m_program->uniform_location ( "u_color");
      m_uniformBackgroundColorLoc = m_program->uniform_location ("u_background_color");
      m_uniformGLSizeLoc          = m_program->uniform_location ("u_gl_glyph_size");
  }
  else
  {
      m_uniformGLPositionLoc      = -1;
      m_uniformColorLoc           = -1;
      m_uniformBackgroundColorLoc = -1;
      m_uniformGLSizeLoc          = -1;
  }
}
}  // namespace

namespace GL_ASCII
{
namespace v150
{
namespace Word
{
bool isEnabled () { return WordImpl::get_instance ().isEnabled (); }
const float *position () { return WordImpl::get_instance ().m_GLPosition; }
const float *color () { return WordImpl::get_instance ().m_color; }
const float *backgroundColor () { return WordImpl::get_instance ().m_backgroundColor; }

SymbolsData::SymbolsData (const char *symbols): SymbolsData (symbols, strlen (symbols))
{
}

SymbolsData::SymbolsData (const char *symbols, unsigned int symbolsCount): m_symbolsCount (symbolsCount)
{
  unsigned int symbolsVBO;
  glGenBuffers (1, &m_VBO);
  glBindBuffer (GL_ARRAY_BUFFER, m_VBO);
  glBufferData (GL_ARRAY_BUFFER, symbolsCount, symbols, GL_STATIC_DRAW);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
}

SymbolsData::~SymbolsData () { glDeleteBuffers (1, &m_VBO); }

Binder::Binder () : m_internalBinder (WordImpl::get_instance ().program ().bind ()) {}
Binder::~Binder () = default;

Binder &Binder::setGLPosition (const float *pos) { return setGLPosition (pos[0], pos[1], pos[2]); }
Binder &Binder::setGLPosition (float x, float y, float z)
{
  glUniform3f (WordImpl::get_instance ().uniformGLPositionLoc (), x, y, z);
  WordImpl::get_instance ().m_GLPosition[0] = x;
  WordImpl::get_instance ().m_GLPosition[1] = y;
  WordImpl::get_instance ().m_GLPosition[2] = z;
  return *this;
}

Binder &Binder::setColor (const float *color) { return setColor (color[0], color[1], color[2], color[3]); }
Binder &Binder::setColor (float r, float g, float b, float a)
{
  glUniform4f (WordImpl::get_instance ().uniformColorLoc (), r, g, b, a);
  WordImpl::get_instance ().m_color[0] = r;
  WordImpl::get_instance ().m_color[1] = g;
  WordImpl::get_instance ().m_color[2] = b;
  WordImpl::get_instance ().m_color[3] = a;
  return *this;
}

Binder &Binder::setBackgroundColor (const float *color) { return setBackgroundColor (color[0], color[1], color[2], color[3]); }
Binder &Binder::setBackgroundColor (float r, float g, float b, float a)
{
  glUniform4f (WordImpl::get_instance ().uniformBackgroundColorLoc (), r, g, b, a);
  WordImpl::get_instance ().m_backgroundColor[0] = r;
  WordImpl::get_instance ().m_backgroundColor[1] = g;
  WordImpl::get_instance ().m_backgroundColor[2] = b;
  WordImpl::get_instance ().m_backgroundColor[3] = a;
  return *this;
}

Binder &Binder::setGLSize (const float *size) { return setGLSize (size[0], size[1]); }
Binder &Binder::setGLSize (float width, float height)
{
  glUniform2f (WordImpl::get_instance ().uniformGLSizeLoc (), width, height);
  WordImpl::get_instance ().m_GLSize[0] = width;
  WordImpl::get_instance ().m_GLSize[1] = height;
  return *this;
}

Binder &Binder::draw (const SymbolsData &data)
{
  glBindBuffer (GL_ARRAY_BUFFER, data.VBO ());

  glVertexAttribIPointer (0, 1, GL_UNSIGNED_BYTE, 0, (void *) nullptr);

  glEnableVertexAttribArray (0);
  glDrawArrays (GL_POINTS /*mode*/, 0 /* first */, data.symbolsCount () /* count */);
  glDisableVertexAttribArray (0);

  glBindBuffer (GL_ARRAY_BUFFER, 0);

  return *this;
}
}
}
}
