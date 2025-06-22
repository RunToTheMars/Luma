#pragma once

namespace GL
{
class ResizeEvent
{
public:
  ResizeEvent (int oldWidth, int oldHeight, int width, int height)
    : m_oldWidth (oldWidth), m_oldHeight (oldHeight), m_width (width), m_height (height)
  {
  }
  ~ResizeEvent () = default;

  int oldWidth () const { return m_oldWidth; }
  int oldheight () const { return m_oldHeight; }

  int width () const { return m_width; }
  int height () const { return m_height; }

private:
  const int m_oldWidth;
  const int m_oldHeight;

  const int m_width;
  const int m_height;
};
}  // namespace GL
