#pragma once

#include "GL/ShaderProgram.h"

namespace GL_ASCII
{
namespace v150
{
namespace Word
{
bool isEnabled ();

constexpr int glyphTextureWidth () { return 8; }
constexpr int glyphTextureHeight () { return 16; }

const float *GLPosition ();
const float *color ();
const float *backgroundColor ();
const float *GLSize ();

class SymbolsData
{
public:
  explicit SymbolsData (const char *symbols);
  explicit SymbolsData (const char *symbols, unsigned int symbolsCount);
  SymbolsData (const SymbolsData &) = delete;
  SymbolsData &operator= (const SymbolsData &) = delete;
  SymbolsData (SymbolsData &&) = delete;
  SymbolsData &operator= (SymbolsData &&) = delete;
  ~SymbolsData ();

  unsigned int VBO () const { return m_VBO; }
  unsigned int symbolsCount () const { return m_symbolsCount; }

private:
  unsigned int m_VBO;
  unsigned int m_symbolsCount;
};

class Binder
{
public:
  Binder ();
  ~Binder ();

  Binder &setGLPosition (const float *pos);
  Binder &setGLPosition (float x, float y, float z);

  Binder &setColor (const float *color);
  Binder &setColor (float r, float g, float b, float a = 1.f);

  Binder &setBackgroundColor (const float *color);
  Binder &setBackgroundColor (float r, float g, float b, float a = 1.f);

  Binder &setGLSize (const float *size);
  Binder &setGLSize (float width, float height);

  Binder &draw (const SymbolsData &data);

private:
  GL::ShaderProgramBinder m_internalBinder;
};
}  // namespace Word
}  // namespace v150
}  // namespace GL_ASCII
