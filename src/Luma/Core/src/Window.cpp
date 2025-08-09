#include "Luma/Core/Window.h"
#include "WindowImpl.inl"

namespace Luma::Core
{
Window::Window (const Vec2I &size, const char *title, const WindowHints &hints, Object *parent):
    Object (parent),
    mPimpl (std::make_unique <WindowImpl> (*this, size, title, hints))
{
}

Window::Window (const Vec2I &resolution, const char *title, const Monitor &monitor, const WindowHints &hints, Object *parent):
    Object (parent),
    mPimpl (std::make_unique <WindowImpl> (*this, resolution, title, monitor, hints))
{
}

Window::Window (const char *title, const Monitor &monitor, const VideoMode &videoMode, Object *parent):
    Object (parent),
    mPimpl (std::make_unique <WindowImpl> (*this, title, monitor, videoMode))
{
}

Window::Window (const char *title, const Monitor &monitor, Object *parent): Object (parent), mPimpl (std::make_unique <WindowImpl> (*this, title, monitor))
{
}

Window::Window (const char *title, Object *parent): Object (parent), mPimpl (std::make_unique <WindowImpl> (*this, title))
{
}

Window::~Window () = default;

bool Window::isWindowed () const
{
  return mPimpl->isWindowed ();
}

bool Window::isFullScreen () const
{
  return mPimpl->isFullScreen ();
}

Monitor Window::monitor () const
{
  return mPimpl->monitor ();
}

void Window::setMonitor (const Monitor &monitor, const Vec2I &resolution, std::optional<int> refreshRate)
{
  mPimpl->setMonitor (monitor, resolution, refreshRate);
}

void Window::setMonitor (const Monitor &monitor, const VideoMode &videoMode)
{
  mPimpl->setMonitor (monitor, videoMode);
}

void Window::setMonitor (const Monitor &monitor)
{
  mPimpl->setMonitor (monitor);
}

void Window::setWindowed (const Vec2I &pos, const Vec2I &size)
{
  mPimpl->setWindowed (pos, size);
}

bool Window::isDestroyed () const
{
  return mPimpl->isDestroyed ();
}

void Window::destroy ()
{
  mPimpl->destroy ();
}

void Window::update ()
{
  mPimpl->update ();
}

void Window::update (const RectI &rect)
{
  mPimpl->update (rect);
}

void Window::repaint ()
{
  mPimpl->repaint ();
}

void Window::repaint (const RectI &rect)
{
  mPimpl->repaint (rect);
}

void Window::makeCurrent ()
{
  mPimpl->makeCurrent ();
}

Window::Modality Window::modality () const
{
  return mPimpl->modality ();
}

void Window::setModality (Modality modality)
{
  mPimpl->setModality (modality);
}

const char *Window::title () const
{
  return mPimpl->title ();
}

void Window::setTitle (const char *title)
{
  mPimpl->setTitle (title);
}

Vec2I Window::pos () const
{
  return mPimpl->pos ();
}

void Window::setPos (const Vec2I &pos)
{
  mPimpl->setPos (pos);
}

Vec2I Window::size () const
{
  return mPimpl->size ();
}

void Window::setSize (const Vec2I &size)
{
  mPimpl->setSize (size);
}

std::optional<Vec2I> Window::minSize () const
{
  return mPimpl->minSize ();
}

std::optional<Vec2I> Window::maxSize () const
{
  return mPimpl->maxSize ();
}

void Window::setSizeLimits (const std::optional<Vec2I> &minSize, const std::optional<Vec2I> &maxSize)
{
  mPimpl->setSizeLimits (minSize, maxSize);
}

std::optional<Vec2I> Window::aspectRatio () const
{
  return mPimpl->aspectRatio ();
}

void Window::setAspectRatio (const std::optional<Vec2I> &ratio)
{
  mPimpl->setAspectRatio (ratio);
}

Vec2I Window::frameBufferSize () const
{
  return mPimpl->frameBufferSize ();
}

UiRectI Window::frameUiRect () const
{
  return mPimpl->frameUiRect ();
}

Vec2F Window::contentScale () const
{
  return mPimpl->contentScale ();
}

float Window::opacity () const
{
  return mPimpl->opacity ();
}

void Window::setOpacity (float opacity)
{
  mPimpl->setOpacity (opacity);
}

bool Window::isVisible () const
{
  return mPimpl->isVisible ();
}

void Window::setVislble (bool visible)
{
  mPimpl->setVislble (visible);
}

void Window::show ()
{
  mPimpl->show ();
}

void Window::hide ()
{
  mPimpl->hide ();
}

bool Window::isIconified () const
{
  return mPimpl->isIconified ();
}

bool Window::isMaximized () const
{
  return mPimpl->isMaximized ();
}

void Window::iconify ()
{
  mPimpl->iconify ();
}

void Window::maximize ()
{
  mPimpl->maximize ();
}

void Window::restore ()
{
  mPimpl->restore ();
}

void Window::requestAttention ()
{
  mPimpl->requestAttention ();
}

bool Window::isFocused () const
{
  return mPimpl->isFocused ();
}

void Window::focus ()
{
  mPimpl->focus ();
}

bool Window::isDecorated () const
{
  return mPimpl->isDecorated ();
}

void Window::setDecorated (bool decorated)
{
  mPimpl->setDecorated (decorated);
}

bool Window::isResizable () const
{
  return mPimpl->isResizable ();
}

void Window::setResizable (bool resizable)
{
  mPimpl->setResizable (resizable);
}

bool Window::isFloating () const
{
  return mPimpl->isFloating ();
}

void Window::setFloating (bool floating)
{
  mPimpl->setFloating (floating);
}

bool Window::isAutoIconified () const
{
  return mPimpl->isAutoIconified ();
}

void Window::setAutoIconified (bool autoIconified)
{
  mPimpl->setAutoIconified (autoIconified);
}

bool Window::isFocusedOnShow () const
{
  return mPimpl->isFocusedOnShow ();
}

void Window::setFocusedOnShow (bool focusedOnShow)
{
  mPimpl->setFocusedOnShow (focusedOnShow);
}

bool Window::isMousePassthrough () const
{
  return mPimpl->isMousePassthrough ();
}

void Window::setMousePassthrough (bool mousePassthrough)
{
  mPimpl->setMousePassthrough (mousePassthrough);
}

bool Window::isHovered () const
{
  return mPimpl->isHovered ();
}

bool Window::hasTransparentFramebuffer () const
{
  return mPimpl->hasTransparentFramebuffer ();
}

bool Window::hasDoubleBuffer () const
{
  return mPimpl->hasDoubleBuffer ();
}

Window::CursorInputMode Window::cursorInputMode () const
{
  return mPimpl->cursorInputMode ();
}

void Window::setCursorInputMode (CursorInputMode mode)
{
  mPimpl->setCursorInputMode (mode);
}

bool Window::stickKeysEnabled () const
{
  return mPimpl->stickKeysEnabled ();
}

void Window::setStickKeysEnabled (bool enabled)
{
  mPimpl->setStickKeysEnabled (enabled);
}

bool Window::stickMouseButtonsEnabled () const
{
  return mPimpl->stickMouseButtonsEnabled ();
}

void Window::setStickMouseButtonsEnabled (bool enabled)
{
  mPimpl->setStickMouseButtonsEnabled (enabled);
}

bool Window::lockKeyModsEnabled () const
{
  return mPimpl->lockKeyModsEnabled ();
}

void Window::setLockKeyModsEnabled (bool enabled)
{
  mPimpl->setLockKeyModsEnabled (enabled);
}

bool Window::rawMouseMotionEnabled () const
{
  return mPimpl->rawMouseMotionEnabled ();
}

void Window::setRawMouseMotionEnabled (bool enabled)
{
  mPimpl->setRawMouseMotionEnabled (enabled);
}

bool Window::rawMouseMotionSupported () const
{
  return mPimpl->rawMouseMotionSupported ();
}

Vec2D Window::cursorPos () const
{
  return mPimpl->cursorPos ();
}

void Window::setCursorPos (const Vec2D &pos)
{
  mPimpl->setCursorPos (pos);
}

const char *Window::clipboard () const
{
  return mPimpl->clipboard ();
}

void Window::setClipboard (const char *data)
{
  mPimpl->setClipboard (data);
}

void Window::closeEvent ()
{
  mPimpl->closeEvent ();
}

void Window::focusInEvent ()
{
  mPimpl->focusInEvent ();
}

void Window::focusOutEvent ()
{
  mPimpl->focusOutEvent ();
}

void Window::renderEvent (const RenderEvent &event)
{
  mPimpl->renderEvent (event);
}

void Window::hoverEvent (const HoverEvent &event)
{
  mPimpl->hoverEvent (event);
}

void Window::moveEvent (const MoveEvent &event)
{
  mPimpl->moveEvent (event);
}

void Window::resizeEvent (const ResizeEvent &event)
{
  mPimpl->resizeEvent (event);
}

void Window::keyEvent (const KeyEvent &event)
{
  mPimpl->keyEvent (event);
}

void Window::charEvent (const CharEvent &event)
{
  mPimpl->charEvent (event);
}

void Window::mouseEvent (const MouseEvent &event)
{
  mPimpl->mouseEvent (event);
}

void Window::scrollEvent (const ScrollEvent &event)
{
  mPimpl->scrollEvent (event);
}

void Window::enterEvent ()
{
  mPimpl->enterEvent ();
}

void Window::leaveEvent ()
{
  mPimpl->leaveEvent ();
}

void Window::scaleEvent (const ScaleEvent &event)
{
  mPimpl->scaleEvent (event);
}

void Window::appRenderEvent ()
{
  mPimpl->appRenderEvent ();
}

}  // namespace Luma::Core
