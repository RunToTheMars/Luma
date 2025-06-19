#include "GL/Window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
namespace
{

struct
{
  GLFWwindow *nativePtr = nullptr;
  GL::Window *ptr = nullptr;
  int width = 0;
  int height = 0;
} global_window_data;

// clang-format off
int toGLFW (GL::WindowCreateConfig::Profile profile)
{
  switch (profile)
    {
      using m = GL::WindowCreateConfig::Profile;
      case m::ANY   : return GLFW_OPENGL_ANY_PROFILE;
      case m::CORE  : return GLFW_OPENGL_CORE_PROFILE;
      case m::COMPAT: return GLFW_OPENGL_COMPAT_PROFILE;
    }

  throw std::runtime_error ("Invalid Profile");
  return GLFW_OPENGL_ANY_PROFILE;
}
// clang-format on

// clang-format off
GL::Key toKey (int key)
{
  switch (key)
    {
      case GLFW_KEY_0: return GL::Key::Key_0;
      case GLFW_KEY_1: return GL::Key::Key_1;
      case GLFW_KEY_2: return GL::Key::Key_2;
      case GLFW_KEY_3: return GL::Key::Key_3;
      case GLFW_KEY_4: return GL::Key::Key_4;
      case GLFW_KEY_5: return GL::Key::Key_5;
      case GLFW_KEY_6: return GL::Key::Key_6;
      case GLFW_KEY_7: return GL::Key::Key_7;
      case GLFW_KEY_8: return GL::Key::Key_8;
      case GLFW_KEY_9: return GL::Key::Key_9;

      case GLFW_KEY_ESCAPE:  return GL::Key::Key_ESCAPE;
    }

  throw std::runtime_error ("Invalid Key");
  return GL::Key::Key_ESCAPE;
}
// clang-format on

GL::KeyAction toKeyAction (int action)
{
  switch (action)
    {
      case GLFW_PRESS  : return GL::KeyAction::Press;
      case GLFW_RELEASE: return GL::KeyAction::Release;
      case GLFW_REPEAT : return GL::KeyAction::Repeat;
    }

  throw std::runtime_error ("Invalid KeyAction");
  return GL::KeyAction::Repeat;
}

void key_callback (GLFWwindow * /*window*/, int key, int scancode, int action, int /*mode*/)
{
  global_window_data.ptr->keyEvent (GL::KeyEvent (toKey (key), toKeyAction (action)));
}

void resize_callback (GLFWwindow * /*window*/, int width, int height)
{
  int oldWidth = global_window_data.width;
  int oldHeight = global_window_data.height;

  global_window_data.width = width;
  global_window_data.height = height;

  global_window_data.ptr->resizeEvent (GL::ResizeEvent (oldWidth, oldHeight, width, height));
}
}  // namespace

namespace GL
{

Window::Window ()
{
  if (global_window_data.ptr)
    throw std::runtime_error ("Window is singleton");

  glfwInit ();
  global_window_data.ptr = this;
}

Window::~Window ()
{
  if (global_window_data.nativePtr)
    glfwDestroyWindow (global_window_data.nativePtr);

  glfwTerminate ();
}

WindowCreateConfig Window::create (int width, int height, const char *title)
{
  if (global_window_data.nativePtr)
    throw std::runtime_error ("Call Window::create once!");

  return WindowCreateConfig (width, height, title);
}

void Window::init () {}
void Window::renderEvent () {}
void Window::resizeEvent (const ResizeEvent & /*event*/) {}
void Window::keyEvent (const KeyEvent & /*event*/) {}
void Window::mouseEvent (const MouseEvent & /*event*/) {}
void Window::exec ()
{
  glfwMakeContextCurrent (global_window_data.nativePtr);
  glfwSwapInterval (0);
  init ();

  while (!glfwWindowShouldClose (global_window_data.nativePtr))
    {
      renderEvent ();
      glfwSwapBuffers (global_window_data.nativePtr);
      glfwPollEvents ();
    }
}

void Window::close () { glfwSetWindowShouldClose (global_window_data.nativePtr, GL_TRUE); }

int Window::width () const { return global_window_data.width; }
int Window::height () const { return global_window_data.height; }

WindowCreateConfig::WindowCreateConfig (int width, int height, const char *title)
  : m_width (width), m_height (height), m_title (title)
{
  glfwDefaultWindowHints ();
}

WindowCreateConfig::~WindowCreateConfig ()
{
  global_window_data.nativePtr = glfwCreateWindow (m_width, m_height, m_title, nullptr, nullptr);
  global_window_data.width = m_width;
  global_window_data.height = m_height;

  glfwSetKeyCallback (global_window_data.nativePtr, key_callback);
  glfwSetFramebufferSizeCallback (global_window_data.nativePtr, resize_callback);
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
  glfwWindowHint (GLFW_OPENGL_PROFILE, toGLFW (profile));
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
