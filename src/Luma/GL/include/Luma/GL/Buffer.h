#pragma once

namespace Luma::GL
{
class Buffer
{
public:
  enum class Type
  {
    Vertex = 0x8892, // GL_ARRAY_BUFFER
    Index  = 0x8893  // GL_ELEMENT_ARRAY_BUFFER
  };

  enum class UsagePattern
  {
    StreamDraw  = 0x88E0, // GL_STREAM_DRAW
    StaticDraw  = 0x88E4, // GL_STATIC_DRAW
    DynamicDraw = 0x88E8  // GL_DYNAMIC_DRAW
  };

  Buffer (Type type = Type::Vertex) noexcept;
  Buffer (const Buffer &) = delete;
  Buffer (Buffer &&) noexcept;
  ~Buffer () noexcept;

  Buffer &operator= (const Buffer &) noexcept;
  Buffer &operator= (Buffer &&) noexcept;

  void create () noexcept;
  bool isCreated () const noexcept;
  void destroy () noexcept;

  /// -------------------------------
  /// \note Only for Created
  void bind () const noexcept;
  void unbind () const noexcept;
  /// -------------------------------

  /// -------------------------------
  /// \note Only for Binded
  void allocate (int count, Buffer::UsagePattern usagePattern = Buffer::UsagePattern::StaticDraw) noexcept { allocate (nullptr, count, usagePattern); }
  void allocate (const void *data, int count, Buffer::UsagePattern usagePattern = Buffer::UsagePattern::StaticDraw) noexcept;
  void read (int offset, void *data, int count) const noexcept;
  void write (int offset, const void *data, int count) const noexcept;
  /// -------------------------------

  /// -------------------------------
  /// \note Only for allocated buffer
  Buffer::UsagePattern usagePattern () const noexcept;
  int count () const noexcept;
  /// -------------------------------

private:
  Type mType;
  unsigned int mId = 0;
  Buffer::UsagePattern mUsagePattern = Buffer::UsagePattern::StaticDraw;
  int mCount = 0;
};
}  // namespace GL
