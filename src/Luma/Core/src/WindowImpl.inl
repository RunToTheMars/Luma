#pragma once

#include "ApplicationImpl.h"
#include "ImplCast.inl"
#include "Luma/Core/Application.h"
#include "Luma/Core/CharEvent.h"
#include "Luma/Core/HoverEvent.h"
#include "Luma/Core/KeyEvent.h"
#include "Luma/Core/Monitor.h"
#include "Luma/Core/MouseEvent.h"
#include "Luma/Core/MoveEvent.h"
#include "Luma/Core/RenderEvent.h"
#include "Luma/Core/ResizeEvent.h"
#include "Luma/Core/ScaleEvent.h"
#include "Luma/Core/ScrollEvent.h"
#include "Luma/Core/Window.h"
#include <GLFW/glfw3.h>

namespace Luma::Core
{

class WindowEventDispatcher;

using namespace Luma::Geom;

class WindowImpl
{
  friend class Luma::Core::WindowEventDispatcher;

public:
  WindowImpl (Window &window, const Vec2I &size, const char *title, const WindowHints &hints);
  WindowImpl (Window &window, const Vec2I &resolution, const char *title, const Monitor &monitor, const WindowHints &hints);
  WindowImpl (Window &window, const char *title, const Monitor &monitor, const VideoMode &videoMode);
  WindowImpl (Window &window, const char *title, const Monitor &monitor);
  WindowImpl (Window &window, const char *title);

  void init (const Luma::Core::Vec2I &size, const char *title, GLFWmonitor *monitor, const Luma::Core::WindowHints &hints);

  ~WindowImpl ();

  WindowImpl (const WindowImpl &) = delete;
  WindowImpl &operator= (const WindowImpl &) = delete;

  WindowImpl (WindowImpl &&) = delete;
  WindowImpl &operator= (WindowImpl &&) = delete;

  bool isWindowed () const;
  bool isFullScreen () const;
  Monitor monitor () const;
  void setMonitor (const Monitor &monitor, const Vec2I &resolution, std::optional<int> refreshRate);
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

  Window::Modality modality () const;
  void setModality (Window::Modality modality);

  const char *title () const;
  void setTitle (const char *title);

  Vec2I pos () const;
  void setPos (const Vec2I &pos);

  Vec2I size () const;
  void setSize (const Vec2I &size);

  std::optional<Vec2I> minSize () const;
  std::optional<Vec2I> maxSize () const;
  void setSizeLimits (const std::optional<Vec2I> &minSize, const std::optional<Vec2I> &maxSize);

  std::optional<Vec2I> aspectRatio () const;
  void setAspectRatio (const std::optional<Vec2I> &ratio);

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
  bool isMaximized () const;
  void iconify ();
  void maximize ();
  void restore ();

  void requestAttention ();

  bool isFocused () const;
  void focus ();

  bool isDecorated () const;
  void setDecorated (bool decorated);

  bool isResizable () const;
  void setResizable (bool resizable);

  bool isFloating () const;
  void setFloating (bool floating);

  bool isAutoIconified () const;
  void setAutoIconified (bool autoIconified);

  bool isFocusedOnShow () const;
  void setFocusedOnShow (bool focusedOnShow);

  bool isMousePassthrough () const;
  void setMousePassthrough (bool mousePassthrough);

  bool isHovered () const;

  bool hasTransparentFramebuffer () const;

  bool hasDoubleBuffer () const;

  Window::CursorInputMode cursorInputMode () const;
  void setCursorInputMode (Window::CursorInputMode mode);

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

public:
  void closeEvent    ();
  void focusInEvent  ();
  void focusOutEvent ();
  void renderEvent   (const RenderEvent &);
  void moveEvent     (const MoveEvent &);
  void resizeEvent   (const ResizeEvent &);
  void hoverEvent    (const HoverEvent &);
  void keyEvent      (const KeyEvent &);
  void charEvent     (const CharEvent &);
  void mouseEvent    (const MouseEvent &);
  void scrollEvent   (const ScrollEvent &);
  void enterEvent    ();
  void leaveEvent    ();
  void scaleEvent    (const ScaleEvent &);

  void appRenderEvent ();

private:
  Window &mWindow;
  GLFWwindow *mPimpl = nullptr;

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

