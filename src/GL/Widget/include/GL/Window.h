#pragma once

#include "Common/Signal.h"
#include "Geometry/Size.h"
#include <memory>

namespace GL
{
class WindowCreateConfig;
class WindowImpl;
class EventCallback;
class Widget;

class Window
{
public:
  static Window &getInstance ();

  WindowCreateConfig create (const Geometry::Size &size, const char *title);

  void open (std::unique_ptr<GL::Widget> widget);
  void close ();
  GL::Widget *widget ();

  Geometry::Size size () const;

/// signals
public:
  Common::Signal<Geometry::Size> sizeChanged;

private:
  Window () noexcept;
  ~Window () noexcept;

  Window (const Window &) = delete;
  Window &operator= (const Window &) = delete;

  Window (Window &&) = delete;
  Window &operator= (Window &&) = delete;

private:
  friend class GL::WindowCreateConfig;
  friend class GL::EventCallback;

  std::unique_ptr<GL::Widget> mWidget;
  Geometry::Size mSize;
};

class WindowCreateConfig
{
public:
  ~WindowCreateConfig ();

  /// FrameBuffer
  WindowCreateConfig &setRedBits   (int val);
  WindowCreateConfig &setGreenBits (int val);
  WindowCreateConfig &setBlueBits  (int val);
  WindowCreateConfig &setAlphaBits (int val);

  /// Window
  WindowCreateConfig &setPositionX    (int val);
  WindowCreateConfig &setPositionY    (int val);
  WindowCreateConfig &setSamples      (int val);
  WindowCreateConfig &setRefreshRate  (int val);
  WindowCreateConfig &setDoubleBuffer (bool val);
  WindowCreateConfig &setResizable    (bool val);
  WindowCreateConfig &setDecorated    (bool val);
  WindowCreateConfig &setFocused      (bool val);
  WindowCreateConfig &setVisible      (bool val);
  WindowCreateConfig &setMaximized    (bool val);
  WindowCreateConfig &setAutoIconify  (bool val);
  WindowCreateConfig &setFloating     (bool val);

  /// Context
  enum class Profile
  {
    Any    = 0,          /* GLFW_OPENGL_ANY_PROFILE    */
    Core   = 0x00032001, /* GLFW_OPENGL_CORE_PROFILE   */
    Compat = 0x00032002  /* GLFW_OPENGL_COMPAT_PROFILE */
  };

  WindowCreateConfig &setOpenGLProfile (Profile profile);
  WindowCreateConfig &setVersionMajor (int val);
  WindowCreateConfig &setVersionMinor (int val);

private:
  friend class GL::Window;
  WindowCreateConfig (const Geometry::Size &size, const char *title);

private:
  Geometry::Size mSize = {800, 600};
  const char *mTitle = nullptr;
};
}  // namespace GL
