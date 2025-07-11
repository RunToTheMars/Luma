#pragma once

#include "Common/Signal.h"
#include "GL/KeyEvent.h"
#include "GL/MouseButtonEvent.h"
#include "Geometry/Rect.h"
#include "Geometry/Vector.h"
#include <optional>

namespace Application
{
void exec ();
}

namespace GL
{
class Monitor;
class VideoMode;
class Application;
class WindowEventDispatcher;

struct WindowHints
{
  enum class Profile
  {
    Any = 0,            /* GLFW_OPENGL_ANY_PROFILE    */
    Core = 0x00032001,  /* GLFW_OPENGL_CORE_PROFILE   */
    Compat = 0x00032002 /* GLFW_OPENGL_COMPAT_PROFILE */
  };

  Profile profile = Profile::Core; /* GLFW_OPENGL_PROFILE        */
  int contextVersionMajor = 3;     /* GLFW_CONTEXT_VERSION_MAJOR */
  int contextVersionMinor = 3;     /* GLFW_CONTEXT_VERSION_MINOR */

  std::optional<int> samples = 0;  /* GLFW_SAMPLES */

  bool resizable         = true;   /* GLFW_RESIZABLE         */
  bool visible           = true;   /* GLFW_VISIBLE           */
  bool decorated         = true;   /* GLFW_DECORATED         */
  bool focused           = true;   /* GLFW_FOCUSED           */
  bool autoIconify       = true;   /* GLFW_AUTO_ICONIFY      */
  bool floating          = false;  /* GLFW_FLOATING          */
  bool maximized         = false;  /* GLFW_MAXIMIZED         */
  bool centerCursor      = true;   /* GLFW_CENTER_CURSOR     */
  bool focusOnShow       = true;   /* GLFW_FOCUS_ON_SHOW     */
  bool mousePassthrough  = false;  /* GLFW_MOUSE_PASSTHROUGH */
  bool doublebuffer      = true;   /* GLFW_DOUBLEBUFFER      */
  bool transparentFrameBuffer = false; /* GLFW_TRANSPARENT_FRAMEBUFFER */

  std::optional<Geom::Vec2I> pos; /* GLFW_FOCUSED */

  std::optional<int> refreshRate; /* GLFW_FOCUSED */

  int redBits     = 8;  /* GLFW_FOCUSED */
  int greenBits   = 8;  /* GLFW_FOCUSED */
  int blueBits    = 8;  /* GLFW_FOCUSED */
  int alphaBits   = 8;  /* GLFW_FOCUSED */
  int depthBits   = 24; /* GLFW_FOCUSED */
  int stencilBits = 8;  /* GLFW_FOCUSED */
};

class Window
{
public:
  enum class Modality
  {
    NonModal,
    // WindowModal,
    ApplicationModal
  };

  Window (const Geom::Vec2I &size, const char *title, const WindowHints &hints = {}, Window *parent = nullptr) noexcept;
  Window (const Geom::Vec2I &resolution, const char *title, const Monitor &monitor,  const WindowHints &hints = {}, Window *parent = nullptr) noexcept;
  Window (const char *title, const Monitor &monitor, const GL::VideoMode &videoMode, Window *parent = nullptr) noexcept;
  Window (const char *title, const Monitor &monitor, Window *parent = nullptr) noexcept;

  ~Window () noexcept;

  Window (const Window &) = delete;
  Window &operator= (const Window &) = delete;

  Window (Window &&) = delete;
  Window &operator= (Window &&) = delete;

  Window *parent () const;
  void setParent (Window *parent);

  bool isWindowed () const;
  bool isFullScreen () const;
  Monitor monitor () const;
  void setMonitor (const Monitor &monitor, const Geom::Vec2I &resolution, std::optional<int> refreshRate = std::nullopt);
  void setMonitor (const Monitor &monitor, const GL::VideoMode &videoMode);
  void setMonitor (const Monitor &monitor);
  void setWindowed (const Geom::Vec2I &pos, const Geom::Vec2I &size);

  bool isDestroyed () const;
  void destroy ();

  void update ();
  void repaint ();

  Modality modality () const;
  void setModality (Modality modality);

  const char *title () const;
  void setTitle (const char *title);

  Geom::Vec2I pos () const;                /// For Windowed
  void setPos (const Geom::Vec2I &pos);    /// For Windowed

