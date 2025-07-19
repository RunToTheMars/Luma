#include "Luma/Core/Window.h"
#include "ApplicationImpl.h"
#include "ImplCast.inl"
#include "Luma/Core/Application.h"
#include "Luma/Core/HoverEvent.h"
#include "Luma/Core/KeyEvent.h"
#include "Luma/Core/Monitor.h"
#include "Luma/Core/MouseEvent.h"
#include "Luma/Core/MoveEvent.h"
#include "Luma/Core/RenderEvent.h"
#include "Luma/Core/ResizeEvent.h"
#include <GLFW/glfw3.h>

namespace Luma::Core
{
class WindowEventDispatcher
{
public:
  template <typename TFunc, typename TFallbackFunc>
  static void notifyInputEvent (Window *GLwindow, const TFunc &func, const TFallbackFunc &fallbackFunc)
  {
    if (ApplicationImpl::allApplicationModalWindows.empty ()
        || GLwindow->mModality == Window::Modality::ApplicationModal)
      func ();
    else
      fallbackFunc ();
  }

  template<typename FUNC>
  static void notifyInputEvent (Window *GLwindow, const FUNC &func)
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
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->mPos = {xpos, ypos};
    GLwindow->moveEvent (GLwindow->mPos);
  }

  static void WindowSizeChangedHandle (GLFWwindow *window, int width, int height)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->mSize = {width, height};
    GLwindow->resizeEvent (ResizeEvent (GLwindow->mSize));
  }

  static void WindowClosedHandle (GLFWwindow *window)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    notifyInputEvent (GLwindow, [&] { GLwindow->closeEvent (); }, &requestAttentionToModal);
  }

  static void WindowRefreshHandle (GLFWwindow *window)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->update ();
  }

  static void WindowFocusChangedHandle (GLFWwindow *window, int focused)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->mFocused = focused == GLFW_TRUE;
    if (GLwindow->mFocused)
      {
        ApplicationImpl::focusWindow = GLwindow;
        GLwindow->focusInEvent ();
      }
    else
      {
        if (ApplicationImpl::focusWindow == GLwindow)
          ApplicationImpl::focusWindow = nullptr;

        GLwindow->focusOutEvent ();
      }
  }

  static void WindowIconifiedHandle (GLFWwindow *window, int iconified)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->mIconified = iconified == GLFW_TRUE;
  }

  static void WindowMaximizedHandle (GLFWwindow *window, int maximized)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->mMaximized = maximized == GLFW_TRUE;
  }

  static void WindowFramebufferSizeChangedHandle (GLFWwindow *window, int width, int height)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->mFrameBufferSize = {width, height};
  }

  static void WindowContentScaleChangedHandle (GLFWwindow *window, float xscale, float yscale)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    GLwindow->mContentScale = {xscale, yscale};
  }

  static void WindowKeyEventHandle (GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    notifyInputEvent (GLwindow, [&] { GLwindow->keyEvent (KeyEvent (static_cast<Key> (key), static_cast<KeyAction> (action))); });
  }

  static void WindowMouseButtonEventHandle (GLFWwindow *window, int button, int action, int mods)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    notifyInputEvent (GLwindow, [&] {
      GLwindow->mouseEvent (MouseEvent (static_cast<MouseButton> (button), static_cast<MouseButtonAction> (action)));
    });
  }

  static void WindowCursorPosChangedHandle (GLFWwindow *window, double xpos, double ypos)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    notifyInputEvent (GLwindow, [&] {
      GLwindow->mCursorPos = {xpos, ypos};
      GLwindow->hoverEvent (GLwindow->mCursorPos);
    });
  }

  static void WindowCursorEnterChangedHandle (GLFWwindow *window, int entered)
  {
    Window *GLwindow = toGLwindow (glfwGetWindowUserPointer (window));
    notifyInputEvent (GLwindow, [&] {
      GLwindow->mHovered = entered == GLFW_TRUE;
      if (GLwindow->mHovered)
        GLwindow->enterEvent ();
      else
        GLwindow->leaveEvent ();
    });
  }
};
}  // namespace Luma::Core