  Window::Modality mModality = Window::Modality::NonModal;
};

class WindowEventDispatcher
{
public:
  template <typename TFunc, typename TFallbackFunc>
  static void notifyInputEvent (WindowImpl *GLwindow, const TFunc &func, const TFallbackFunc &fallbackFunc)
  {
    if (ApplicationImpl::allApplicationModalWindows.empty ()
        || GLwindow->mModality == Window::Modality::ApplicationModal)
      func ();
    else
      fallbackFunc ();
  }

  template<typename FUNC>
  static void notifyInputEvent (WindowImpl *GLwindow, const FUNC &func)
  {
    notifyInputEvent (GLwindow, func, [] {});
  }

  static void requestAttentionToModal ()
  {
    for (Window *modal : ApplicationImpl::allApplicationModalWindows)
      modal->requestAttention ();
  }

  static void WindowPosChangedHandle (GLFWwindow *window, int xpos, int ypos)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->mPos = {xpos, ypos};
    windowImpl->mWindow.moveEvent (windowImpl->mPos);
  }

  static void WindowSizeChangedHandle (GLFWwindow *window, int width, int height)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->mSize = {width, height};
    windowImpl->mWindow.resizeEvent (ResizeEvent (windowImpl->mSize));
  }

  static void WindowClosedHandle (GLFWwindow *window)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    notifyInputEvent (windowImpl, [&] { windowImpl->mWindow.closeEvent (); }, &requestAttentionToModal);
  }

  static void WindowRefreshHandle (GLFWwindow *window)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->update ();
  }

  static void WindowFocusChangedHandle (GLFWwindow *window, int focused)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->mFocused = focused == GLFW_TRUE;
    if (windowImpl->mFocused)
      {
        ApplicationImpl::focusWindow = &windowImpl->mWindow;
        windowImpl->mWindow.focusInEvent ();
      }
    else
      {
        if (ApplicationImpl::focusWindow == &windowImpl->mWindow)
          ApplicationImpl::focusWindow = nullptr;

        windowImpl->mWindow.focusOutEvent ();
      }
  }

  static void WindowIconifiedHandle (GLFWwindow *window, int iconified)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->mIconified = iconified == GLFW_TRUE;
  }

  static void WindowMaximizedHandle (GLFWwindow *window, int maximized)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->mMaximized = maximized == GLFW_TRUE;
  }

  static void WindowFramebufferSizeChangedHandle (GLFWwindow *window, int width, int height)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->mFrameBufferSize = {width, height};
  }

  static void WindowContentScaleChangedHandle (GLFWwindow *window, float xscale, float yscale)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    windowImpl->mContentScale = {xscale, yscale};
    windowImpl->mWindow.scaleEvent (windowImpl->mContentScale);
  }

  static void WindowKeyEventHandle (GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    notifyInputEvent (windowImpl, [&] { windowImpl->mWindow.keyEvent (KeyEvent (static_cast<Key> (key), static_cast<KeyEvent::Action> (action), static_cast<KeyEvent::Mode> (mods))); });
  }

  static void WindowCharEventHandle (GLFWwindow *window, unsigned int codepoint)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    notifyInputEvent (windowImpl, [&] { windowImpl->mWindow.charEvent (CharEvent (codepoint)); });
  }

  static void WindowMouseButtonEventHandle (GLFWwindow *window, int button, int action, int mods)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    notifyInputEvent (windowImpl, [&] {
      windowImpl->mWindow.mouseEvent (MouseEvent (static_cast<MouseEvent::Button> (button), static_cast<MouseEvent::Action> (action)));
    });
  }

  static void WindowScrollEventHandle (GLFWwindow *window, double xoffset, double yoffset)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    notifyInputEvent (windowImpl, [&] {
      windowImpl->mWindow.scrollEvent (ScrollEvent ({xoffset, yoffset}));
    });
  }

  static void WindowCursorPosChangedHandle (GLFWwindow *window, double xpos, double ypos)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    notifyInputEvent (windowImpl, [&] {
      windowImpl->mCursorPos = {xpos, ypos};
      windowImpl->mWindow.hoverEvent (windowImpl->mCursorPos);
    });
  }

  static void WindowCursorEnterChangedHandle (GLFWwindow *window, int entered)
  {
    WindowImpl *windowImpl = static_cast<WindowImpl *> (glfwGetWindowUserPointer (window));
    notifyInputEvent (windowImpl, [&] {
      windowImpl->mHovered = entered == GLFW_TRUE;
      if (windowImpl->mHovered)
        windowImpl->mWindow.enterEvent ();
      else
        windowImpl->mWindow.leaveEvent ();
    });
  }
};
}  // namespace Luma::Core