  Geom::Vec2I size () const;               /// For Windowed
  void setSize (const Geom::Vec2I &size);  /// For Windowed

  std::optional<Geom::Vec2I> minSize () const; /// For Windowed
  std::optional<Geom::Vec2I> maxSize () const; /// For Windowed
  void setSizeLimits (const std::optional<Geom::Vec2I> &minSize, const std::optional<Geom::Vec2I> &maxSize); /// For Windowed

  std::optional<Geom::Vec2I> aspectRatio () const;               /// For Windowed
  void setAspectRatio (const std::optional<Geom::Vec2I> &ratio); /// For Windowed

  Geom::Vec2I frameBufferSize () const;

  Geom::RectI frameRect () const;

  Geom::Vec2F contentScale () const;

  float opacity () const;
  void setOpacity (float opacity);

  bool isVisible () const;
  void setVislble (bool visible);
  void show ();
  void hide ();

  bool isIconified () const;
  bool isMaximized () const; /// For Windowed
  void iconify ();
  void maximize ();          /// For Windowed
  void restore ();           /// For Iconified || Maximized

  void requestAttention ();

  bool isFocused () const;
  void focus ();

  bool isDecorated () const;           /// For Windowed
  void setDecorated (bool decorated);  /// For Windowed

  bool isResizable () const;          /// For Windowed && Decorated
  void setResizable (bool resizable); /// For Windowed && Decorated

  bool isFloating () const;         /// For Windowed
  void setFloating (bool floating); /// For Windowed

  bool isAutoIconified () const;
  void setAutoIconified (bool autoIconified);

  bool isFocusedOnShow () const;
  void setFocusedOnShow (bool focusedOnShow);

  bool isMousePassthrough () const;
  void setMousePassthrough (bool mousePassthrough);

  bool isHovered () const;

  bool hasTransparentFramebuffer () const;

  bool hasDoubleBuffer () const;

  enum class CursorInputMode
  {
    Normal   = 0x00034001, /* GLFW_CURSOR_NORMAL   */
    Hidden   = 0x00034002, /* GLFW_CURSOR_HIDDEN   */
    Disabled = 0x00034003, /* GLFW_CURSOR_DISABLED */
    Captured = 0x00034004  /* GLFW_CURSOR_CAPTURED */
  };

  CursorInputMode cursorInputMode () const;
  void setCursorInputMode (CursorInputMode mode);

  bool stickKeysEnabled () const;
  void setStickKeysEnabled (bool enabled);

  bool stickMouseButtonsEnabled () const;
  void setStickMouseButtonsEnabled (bool enabled);

  bool lockKeyModsEnabled () const;
  void setLockKeyModsEnabled (bool enabled);

  bool rawMouseMotionEnabled () const;
  void setRawMouseMotionEnabled (bool enabled);
  bool rawMouseMotionSupported () const;

  GL::MouseButtonAction lastMouseButtonAction (GL::MouseButton button) const;

  Geom::Vec2D cursorPos () const;
  void setCursorPos (const Geom::Vec2D &pos);

public:
  /// Events
  Common::Signal<> closeEvent;
  Common::Signal<Geom::Vec2I> framebufferResizeEvent;
  Common::Signal<float, float> scaleEvent;
  Common::Signal<bool> iconifyEvent;
  Common::Signal<bool> maximizeEvent;
  Common::Signal<bool> focusEvent;
  Common::Signal<Geom::Vec2I> moveEvent;
  Common::Signal<Geom::Vec2I> resizeEvent;
  Common::Signal<GL::KeyEvent> keyEvent;
  Common::Signal<GL::MouseButtonEvent> mouseButtonEvent;
  Common::Signal<Geom::Vec2D> hoverEvent;
  Common::Signal<> enterEvent;
  Common::Signal<> leaveEvent;
  Common::Signal<Geom::Vec2D> scrollEvent;
  Common::Signal<> renderEvent;

  Common::Signal<> destroyed;

private:
  friend class GL::Application;
  friend class GL::WindowEventDispatcher;

  std::optional<Geom::Vec2I> mMinSize;
  std::optional<Geom::Vec2I> mMaxSize;
  std::optional<Geom::Vec2I> mAspectRatio;

  void *mPimpl = nullptr;

  bool mUpdateNeeded = true;

  Window *mParent = nullptr;
  Modality mModality = Modality::NonModal;

  Common::Slots mParentSlots;
};
}  // namespace GL
