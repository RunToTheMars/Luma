#pragma once

#include "GL/ResizeEvent.h"
#include "GL/KeyEvent.h"
#include "GL/MouseEvent.h"
#include <memory>
#include <QWidget>

namespace GL
{
class WidgetImpl;
class Widget
{
public:
  Widget ();
  ~Widget ();

  int x () const;
  int y () const;
  int width () const;
  int height () const;

private:
  virtual void resizeEvent (const ResizeEvent &event);
  virtual void keyEvent (const KeyEvent &event);
  virtual void mouseEvent (const MouseEvent &event);
  virtual void renderEvent ();

private:
  std::unique_ptr<WidgetImpl> m_pimpl;
};
}

namespace GL
{
class WindowCreateConfig;
class Window
{
public:
  Window ();
  virtual ~Window ();

  Window (const Window &) = delete;
  Window &operator= (const Window &) = delete;

  Window (Window &&) = delete;
  Window &operator= (Window &&) = delete;

  WindowCreateConfig create (int width, int height, const char *title);
  void exec ();
  void close ();

  int width () const;
  int height () const;

public:
  virtual void resizeEvent (const ResizeEvent &event);
  virtual void keyEvent (const KeyEvent &event);
  virtual void mouseEvent (const MouseEvent &event);

private:
  virtual void init ();
  virtual void renderEvent ();
};

class WindowCreateConfig
{
public:
  WindowCreateConfig (int width, int height, const char *title);
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
  int m_width = 800;
  int m_height = 600;
  const char *m_title = nullptr;
};
}  // namespace GL