namespace
{
Luma::Core::WindowHints videoModeHints (const Luma::Core::VideoMode &videoMode)
{
  Luma::Core::WindowHints hints;
  hints.redBits = videoMode.redBits ();
  hints.greenBits = videoMode.greenBits ();
  hints.blueBits = videoMode.blueBits ();
  hints.refreshRate = videoMode.refreshRate ();
  return hints;
}
}

namespace Luma::Core
{
WindowImpl::WindowImpl (Window &window, const Vec2I &size, const char *title, const WindowHints &hints): mWindow (window)
{
  init (size, title, nullptr, hints);
}

WindowImpl::WindowImpl (Window &window, const Vec2I &resolution, const char *title, const Monitor &monitor, const WindowHints &hints): mWindow (window)
{
  init (resolution, title, static_cast<GLFWmonitor*> (monitor.mPimpl), hints);
}

WindowImpl::WindowImpl (Window &window, const char *title, const Monitor &monitor, const VideoMode &videoMode): mWindow (window)
{
  init (videoMode.size (), title, static_cast<GLFWmonitor*> (monitor.mPimpl), videoModeHints (videoMode));
}

WindowImpl::WindowImpl (Window &window, const char *title, const Monitor &monitor): WindowImpl (window, title, monitor, monitor.videoMode ())
{
}

WindowImpl::WindowImpl (Window &window, const char *title): WindowImpl (window, title, Application::primaryMonitor ())
{
}

void WindowImpl::init (const Luma::Core::Vec2I &size, const char *title, GLFWmonitor *monitor, const Luma::Core::WindowHints &hints)
{
  glfwWindowHint (GLFW_OPENGL_PROFILE       , static_cast<int> (hints.profile));
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, hints.contextVersionMinor);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, hints.contextVersionMajor);
  glfwWindowHint (GLFW_REFRESH_RATE         , hints.refreshRate.value_or (GLFW_DONT_CARE));

  glfwWindowHint (GLFW_SAMPLES, hints.samples.value_or (GLFW_DONT_CARE));

  glfwWindowHint (GLFW_RESIZABLE              , hints.resizable        ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_DECORATED              , hints.decorated        ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_FOCUSED                , hints.focused          ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_AUTO_ICONIFY           , hints.autoIconify      ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_FLOATING               , hints.floating         ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_MAXIMIZED              , hints.maximized        ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_VISIBLE                , hints.visible          ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_CENTER_CURSOR          , hints.centerCursor     ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_FOCUS_ON_SHOW          , hints.focusOnShow      ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_MOUSE_PASSTHROUGH      , hints.mousePassthrough ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_DOUBLEBUFFER           , hints.doublebuffer     ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint (GLFW_TRANSPARENT_FRAMEBUFFER, hints.transparentFrameBuffer ? GLFW_TRUE : GLFW_FALSE);

  if (hints.pos.has_value ())
    {
      glfwWindowHint (GLFW_POSITION_X, hints.pos->at (0));
      glfwWindowHint (GLFW_POSITION_Y, hints.pos->at (1));
    }
  else
    {
      glfwWindowHint (GLFW_POSITION_X, GLFW_ANY_POSITION);
      glfwWindowHint (GLFW_POSITION_Y, GLFW_ANY_POSITION);
    }

  glfwWindowHint (GLFW_RED_BITS    , hints.redBits);
  glfwWindowHint (GLFW_GREEN_BITS  , hints.greenBits);
  glfwWindowHint (GLFW_BLUE_BITS   , hints.blueBits);
  glfwWindowHint (GLFW_ALPHA_BITS  , hints.alphaBits);
  glfwWindowHint (GLFW_DEPTH_BITS  , hints.depthBits);
  glfwWindowHint (GLFW_STENCIL_BITS, hints.stencilBits);

  mPimpl = glfwCreateWindow (size[0], size[1], title, monitor, nullptr);

  glfwSetWindowUserPointer          (mPimpl, this);
  glfwSetWindowPosCallback          (mPimpl, &Luma::Core::WindowEventDispatcher::WindowPosChangedHandle);
  glfwSetWindowSizeCallback         (mPimpl, &Luma::Core::WindowEventDispatcher::WindowSizeChangedHandle);
  glfwSetWindowCloseCallback        (mPimpl, &Luma::Core::WindowEventDispatcher::WindowClosedHandle);
  glfwSetWindowRefreshCallback      (mPimpl, &Luma::Core::WindowEventDispatcher::WindowRefreshHandle);
  glfwSetWindowFocusCallback        (mPimpl, &Luma::Core::WindowEventDispatcher::WindowFocusChangedHandle);
  glfwSetWindowIconifyCallback      (mPimpl, &Luma::Core::WindowEventDispatcher::WindowIconifiedHandle);
  glfwSetWindowMaximizeCallback     (mPimpl, &Luma::Core::WindowEventDispatcher::WindowMaximizedHandle);
  glfwSetFramebufferSizeCallback    (mPimpl, &Luma::Core::WindowEventDispatcher::WindowFramebufferSizeChangedHandle);
  glfwSetWindowContentScaleCallback (mPimpl, &Luma::Core::WindowEventDispatcher::WindowContentScaleChangedHandle);
  glfwSetKeyCallback                (mPimpl, &Luma::Core::WindowEventDispatcher::WindowKeyEventHandle);
  glfwSetCharCallback               (mPimpl, &Luma::Core::WindowEventDispatcher::WindowCharEventHandle);
  glfwSetMouseButtonCallback        (mPimpl, &Luma::Core::WindowEventDispatcher::WindowMouseButtonEventHandle);
  glfwSetScrollCallback             (mPimpl, &Luma::Core::WindowEventDispatcher::WindowScrollEventHandle);
  glfwSetCursorPosCallback          (mPimpl, &Luma::Core::WindowEventDispatcher::WindowCursorPosChangedHandle);
  glfwSetCursorEnterCallback        (mPimpl, &Luma::Core::WindowEventDispatcher::WindowCursorEnterChangedHandle);

  glfwMakeContextCurrent (mPimpl);
  glfwSwapInterval (0);

  Luma::Core::ApplicationImpl::allWindows.insert (&mWindow);

  glfwGetWindowPos (mPimpl, &mPos[0], &mPos[1]);
  glfwGetWindowSize (mPimpl, &mSize[0], &mSize[1]);
  glfwGetCursorPos (mPimpl, &mCursorPos[0], &mCursorPos[1]);
  glfwGetFramebufferSize (mPimpl, &mFrameBufferSize[0], &mFrameBufferSize[1]);
  glfwGetWindowContentScale (mPimpl, &mContentScale[0], &mContentScale[1]);
  mOpacity          = glfwGetWindowOpacity (mPimpl);
  mIconified        = glfwGetWindowAttrib (mPimpl, GLFW_ICONIFIED        ) == GLFW_TRUE;
  mVisible          = glfwGetWindowAttrib (mPimpl, GLFW_VISIBLE          ) == GLFW_TRUE;
  mFocused          = glfwGetWindowAttrib (mPimpl, GLFW_FOCUSED          ) == GLFW_TRUE;
  mAutoIconify      = glfwGetWindowAttrib (mPimpl, GLFW_AUTO_ICONIFY     ) == GLFW_TRUE;
  mResizable        = glfwGetWindowAttrib (mPimpl, GLFW_RESIZABLE        ) == GLFW_TRUE;
  mDecorated        = glfwGetWindowAttrib (mPimpl, GLFW_DECORATED        ) == GLFW_TRUE;
  mFloating         = glfwGetWindowAttrib (mPimpl, GLFW_FLOATING         ) == GLFW_TRUE;
  mMaximized        = glfwGetWindowAttrib (mPimpl, GLFW_MAXIMIZED        ) == GLFW_TRUE;
  mFocusOnShow      = glfwGetWindowAttrib (mPimpl, GLFW_FOCUS_ON_SHOW    ) == GLFW_TRUE;
  mMousePassthrough = glfwGetWindowAttrib (mPimpl, GLFW_MOUSE_PASSTHROUGH) == GLFW_TRUE;
  mHovered          = glfwGetWindowAttrib (mPimpl, GLFW_HOVERED          ) == GLFW_TRUE;
  mDoublebuffer           = glfwGetWindowAttrib (mPimpl, GLFW_DOUBLEBUFFER) == GLFW_TRUE;
  mTransparentFrameBuffer = glfwGetWindowAttrib (mPimpl, GLFW_TRANSPARENT_FRAMEBUFFER) == GLFW_TRUE;

  mProfile                = hints.profile;
  mContextVersionMajor    = hints.contextVersionMajor;
  mContextVersionMinor    = hints.contextVersionMinor;

  //mResizable              = hints.resizable;
  //mVisible                = hints.visible;
  //mDecorated              = hints.decorated;
  //mFocused                = hints.focused;
  //mAutoIconify            = hints.autoIconify;
  //mFloating               = hints.floating;
  //mMaximized              = hints.maximized;
  //mFocusOnShow            = hints.focusOnShow;
  //mMousePassthrough       = hints.mousePassthrough;
  //mDoublebuffer           = hints.doublebuffer;
  //mTransparentFrameBuffer = hints.transparentFrameBuffer;
}

