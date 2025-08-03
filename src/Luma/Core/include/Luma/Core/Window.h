#pragma once

#include "Luma/Core/Object.h"
#include "Luma/Geom/Rect.h"
#include "Luma/Geom/UiRect.h"
#include "Luma/Geom/Vector.h"
#include <optional>

namespace Luma::Core
{
using namespace Luma::Geom;

class KeyEvent;
class CharEvent;
class HoverEvent;
class MouseEvent;
class ResizeEvent;
class MoveEvent;
class RenderEvent;
class ScaleEvent;
class ScrollEvent;
class Monitor;
class VideoMode;
class Application;
class WindowEventDispatcher;

enum class Profile
{
  Any = 0,            /* GLFW_OPENGL_ANY_PROFILE    */
  Core = 0x00032001,  /* GLFW_OPENGL_CORE_PROFILE   */
  Compat = 0x00032002 /* GLFW_OPENGL_COMPAT_PROFILE */
};

struct WindowHints
{
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

  std::optional<Vec2I> pos; /* GLFW_FOCUSED */

  std::optional<int> refreshRate; /* GLFW_FOCUSED */

  int redBits     = 8;  /* GLFW_FOCUSED */
  int greenBits   = 8;  /* GLFW_FOCUSED */
  int blueBits    = 8;  /* GLFW_FOCUSED */
  int alphaBits   = 8;  /* GLFW_FOCUSED */
  int depthBits   = 24; /* GLFW_FOCUSED */
  int stencilBits = 8;  /* GLFW_FOCUSED */
};

class Window: public Object
{
public:
  enum class Modality
  {
    NonModal,
    // WindowModal,
    ApplicationModal
  };

  Window (const Vec2I &size, const char *title, const WindowHints &hints = {}, Object *parent = nullptr);
  Window (const Vec2I &resolution, const char *title, const Monitor &monitor, const WindowHints &hints = {}, Object *parent = nullptr);
  Window (const char *title, const Monitor &monitor, const VideoMode &videoMode, Object *parent = nullptr);
  Window (const char *title, const Monitor &monitor, Object *parent = nullptr);
  Window (const char *title, Object *parent = nullptr);

  ~Window ();

  Window (const Window &) = delete;
  Window &operator= (const Window &) = delete;

  Window (Window &&) = delete;
  Window &operator= (Window &&) = delete;

  bool isWindowed () const;
  bool isFullScreen () const;
  Monitor monitor () const;
  void setMonitor (const Monitor &monitor, const Vec2I &resolution, std::optional<int> refreshRate = std::nullopt);
  void setMonitor (const Monitor &monitor, const VideoMode &videoMode);
  void setMonitor (const Monitor &monitor);
  void setWindowed (const Vec2I &pos, const Vec2I &size);

  bool isDestroyed () const;
  void destroy ();

  void update ();
  void update (const RectI &rect);
  void repaint ();
  void repaint (const RectI &rect);
  void makeCurrent ();

  Modality modality () const;
  void setModality (Modality modality);

  const char *title () const;
  void setTitle (const char *title);

  Vec2I pos () const;                /// For Windowed
  void setPos (const Vec2I &pos);    /// For Windowed

  Vec2I size () const;               /// For Windowed
  void setSize (const Vec2I &size);  /// For Windowed

  std::optional<Vec2I> minSize () const; /// For Windowed
  std::optional<Vec2I> maxSize () const; /// For Windowed
  void setSizeLimits (const std::optional<Vec2I> &minSize, const std::optional<Vec2I> &maxSize); /// For Windowed

  std::optional<Vec2I> aspectRatio () const;               /// For Windowed
  void setAspectRatio (const std::optional<Vec2I> &ratio); /// For Windowed

  Vec2I frameBufferSize () const;

  UiRectI frameUiRect () const;

  Vec2F contentScale () const;

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

  Vec2D cursorPos () const;
  void setCursorPos (const Vec2D &pos);

  const char *clipboard () const;
  void setClipboard (const char *data);

protected:
  /// Events
  virtual void closeEvent    ();
  virtual void focusInEvent  ();
  virtual void focusOutEvent ();
  virtual void renderEvent   (const RenderEvent &);
  virtual void moveEvent     (const MoveEvent &);
  virtual void resizeEvent   (const ResizeEvent &);
  virtual void hoverEvent    (const HoverEvent &);
  virtual void keyEvent      (const KeyEvent &);
  virtual void charEvent     (const CharEvent &);
  virtual void mouseEvent    (const MouseEvent &);
  virtual void scrollEvent   (const ScrollEvent &);
  virtual void enterEvent    ();
  virtual void leaveEvent    ();
  virtual void scaleEvent    (const ScaleEvent &);

private:
  friend class Luma::Core::Application;
  friend class Luma::Core::WindowEventDispatcher;

  void *mPimpl = nullptr;

  Vec2I mPos;
  Vec2I mSize;
  Vec2I mFrameBufferSize;
  Vec2F mContentScale;
  Vec2D mCursorPos;
  Profile mProfile;
  int mContextVersionMajor;
  int mContextVersionMinor;
  bool mResizable;
  bool mVisible;
  bool mDecorated;
  bool mFocused;
  bool mHovered;
  bool mIconified;
  bool mAutoIconify;
  bool mFloating;
  bool mMaximized;
  bool mFocusOnShow;
  bool mMousePassthrough;
  bool mDoublebuffer;
  bool mTransparentFrameBuffer;
  std::optional<Vec2I> mMinSize;
  std::optional<Vec2I> mMaxSize;
  std::optional<Vec2I> mAspectRatio;
  float mOpacity;

  bool mUpdateNeeded = false;
  RectI mUpdateRect;

  Modality mModality = Modality::NonModal;
};
}  // namespace GL
