#include "Luma/GL/Buffer.h"
#include <GL/glew.h> /* for OpenGL */

namespace Luma::GL
{
Buffer::Buffer (Type type) noexcept: mType (type)
{
}

Buffer::Buffer (Buffer &&other) noexcept: mId (other.mId), mUsagePattern (other.mUsagePattern), mCount (other.mCount)
{
  other.mId = 0;
}

Buffer::~Buffer () noexcept
{
  if (isCreated ())
    glDeleteBuffers (1, &mId);
}

void Buffer::create () noexcept
{
  if (!isCreated ())
    glGenBuffers (1, &mId);
}

bool Buffer::isCreated () const noexcept
{
  return mId != 0;
}

void Buffer::destroy () noexcept
{
  if (isCreated ())
    {
      glDeleteBuffers (1, &mId);
      mId = 0;
    }
}

void Buffer::bind () const noexcept
{
  glBindBuffer (static_cast<unsigned int> (mType), mId);
}

void Buffer::unbind () const noexcept
{
  glBindBuffer (static_cast<unsigned int> (mType), 0);
}

void Buffer::allocate (const void *data, int count, Buffer::UsagePattern usagePattern) noexcept
{
  glBufferData (static_cast<unsigned int> (mType), count, data, static_cast<unsigned int> (usagePattern));
  mCount = count;
  mUsagePattern = usagePattern;
}

void Buffer::read (int offset, void *data, int count) const noexcept
{
  glGetBufferSubData (static_cast<unsigned int> (mType), offset, count, data);
}

void Buffer::write (int offset, const void *data, int count) const noexcept
{
  glBufferSubData (static_cast<unsigned int> (mType), offset, count, data);
}

Buffer::UsagePattern Buffer::usagePattern () const noexcept
{
  return mUsagePattern;
}

int Buffer::count () const noexcept
{
  return mCount;
}
}