WindowImpl::~WindowImpl ()
{
  destroy ();
}

bool WindowImpl::isDestroyed () const
{
  return !mPimpl;
}

void WindowImpl::destroy ()
{
  if (!isDestroyed ())
    {
      ApplicationImpl::allWindows.erase (&mWindow);
      glfwDestroyWindow (mPimpl);
      mPimpl = nullptr;

      if (mModality == Window::Modality::ApplicationModal)
        ApplicationImpl::allApplicationModalWindows.erase (&mWindow);
    }
}

void WindowImpl::update ()
{
  update (RectI {{0, 0}, size ()});
}

void WindowImpl::update (const RectI &rect)
{
  if (!mUpdateNeeded)
    {
      mUpdateNeeded = true;
      mUpdateRect = rect;
      ApplicationImpl::windowsForPostRepaint->push_back (&mWindow);
    }
  else
    {
      mUpdateRect.unit (rect);
    }
}

void WindowImpl::repaint ()
{
  repaint (RectI {{0, 0}, size ()});
}

void WindowImpl::repaint (const RectI &rect)
{
  GLFWwindow *impl = mPimpl;
  glfwMakeContextCurrent (impl);
  mWindow.renderEvent (RenderEvent (rect));
  glfwSwapBuffers (impl);
}

void WindowImpl::makeCurrent ()
{
  GLFWwindow *impl = mPimpl;
  glfwMakeContextCurrent (impl);
}