namespace
{
GLFWwindow *createWindowImpl (Luma::Core::Window *window, const Luma::Core::Vec2I &size, const char *title, GLFWmonitor *monitor, const Luma::Core::WindowHints &hints)
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

  GLFWwindow *impl = glfwCreateWindow (size[0], size[1], title, monitor, nullptr);

  glfwSetWindowUserPointer          (impl, window);
  glfwSetWindowPosCallback          (impl, &Luma::Core::WindowEventDispatcher::WindowPosChangedHandle);
  glfwSetWindowSizeCallback         (impl, &Luma::Core::WindowEventDispatcher::WindowSizeChangedHandle);
  glfwSetWindowCloseCallback        (impl, &Luma::Core::WindowEventDispatcher::WindowClosedHandle);
  glfwSetWindowRefreshCallback      (impl, &Luma::Core::WindowEventDispatcher::WindowRefreshHandle);
  glfwSetWindowFocusCallback        (impl, &Luma::Core::WindowEventDispatcher::WindowFocusChangedHandle);
  glfwSetWindowIconifyCallback      (impl, &Luma::Core::WindowEventDispatcher::WindowIconifiedHandle);
  glfwSetWindowMaximizeCallback     (impl, &Luma::Core::WindowEventDispatcher::WindowMaximizedHandle);
  glfwSetFramebufferSizeCallback    (impl, &Luma::Core::WindowEventDispatcher::WindowFramebufferSizeChangedHandle);
  glfwSetWindowContentScaleCallback (impl, &Luma::Core::WindowEventDispatcher::WindowContentScaleChangedHandle);
  glfwSetKeyCallback                (impl, &Luma::Core::WindowEventDispatcher::WindowKeyEventHandle);
  glfwSetMouseButtonCallback        (impl, &Luma::Core::WindowEventDispatcher::WindowMouseButtonEventHandle);
  glfwSetCursorPosCallback          (impl, &Luma::Core::WindowEventDispatcher::WindowCursorPosChangedHandle);
  glfwSetCursorEnterCallback        (impl, &Luma::Core::WindowEventDispatcher::WindowCursorEnterChangedHandle);

  glfwMakeContextCurrent (impl);
  glfwSwapInterval (0);

  Luma::Core::ApplicationImpl::allWindows.insert (window);

  return impl;
}

Luma::Core::WindowHints videoModeHints (const Luma::Core::VideoMode &videoMode)
{
  Luma::Core::WindowHints hints;
  hints.redBits = videoMode.redBits ();
  hints.greenBits = videoMode.greenBits ();
  hints.blueBits = videoMode.blueBits ();
  hints.refreshRate = videoMode.refreshRate ();
  return hints;
}

