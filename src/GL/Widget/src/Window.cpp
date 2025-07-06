#include "GL/Window.h"
#include "GL/KeyEvent.h"
#include "GL/MouseEvent.h"
#include "GL/Widget.h"
#include <GLFW/glfw3.h>
#include "GL/Widget.h"


namespace
{
GLFWwindow *globalGLFWWindow = nullptr;
GL::Window *globalWindow = nullptr;
}  // namespace

namespace
{

}  // namespace

namespace GL
{

Window &Window::getInstance ()
{
  static Window window;
  return window;
}

Window::Window () noexcept
{
  globalWindow = this;
}

Window::~Window () noexcept
{
  if (globalGLFWWindow)
    glfwDestroyWindow (globalGLFWWindow);
}

WindowCreateConfig Window::create (const Geometry::Size &size, const char *title)
{
  return WindowCreateConfig (size, title);
}

void Window::open (std::unique_ptr<GL::Widget> widget)
{
  mWidget = std::move (widget);

  while (!glfwWindowShouldClose (globalGLFWWindow))
    {
      mWidget->renderEventPrivate ();
      glfwSwapBuffers (globalGLFWWindow);
      glfwPollEvents ();
    }
}

void Window::close ()
{
  glfwSetWindowShouldClose (globalGLFWWindow, GL_TRUE);
}

GL::Widget *Window::widget ()
{
  return mWidget.get ();
}

Geometry::Size Window::size () const
{
  return mSize;
}

WindowCreateConfig::WindowCreateConfig (const Geometry::Size &size, const char *title)
  : mSize (size), mTitle (title)
{
  glfwInit ();
  glfwDefaultWindowHints ();
}

WindowCreateConfig::~WindowCreateConfig ()
{
  globalGLFWWindow = glfwCreateWindow (mSize.width (), mSize.height (), mTitle, nullptr, nullptr);

  glfwGetWindowSize (globalGLFWWindow, &globalWindow->mSize.rwidth (), &globalWindow->mSize.rheight ());
  globalWindow->sizeChanged.notify (globalWindow->mSize);

  glfwSetKeyCallback (globalGLFWWindow, [] (GLFWwindow */*window*/, int key, int /*scancode*/, int action, int /*mode*/) {
    globalWindow->mWidget->keyEventPrivate (GL::KeyEvent (static_cast<GL::Key> (key), static_cast<GL::KeyAction> (action)));
  });

  glfwSetWindowSizeCallback (globalGLFWWindow, [] (GLFWwindow * /*window*/, int width, int height) {
    if (width != globalWindow->mSize.width () || height != globalWindow->mSize.height ())
      {
        globalWindow->mSize = Geometry::Size (width, height);
        globalWindow->sizeChanged.notify (globalWindow->mSize);
      }
  });

  glfwMakeContextCurrent (globalGLFWWindow);
  glfwSwapInterval (0);
}

WindowCreateConfig &WindowCreateConfig::setRedBits (int val)
{
  glfwWindowHint (GLFW_RED_BITS, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setGreenBits (int val)
{
  glfwWindowHint (GLFW_GREEN_BITS, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setBlueBits (int val)
{
  glfwWindowHint (GLFW_BLUE_BITS, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setAlphaBits (int val)
{
  glfwWindowHint (GLFW_ALPHA_BITS, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setPositionX (int val)
{
  glfwWindowHint (GLFW_POSITION_X, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setPositionY (int val)
{
  glfwWindowHint (GLFW_POSITION_Y, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setSamples (int val)
{
  glfwWindowHint (GLFW_SAMPLES, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setRefreshRate (int val)
{
  glfwWindowHint (GLFW_REFRESH_RATE, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setDoubleBuffer (bool val)
{
  glfwWindowHint (GLFW_DOUBLEBUFFER, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setResizable (bool val)
{
  glfwWindowHint (GLFW_RESIZABLE, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setDecorated (bool val)
{
  glfwWindowHint (GLFW_DECORATED, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setFocused (bool val)
{
  glfwWindowHint (GLFW_FOCUSED, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setVisible (bool val)
{
  glfwWindowHint (GLFW_VISIBLE, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setMaximized (bool val)
{
  glfwWindowHint (GLFW_MAXIMIZED, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setAutoIconify (bool val)
{
  glfwWindowHint (GLFW_AUTO_ICONIFY, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setFloating (bool val)
{
  glfwWindowHint (GLFW_FLOATING, static_cast<int> (val));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setOpenGLProfile (Profile profile)
{
  glfwWindowHint (GLFW_OPENGL_PROFILE, static_cast<int> (profile));
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setVersionMajor (int val)
{
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, val);
  return *this;
}

WindowCreateConfig &WindowCreateConfig::setVersionMinor (int val)
{
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, val);
  return *this;
}
}  // namespace GLFW
