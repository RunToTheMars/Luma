#pragma once

namespace Luma::GL
{
class VertexArray
{
public:
  VertexArray () noexcept;
  VertexArray (const VertexArray &) = delete;
  VertexArray (VertexArray &&) noexcept;
  ~VertexArray () noexcept;

  VertexArray &operator= (const VertexArray &) = delete;
  VertexArray &operator= (VertexArray &&) noexcept;

  void create () noexcept;
  bool isCreated () const noexcept;
  void destroy () noexcept;

  /// -------------------------------
  /// \note Only for Created
  void bind () const noexcept;
  void unbind () const noexcept;
  /// -------------------------------

private:
  unsigned int mId = 0;
};
}  // namespace GL