void initWindowParams (GLFWwindow *impl,
                       const Luma::Core::WindowHints &hints,
                       Luma::Core::Vec2I &pos,
                       Luma::Core::Vec2I &size,
                       Luma::Core::Vec2D &cusorPos,
                       Luma::Core::Vec2I &frameBufferSize,
                       Luma::Core::Vec2F &contentScale,
                       Luma::Core::Profile &profile,
                       int &contextVersionMajor,
                       int &contextVersionMinor,
                       bool &resizable,
                       bool &visible,
                       bool &decorated,
                       bool &focused,
                       bool &iconified,
                       bool &autoIconify,
                       bool &floating,
                       bool &maximized,
                       bool &focusOnShow,
                       bool &mousePassthrough,
                       bool &doublebuffer,
                       bool &transparentFrameBuffer,
                       bool &hovered,
                       float &opacity)
{
  glfwGetWindowPos (impl, &pos[0], &pos[1]);
  glfwGetWindowSize (impl, &size[0], &size[1]);
  glfwGetCursorPos (impl, &cusorPos[0], &cusorPos[1]);
  glfwGetFramebufferSize (impl, &frameBufferSize[0], &frameBufferSize[1]);
  glfwGetWindowContentScale (impl, &contentScale[0], &contentScale[1]);
  opacity          = glfwGetWindowOpacity (impl);
  iconified        = glfwGetWindowAttrib (impl, GLFW_ICONIFIED        ) == GLFW_TRUE;
  visible          = glfwGetWindowAttrib (impl, GLFW_VISIBLE          ) == GLFW_TRUE;
  focused          = glfwGetWindowAttrib (impl, GLFW_FOCUSED          ) == GLFW_TRUE;
  autoIconify      = glfwGetWindowAttrib (impl, GLFW_AUTO_ICONIFY     ) == GLFW_TRUE;
  resizable        = glfwGetWindowAttrib (impl, GLFW_RESIZABLE        ) == GLFW_TRUE;
  decorated        = glfwGetWindowAttrib (impl, GLFW_DECORATED        ) == GLFW_TRUE;
  floating         = glfwGetWindowAttrib (impl, GLFW_FLOATING         ) == GLFW_TRUE;
  maximized        = glfwGetWindowAttrib (impl, GLFW_MAXIMIZED        ) == GLFW_TRUE;
  focusOnShow      = glfwGetWindowAttrib (impl, GLFW_FOCUS_ON_SHOW    ) == GLFW_TRUE;
  mousePassthrough = glfwGetWindowAttrib (impl, GLFW_MOUSE_PASSTHROUGH) == GLFW_TRUE;
  hovered          = glfwGetWindowAttrib (impl, GLFW_HOVERED          ) == GLFW_TRUE;
  doublebuffer           = glfwGetWindowAttrib (impl, GLFW_DOUBLEBUFFER) == GLFW_TRUE;
  transparentFrameBuffer = glfwGetWindowAttrib (impl, GLFW_TRANSPARENT_FRAMEBUFFER) == GLFW_TRUE;

  profile                = hints.profile;
  contextVersionMajor    = hints.contextVersionMajor;
  contextVersionMinor    = hints.contextVersionMinor;
  //resizable              = hints.resizable;
  //visible                = hints.visible;
  //decorated              = hints.decorated;
  //focused                = hints.focused;
  //autoIconify            = hints.autoIconify;
  //floating               = hints.floating;
  //maximized              = hints.maximized;
  //focusOnShow            = hints.focusOnShow;
  //mousePassthrough       = hints.mousePassthrough;
  //doublebuffer           = hints.doublebuffer;
  //transparentFrameBuffer = hints.transparentFrameBuffer;
}
}