Window::Modality WindowImpl::modality () const
{
  return mModality;
}

void WindowImpl::setModality (Window::Modality modality)
{
  if (mModality == modality)
    return;

  Window::Modality oldModality = mModality;
  mModality = modality;

  if (oldModality == Window::Modality::ApplicationModal)
    ApplicationImpl::allApplicationModalWindows.erase (&mWindow);
  else if (modality == Window::Modality::ApplicationModal)
    {
      ApplicationImpl::allApplicationModalWindows.insert (&mWindow);
      focus ();
      requestAttention ();
    }
}

bool WindowImpl::isWindowed () const
{
  GLFWmonitor *monitorImpl = glfwGetWindowMonitor (mPimpl);
  return monitorImpl == nullptr;
}

bool WindowImpl::isFullScreen () const
{
  return !isWindowed ();
}

Monitor WindowImpl::monitor () const
{
  GLFWmonitor *monitorImpl = glfwGetWindowMonitor (mPimpl);
  return Monitor (monitorImpl);
}

void WindowImpl::setMonitor (const Monitor &monitor, const Vec2I &resolution, std::optional<int> refreshRate)
{
  glfwSetWindowMonitor (mPimpl, static_cast<GLFWmonitor*> (monitor.mPimpl), 0, 0, resolution[0], resolution[1], refreshRate.value_or (GL_DONT_CARE));
}

void WindowImpl::setMonitor (const Monitor &monitor, const VideoMode &videoMode)
{
  glfwSetWindowMonitor (mPimpl, static_cast<GLFWmonitor*> (monitor.mPimpl), 0, 0, videoMode.width (), videoMode.height (), videoMode.refreshRate ());
}

void WindowImpl::setMonitor (const Monitor &monitor)
{
  setMonitor (monitor, monitor.videoMode ());
}

void WindowImpl::setWindowed (const Vec2I &pos, const Vec2I &size)
{
  glfwSetWindowMonitor (mPimpl, nullptr, pos[0], pos[1], size[0], size[1], GL_DONT_CARE);
}

