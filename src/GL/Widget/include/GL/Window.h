#pragma once

#include "GL/ResizeEvent.h"
#include "GL/Widget.h"
#include <memory>

namespace GL
{
class WindowCreateConfig;
class WindowImpl;
class EventCallback;
class Window: public GL::Widget
{
public:
  Window ();
  ~Window ();

  Window (const Window &) = delete;
  Window &operator= (const Window &) = delete;

  Window (Window &&) = delete;
  Window &operator= (Window &&) = delete;

  WindowCreateConfig create (const Geometry::Size &size, const char *title);
  void exec ();
  void close ();

private:
  virtual void init ();

protected:
  void resizeEvent (const ResizeEvent &event) override;
  void keyEvent (const KeyEvent &event) override;
  void mouseEvent (const MouseEvent &event) override;
  void renderEvent () override;

private:
  friend class GL::WindowCreateConfig;
  friend class GL::EventCallback;

  std::unique_ptr<WindowImpl> m_pimpl;
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
    ANY,
    CORE,
    COMPAT
  };

  WindowCreateConfig &setOpenGLProfile (Profile profile);
  WindowCreateConfig &setVersionMajor (int val);
  WindowCreateConfig &setVersionMinor (int val);

private:
  friend class GL::Window;

  WindowCreateConfig (GL::Window &window, const Geometry::Size &size, const char *title);

private:
  GL::Window &m_window;
  Geometry::Size m_size = {800, 600};
  const char *m_title = nullptr;
};
}  // namespace GL