namespace Luma::Core
{
Window::Window (const Vec2I &size, const char *title, const WindowHints &hints, Object *parent): Object (parent)
{
  GLFWwindow *impl = createWindowImpl (this, size, title, nullptr, hints);
  mPimpl = impl;
  initWindowParams (impl, hints, mPos, mSize, mCursorPos, mFrameBufferSize, mContentScale,
                    mProfile, mContextVersionMajor, mContextVersionMinor,
                    mResizable, mVisible, mDecorated, mFocused, mIconified, mAutoIconify,
                    mFloating, mMaximized, mFocusOnShow, mMousePassthrough,
                    mDoublebuffer, mTransparentFrameBuffer, mHovered, mOpacity);
}

Window::Window (const Vec2I &resolution, const char *title, const Monitor &monitor, const WindowHints &hints, Object *parent): Object (parent)
{
  GLFWwindow *impl = createWindowImpl (this, resolution, title, static_cast<GLFWmonitor*> (monitor.mPimpl), hints);
  mPimpl = impl;
  initWindowParams (impl, hints, mPos, mSize, mCursorPos, mFrameBufferSize, mContentScale,
                    mProfile, mContextVersionMajor, mContextVersionMinor,
                    mResizable, mVisible, mDecorated, mFocused, mIconified, mAutoIconify,
                    mFloating, mMaximized, mFocusOnShow, mMousePassthrough,
                    mDoublebuffer, mTransparentFrameBuffer, mHovered, mOpacity);
}

Window::Window (const char *title, const Monitor &monitor, const VideoMode &videoMode,Object *parent): Object (parent)
{
  WindowHints hints = videoModeHints (videoMode);
  GLFWwindow *impl = createWindowImpl (this, videoMode.size (), title, static_cast<GLFWmonitor*> (monitor.mPimpl), hints);
  mPimpl = impl;
  initWindowParams (impl, hints, mPos, mSize, mCursorPos, mFrameBufferSize, mContentScale,
                    mProfile, mContextVersionMajor, mContextVersionMinor,
                    mResizable, mVisible, mDecorated, mFocused, mIconified, mAutoIconify,
                    mFloating, mMaximized, mFocusOnShow, mMousePassthrough,
                    mDoublebuffer, mTransparentFrameBuffer, mHovered, mOpacity);
}

Window::Window (const char *title, const Monitor &monitor, Object *parent): Window (title, monitor, monitor.videoMode (), parent)
{
}

Window::Window (const char *title, Object *parent): Window (title, Application::primaryMonitor (), parent)
{
}

Window::~Window ()
{
  destroy ();
}

bool Window::isDestroyed () const
{
  return !mPimpl;
}

void Window::destroy ()
{
  if (!isDestroyed ())
    {
      ApplicationImpl::allWindows.erase (this);
      glfwDestroyWindow (toGLFWwindow (mPimpl));
      mPimpl = nullptr;

      if (mModality == Modality::ApplicationModal)
        ApplicationImpl::allApplicationModalWindows.erase (this);
    }
}

void Window::update ()
{
  update (RectI {{0, 0}, size ()});
}

void Window::update (const RectI &rect)
{
  if (!mUpdateNeeded)
    {
      mUpdateNeeded = true;
      mUpdateRect = rect;
      ApplicationImpl::windowsForPostRepaint->push_back (this);
    }
  else
    {
      mUpdateRect.unit (rect);
    }
}

void Window::repaint ()
{
  repaint (RectI {{0, 0}, size ()});
}

void Window::repaint (const RectI &rect)
{
  GLFWwindow *impl = toGLFWwindow (mPimpl);
  glfwMakeContextCurrent (impl);
  renderEvent (RenderEvent (rect));
  glfwSwapBuffers (impl);
}

void Window::makeCurrent ()
{
  GLFWwindow *impl = toGLFWwindow (mPimpl);
  glfwMakeContextCurrent (impl);
}

Window::Modality Window::modality () const
{
  return mModality;
}

void Window::setModality (Modality modality)
{
  if (mModality == modality)
    return;

  Modality oldModality = mModality;
  mModality = modality;

  if (oldModality == Modality::ApplicationModal)
    ApplicationImpl::allApplicationModalWindows.erase (this);
  else if (modality == Modality::ApplicationModal)
    {
      ApplicationImpl::allApplicationModalWindows.insert (this);
      focus ();
      requestAttention ();
    }
}

bool Window::isWindowed () const
{
  GLFWmonitor *monitorImpl = glfwGetWindowMonitor (toGLFWwindow (mPimpl));
  return monitorImpl == nullptr;
}

bool Window::isFullScreen () const
{
  return !isWindowed ();
}

Monitor Window::monitor () const
{
  GLFWmonitor *monitorImpl = glfwGetWindowMonitor (toGLFWwindow (mPimpl));
  return Monitor (monitorImpl);
}

void Window::setMonitor (const Monitor &monitor, const Vec2I &resolution, std::optional<int> refreshRate)
{
  glfwSetWindowMonitor (toGLFWwindow (mPimpl), static_cast<GLFWmonitor*> (monitor.mPimpl), 0, 0, resolution[0], resolution[1], refreshRate.value_or (GL_DONT_CARE));
}

void Window::setMonitor (const Monitor &monitor, const VideoMode &videoMode)
{
  glfwSetWindowMonitor (toGLFWwindow (mPimpl), static_cast<GLFWmonitor*> (monitor.mPimpl), 0, 0, videoMode.width (), videoMode.height (), videoMode.refreshRate ());
}

void Window::setMonitor (const Monitor &monitor)
{
  setMonitor (monitor, monitor.videoMode ());
}

void Window::setWindowed (const Vec2I &pos, const Vec2I &size)
{
  glfwSetWindowMonitor (toGLFWwindow (mPimpl), nullptr, pos[0], pos[1], size[0], size[1], GL_DONT_CARE);
}

const char *Window::title () const
{
  return glfwGetWindowTitle (toGLFWwindow (mPimpl));
}

void Window::setTitle (const char *title)
{
  glfwSetWindowTitle (toGLFWwindow (mPimpl), title);
}

Vec2I Window::pos () const
{
  return mPos;
}

void Window::setPos (const Vec2I &pos)
{
  glfwSetWindowPos (toGLFWwindow (mPimpl), pos[0], pos[1]);
}

Vec2I Window::size () const
{
  return mSize;
}

void Window::setSize (const Vec2I &size)
{
  glfwSetWindowSize (toGLFWwindow (mPimpl), size[0], size[1]);
}

std::optional<Vec2I> Window::minSize () const
{
  return mMinSize;
}

std::optional<Vec2I> Window::maxSize () const
{
  return mMaxSize;
}

void Window::setSizeLimits (const std::optional<Vec2I> &minSize, const std::optional<Vec2I> &maxSize)
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
  glfwSetWindowSizeLimits (toGLFWwindow (mPimpl), minWidth, minHeight, maxWidth, maxHeight);
}