const char *WindowImpl::title () const
{
  return glfwGetWindowTitle (mPimpl);
}

void WindowImpl::setTitle (const char *title)
{
  glfwSetWindowTitle (mPimpl, title);
}

Vec2I WindowImpl::pos () const
{
  return mPos;
}

void WindowImpl::setPos (const Vec2I &pos)
{
  glfwSetWindowPos (mPimpl, pos[0], pos[1]);
}

Vec2I WindowImpl::size () const
{
  return mSize;
}

void WindowImpl::setSize (const Vec2I &size)
{
  glfwSetWindowSize (mPimpl, size[0], size[1]);
}

std::optional<Vec2I> WindowImpl::minSize () const
{
  return mMinSize;
}

std::optional<Vec2I> WindowImpl::maxSize () const
{
  return mMaxSize;
}

void WindowImpl::setSizeLimits (const std::optional<Vec2I> &minSize, const std::optional<Vec2I> &maxSize)
{
  int minWidth;
  int minHeight;
  int maxWidth;
  int maxHeight;

  if (minSize)
    {
      minWidth = minSize->at (0);
      minHeight = minSize->at (1);
    }
  else
    {
      minWidth = minHeight = GLFW_DONT_CARE;
    }

  if (maxSize)
    {
      maxWidth = maxSize->at (0);
      maxHeight = maxSize->at (1);
    }
  else
    {
      maxWidth = maxHeight = GLFW_DONT_CARE;
    }

  mMinSize = minSize;
  mMaxSize = maxSize;
  glfwSetWindowSizeLimits (mPimpl, minWidth, minHeight, maxWidth, maxHeight);
}

std::optional<Vec2I> WindowImpl::aspectRatio () const
{
  return mAspectRatio;
}

