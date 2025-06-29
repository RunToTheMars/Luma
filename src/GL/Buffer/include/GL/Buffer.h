#pragma once

namespace GL
{
namespace Buffer
{
enum class Type
{
  Vertex,
  Index,
};

enum class UsagePattern
{
  StaticDraw,
  StreamDraw,
  DynamicDraw,
};
}

template <Buffer::Type Type>
class BufferBinderInterface;

template <Buffer::Type Type>
class BufferImpl
{
public:
  BufferImpl () noexcept;
  BufferImpl (const BufferImpl &) = delete;
  BufferImpl (BufferImpl &&) noexcept;
  ~BufferImpl () noexcept;

  BufferImpl &operator=(const BufferImpl &) noexcept;
  BufferImpl &operator=(BufferImpl &&) noexcept;

  void create () noexcept;
  bool isCreated () const noexcept;

  /// \note Only for allocated buffer
  Buffer::UsagePattern usagePattern () const noexcept;
  int count () const noexcept;

private:
  friend class GL::BufferBinderInterface<Type>;

  unsigned int mId = 0;
  Buffer::UsagePattern mUsagePattern = Buffer::UsagePattern::StaticDraw;
  int mCount = 0;
};

template <Buffer::Type Type>
class BufferBinderInterface
{
protected:
  BufferBinderInterface () noexcept;
  ~BufferBinderInterface () noexcept;

public:
  const BufferBinderInterface &bind (const BufferImpl<Type> &buffer) const noexcept;
  const BufferBinderInterface &allocate (int count, const void *data = nullptr, Buffer::UsagePattern pattern = Buffer::UsagePattern::StaticDraw) const noexcept;
  const BufferBinderInterface &read (int offset, void *data, int count) const noexcept;
  const BufferBinderInterface &write (int offset, const void *data, int count) const noexcept;
};

template <Buffer::Type Type>
class BufferBinderRestore: public BufferBinderInterface<Type>
{
public:
  BufferBinderRestore () noexcept;
  BufferBinderRestore (const BufferImpl<Type> &buffer) noexcept;
  BufferBinderRestore (const BufferBinderRestore &) = delete;
  BufferBinderRestore (BufferBinderRestore &&) = delete;
  ~BufferBinderRestore () noexcept;

  BufferBinderRestore &operator= (const BufferBinderRestore &) = delete;
  BufferBinderRestore &operator= (BufferBinderRestore &&) = delete;

private:
  int mRestoreId;
};

template <Buffer::Type Type>
class BufferBinderUnsafe: public BufferBinderInterface<Type>
{
public:
  BufferBinderUnsafe () noexcept;
  BufferBinderUnsafe (const BufferImpl<Type> &buffer) noexcept;
  BufferBinderUnsafe (const BufferBinderUnsafe &) = delete;
  BufferBinderUnsafe (BufferBinderUnsafe &&) = delete;
  ~BufferBinderUnsafe () noexcept;

  BufferBinderUnsafe &operator= (const BufferBinderUnsafe &) = delete;
  BufferBinderUnsafe &operator= (BufferBinderUnsafe &&) = delete;
};

using VertexBuffer = BufferImpl<Buffer::Type::Vertex>;
using IndexBuffer = BufferImpl<Buffer::Type::Index>;
}