std::optional<Vec2I> Window::aspectRatio () const
{
  return mAspectRatio;
}

void Window::setAspectRatio (const std::optional<Vec2I> &ratio)
{
  mAspectRatio = ratio;
  if (ratio)
    glfwSetWindowAspectRatio (toGLFWwindow (mPimpl), ratio->at (0), ratio->at (1));
  else
    glfwSetWindowAspectRatio (toGLFWwindow (mPimpl), GLFW_DONT_CARE, GLFW_DONT_CARE);
}

Vec2I Window::frameBufferSize () const
{
  return mFrameBufferSize;
}

RectI Window::frameRect () const
{
  int left;
  int top;
  int right;
  int bottom;
  glfwGetWindowFrameSize (toGLFWwindow (mPimpl), &left, &top, &right, &bottom);
  return RectI {Vec2I {left, top}, Vec2I {right - left + 1, bottom - top + 1}};
}

Vec2F Window::contentScale () const
{
  return mContentScale;
}

float Window::opacity () const
{
  return mOpacity;
}

void Window::setOpacity (float opacity)
{
  mOpacity = opacity;
  glfwSetWindowOpacity (toGLFWwindow (mPimpl), opacity);
}

bool Window::isIconified () const
{
  return mIconified;
}

void Window::iconify ()
{
  glfwIconifyWindow (toGLFWwindow (mPimpl));
}

bool Window::isMaximized () const
{
  return mMaximized;
}

void Window::maximize ()
{
  glfwMaximizeWindow (toGLFWwindow (mPimpl));
}

void Window::restore ()
{
  glfwRestoreWindow (toGLFWwindow (mPimpl));
}

bool Window::isVisible () const
{
  return mVisible;
}

void Window::setVislble (bool visible)
{
  if (visible)
    show ();
  else
    hide ();
}

void Window::show ()
{
  glfwShowWindow (toGLFWwindow (mPimpl));
}

void Window::hide ()
{
  glfwHideWindow (toGLFWwindow (mPimpl));
}