void WindowImpl::setAspectRatio (const std::optional<Vec2I> &ratio)
{
  mAspectRatio = ratio;
  if (ratio)
    glfwSetWindowAspectRatio (mPimpl, ratio->at (0), ratio->at (1));
  else
    glfwSetWindowAspectRatio (mPimpl, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

Vec2I WindowImpl::frameBufferSize () const
{
  return mFrameBufferSize;
}

UiRectI WindowImpl::frameUiRect () const
{
  int left;
  int top;
  int right;
  int bottom;
  glfwGetWindowFrameSize (mPimpl, &left, &top, &right, &bottom);
  return UiRectI {Vec2I {left, top}, Vec2I {right - left + 1, bottom - top + 1}};
}

Vec2F WindowImpl::contentScale () const
{
  return mContentScale;
}

float WindowImpl::opacity () const
{
  return mOpacity;
}

void WindowImpl::setOpacity (float opacity)
{
  mOpacity = opacity;
  glfwSetWindowOpacity (mPimpl, opacity);
}

bool WindowImpl::isIconified () const
{
  return mIconified;
}

void WindowImpl::iconify ()
{
  glfwIconifyWindow (mPimpl);
}

bool WindowImpl::isMaximized () const
{
  return mMaximized;
}

void WindowImpl::maximize ()
{
  glfwMaximizeWindow (mPimpl);
}

void WindowImpl::restore ()
{
  glfwRestoreWindow (mPimpl);
}

bool WindowImpl::isVisible () const
{
  return mVisible;
}

void WindowImpl::setVislble (bool visible)
{
  if (visible)
    show ();
  else
    hide ();
}

void WindowImpl::show ()
{
  glfwShowWindow (mPimpl);
}

void WindowImpl::hide ()
{
  glfwHideWindow (mPimpl);
}

void WindowImpl::requestAttention ()
{
  glfwRequestWindowAttention (mPimpl);
}

bool WindowImpl::isFocused () const
{
  return mFocused;
}

void WindowImpl::focus ()
{
  glfwFocusWindow (mPimpl);
}

bool WindowImpl::isAutoIconified () const
{
  return mIconified;
}

void WindowImpl::setAutoIconified (bool autoIconified)
{
  glfwSetWindowAttrib (mPimpl, GLFW_AUTO_ICONIFY, autoIconified ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::isResizable () const
{
  return mResizable;
}

void WindowImpl::setResizable (bool resizable)
{
  glfwSetWindowAttrib (mPimpl, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::isDecorated () const
{
  return mDecorated;
}

void WindowImpl::setDecorated (bool decorated)
{
  glfwSetWindowAttrib (mPimpl, GLFW_DECORATED, decorated ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::isFloating () const
{
  return mFloating;
}

void WindowImpl::setFloating (bool floating)
{
  glfwSetWindowAttrib (mPimpl, GLFW_FLOATING, floating ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::isFocusedOnShow () const
{
  return mFocusOnShow;
}

void WindowImpl::setFocusedOnShow (bool focusedOnShow)
{
  glfwSetWindowAttrib (mPimpl, GLFW_FOCUS_ON_SHOW, focusedOnShow ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::isMousePassthrough () const
{
  return mMousePassthrough;
}

void WindowImpl::setMousePassthrough (bool mousePassthrough)
{
  glfwSetWindowAttrib (mPimpl, GLFW_MOUSE_PASSTHROUGH, mousePassthrough ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::isHovered () const
{
  return mHovered;
}

bool WindowImpl::hasTransparentFramebuffer () const
{
  return mTransparentFrameBuffer;
}

bool WindowImpl::hasDoubleBuffer () const
{
  return mDoublebuffer;
}

Window::CursorInputMode WindowImpl::cursorInputMode () const
{
  return static_cast<Window::CursorInputMode> (glfwGetInputMode (mPimpl, GLFW_CURSOR));
}

void WindowImpl::setCursorInputMode (Window::CursorInputMode mode)
{
  glfwSetInputMode (mPimpl, GLFW_CURSOR, static_cast<int> (mode));
}

bool WindowImpl::stickKeysEnabled () const
{
  return glfwGetInputMode (mPimpl, GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void WindowImpl::setStickKeysEnabled (bool enabled)
{
  glfwSetInputMode (mPimpl, GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::stickMouseButtonsEnabled () const
{
  return glfwGetInputMode (mPimpl, GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

void WindowImpl::setStickMouseButtonsEnabled (bool enabled)
{
  glfwSetInputMode (mPimpl, GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::lockKeyModsEnabled () const
{
  return glfwGetInputMode (mPimpl, GLFW_LOCK_KEY_MODS) == GLFW_TRUE;
}

void WindowImpl::setLockKeyModsEnabled (bool enabled)
{
  glfwSetInputMode (mPimpl, GLFW_LOCK_KEY_MODS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::rawMouseMotionEnabled () const
{
  return glfwGetInputMode (mPimpl, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE;
}

void WindowImpl::setRawMouseMotionEnabled (bool enabled)
{
  glfwSetInputMode (mPimpl, GLFW_RAW_MOUSE_MOTION, enabled ? GLFW_TRUE : GL_FALSE);
}

bool WindowImpl::rawMouseMotionSupported () const
{
  return glfwRawMouseMotionSupported () == GLFW_TRUE;
}

Vec2D WindowImpl::cursorPos () const
{
  return mCursorPos;
}

void WindowImpl::setCursorPos (const Vec2D &pos)
{
  glfwSetCursorPos (mPimpl, pos[0], pos[1]);
}

const char *WindowImpl::clipboard () const
{
  return glfwGetClipboardString (mPimpl);
}

void WindowImpl::setClipboard (const char *data)
{
  return glfwSetClipboardString (mPimpl, data);
}

void WindowImpl::closeEvent ()
{
  destroy ();
}

void WindowImpl::focusInEvent ()
{
}

void WindowImpl::focusOutEvent ()
{
}

void WindowImpl::renderEvent (const RenderEvent &)
{
}

void WindowImpl::hoverEvent (const HoverEvent &)
{
}

void WindowImpl::moveEvent (const MoveEvent &)
{
}

void WindowImpl::resizeEvent (const ResizeEvent &)
{
}

void WindowImpl::keyEvent (const KeyEvent &)
{
}

void WindowImpl::charEvent (const CharEvent &)
{
}

void WindowImpl::mouseEvent (const MouseEvent &)
{
}

void WindowImpl::scrollEvent (const ScrollEvent &)
{
}

void WindowImpl::enterEvent ()
{
}

void WindowImpl::leaveEvent ()
{
}

void WindowImpl::scaleEvent (const ScaleEvent &)
{
}

void WindowImpl::appRenderEvent ()
{
  if (!isVisible () || isIconified () || !mUpdateNeeded)
    return;

  mUpdateNeeded = false;
  repaint (mUpdateRect);
}

}  // namespace Luma::Core
