#include "GL/VertexArray.h"
#include <GL/glew.h> /* for OpenGL */

namespace GL
{
VertexArray::VertexArray () noexcept
{
}

VertexArray::VertexArray (VertexArray &&other) noexcept: mId (other.mId)
{
  other.mId = 0;
}

VertexArray::~VertexArray () noexcept
{
  if (isCreated ())
    glDeleteVertexArrays (1, &mId);
}

void VertexArray::create () noexcept
{
  if (!isCreated ())
    glGenVertexArrays (1, &mId);
}

bool VertexArray::isCreated () const noexcept
{
  return mId != 0;
}

void VertexArray::destroy () noexcept
{
  if (isCreated ())
    {
      glDeleteBuffers (1, &mId);
      mId = 0;
    }
}

void VertexArray::bind () const noexcept
{
  glBindVertexArray (mId);
}

void VertexArray::unbind () const noexcept
{
  glBindVertexArray (0);
}
}