void Window::requestAttention ()
{
  glfwRequestWindowAttention (toGLFWwindow (mPimpl));
}

bool Window::isFocused () const
{
  return mFocused;
}

void Window::focus ()
{
  glfwFocusWindow (toGLFWwindow (mPimpl));
}

bool Window::isAutoIconified () const
{
  return mIconified;
}

void Window::setAutoIconified (bool autoIconified)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_AUTO_ICONIFY, autoIconified ? GLFW_TRUE : GL_FALSE);
}

bool Window::isResizable () const
{
  return mResizable;
}

void Window::setResizable (bool resizable)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_RESIZABLE, resizable ? GLFW_TRUE : GL_FALSE);
}

bool Window::isDecorated () const
{
  return mDecorated;
}

void Window::setDecorated (bool decorated)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_DECORATED, decorated ? GLFW_TRUE : GL_FALSE);
}

bool Window::isFloating () const
{
  return mFloating;
}

void Window::setFloating (bool floating)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_FLOATING, floating ? GLFW_TRUE : GL_FALSE);
}

bool Window::isFocusedOnShow () const
{
  return mFocusOnShow;
}

void Window::setFocusedOnShow (bool focusedOnShow)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_FOCUS_ON_SHOW, focusedOnShow ? GLFW_TRUE : GL_FALSE);
}

bool Window::isMousePassthrough () const
{
  return mMousePassthrough;
}

void Window::setMousePassthrough (bool mousePassthrough)
{
  glfwSetWindowAttrib (toGLFWwindow (mPimpl), GLFW_MOUSE_PASSTHROUGH, mousePassthrough ? GLFW_TRUE : GL_FALSE);
}

bool Window::isHovered () const
{
  return mHovered;
}

bool Window::hasTransparentFramebuffer () const
{
  return mTransparentFrameBuffer;
}

bool Window::hasDoubleBuffer () const
{
  return mDoublebuffer;
}

Window::CursorInputMode Window::cursorInputMode () const
{
  return static_cast<Window::CursorInputMode> (glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_CURSOR));
}

void Window::setCursorInputMode (CursorInputMode mode)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_CURSOR, static_cast<int> (mode));
}

bool Window::stickKeysEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void Window::setStickKeysEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::stickMouseButtonsEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

void Window::setStickMouseButtonsEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::lockKeyModsEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_LOCK_KEY_MODS) == GLFW_TRUE;
}

void Window::setLockKeyModsEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_LOCK_KEY_MODS, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::rawMouseMotionEnabled () const
{
  return glfwGetInputMode (toGLFWwindow (mPimpl), GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE;
}

void Window::setRawMouseMotionEnabled (bool enabled)
{
  glfwSetInputMode (toGLFWwindow (mPimpl), GLFW_RAW_MOUSE_MOTION, enabled ? GLFW_TRUE : GL_FALSE);
}

bool Window::rawMouseMotionSupported () const
{
  return glfwRawMouseMotionSupported () == GLFW_TRUE;
}

Vec2D Window::cursorPos () const
{
  return mCursorPos;
}

void Window::setCursorPos (const Vec2D &pos)
{
  glfwSetCursorPos (toGLFWwindow (mPimpl), pos[0], pos[1]);
}

void Window::closeEvent ()
{
  destroy ();
}

void Window::focusInEvent ()
{
}

void Window::focusOutEvent ()
{
}

void Window::renderEvent (const RenderEvent &)
{
}

void Window::hoverEvent (const HoverEvent &)
{
}

void Window::moveEvent (const MoveEvent &)
{
}

void Window::resizeEvent (const ResizeEvent &)
{
}

void Window::keyEvent (const KeyEvent &)
{
}

void Window::mouseEvent (const MouseEvent &)
{
}

void Window::enterEvent ()
{
}

void Window::leaveEvent ()
{
}

}  // namespace GL
