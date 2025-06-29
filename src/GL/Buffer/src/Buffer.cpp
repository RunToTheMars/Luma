#include "GL/Buffer.h"
#include <GL/glew.h> /* for OpenGL */

namespace GL
{
template <Buffer::Type Type>
BufferImpl<Type>::BufferImpl () noexcept {}

template <Buffer::Type Type>
BufferImpl<Type>::BufferImpl (BufferImpl &&other) noexcept: mId (other.mId), mUsagePattern (other.mUsagePattern), mCount (other.mCount)
{
  other.mId = 0;
}

template<Buffer::Type Type>
BufferImpl<Type>::~BufferImpl () noexcept
{
  if (isCreated ())
    glDeleteBuffers (1, &mId);
}

template<Buffer::Type Type>
void BufferImpl<Type>::create () noexcept
{
  if (!isCreated ())
    glGenBuffers (1, &mId);
}

template<Buffer::Type Type>
bool BufferImpl<Type>::isCreated () const noexcept
{
  return mId != 0;
}

template<Buffer::Type Type>
Buffer::UsagePattern BufferImpl<Type>::usagePattern () const noexcept
{
  return mUsagePattern;
}

template<Buffer::Type Type>
int BufferImpl<Type>::count () const noexcept
{
  return mCount;
}

//-----------------------------------------------------------------------------------------

namespace
{
constexpr unsigned int to_gl_binding (Buffer::Type type)
{
  switch (type)
    {
      case Buffer::Type::Vertex: return GL_ARRAY_BUFFER_BINDING;
      case Buffer::Type::Index : return GL_ELEMENT_ARRAY_BUFFER_BINDING;
    }

  return GL_ARRAY_BUFFER_BINDING;
}

constexpr unsigned int to_gl_bind (Buffer::Type type)
{
  switch (type)
    {
      case Buffer::Type::Vertex: return GL_ARRAY_BUFFER;
      case Buffer::Type::Index : return GL_ELEMENT_ARRAY_BUFFER;
    }

  return GL_ARRAY_BUFFER_BINDING;
}

constexpr unsigned int to_gl_enum (Buffer::UsagePattern pattern)
{
  switch (pattern)
    {
      case Buffer::UsagePattern::StaticDraw : return GL_STATIC_DRAW;
      case Buffer::UsagePattern::StreamDraw : return GL_STREAM_DRAW;
      case Buffer::UsagePattern::DynamicDraw: return GL_DYNAMIC_DRAW;
    }

  return GL_STATIC_DRAW;
}
}

template <Buffer::Type Type>
BufferBinderInterface<Type>::BufferBinderInterface () noexcept
{
}

template <Buffer::Type Type>
BufferBinderInterface<Type>::~BufferBinderInterface () noexcept
{
}

template <Buffer::Type Type>
const BufferBinderInterface<Type> &BufferBinderInterface<Type>::bind (const BufferImpl<Type> &buffer) const noexcept
{
  glBindBuffer (to_gl_bind (Type), buffer.mId);
}

template <Buffer::Type Type>
const BufferBinderInterface<Type> &BufferBinderInterface<Type>::allocate (int count, const void *data, Buffer::UsagePattern pattern) const noexcept
{
  glBufferData (to_gl_bind (Type), count, data, to_gl_enum (pattern));
}

template <Buffer::Type Type>
const BufferBinderInterface<Type> &BufferBinderInterface<Type>::read (int offset, void *data, int count) const noexcept
{
  glGetBufferSubData (to_gl_bind (Type), offset, count, data);
}

template <Buffer::Type Type>
const BufferBinderInterface<Type> &BufferBinderInterface<Type>::write (int offset, const void *data, int count) const noexcept
{
  glBufferSubData (to_gl_bind (Type), offset, count, data);
}

//-----------------------------------------------------------------------------------------

template<Buffer::Type Type>
BufferBinderRestore<Type>::BufferBinderRestore () noexcept
{
  glGetIntegerv (to_gl_binding (Type), &mRestoreId);
}

template<Buffer::Type Type>
BufferBinderRestore<Type>::BufferBinderRestore (const BufferImpl<Type> &buffer) noexcept: BufferBinderRestore ()
{
  bind (buffer);
}

template<Buffer::Type Type>
BufferBinderRestore<Type>::~BufferBinderRestore () noexcept
{
  glBindBuffer (to_gl_bind (Type), mRestoreId);
}

//-----------------------------------------------------------------------------------------

template<Buffer::Type Type>
BufferBinderUnsafe<Type>::BufferBinderUnsafe () noexcept
{
}

template<Buffer::Type Type>
BufferBinderUnsafe<Type>::BufferBinderUnsafe (const BufferImpl<Type> &buffer) noexcept
{
  bind (buffer);
}

//-----------------------------------------------------------------------------------------
}
