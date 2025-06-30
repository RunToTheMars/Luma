#include "GL/Window.h"
#include "GL/KeyEvent.h"
#include "GL/MouseEvent.h"
#include "GL/ResizeEvent.h"
#include "GL/Widget.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <unordered_map>

namespace GL
{
class EventCallback
{
public:
  static void resizeCallback (GL::Window &window, const GL::ResizeEvent &event) { window.resizeEvent (event); }
  static void keyCallback (GL::Window &window, const GL::KeyEvent &event) { window.keyEvent (event); }
  static void mouseCallback (GL::Window &window, const GL::MouseEvent &event) { window.mouseEvent (event); }
};
}

namespace
{

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
// clang-format on

class GLFWWindowContainer
{
public:
  static GLFWWindowContainer &get_instance ()
  {
    static GLFWWindowContainer container;
    return container;
  }

  void add (GLFWwindow &glfwwindow, GL::Window &window) { m_data.emplace (&glfwwindow, &window); }
  void remove (GLFWwindow &glfwwindow) { m_data.erase (&glfwwindow); }

  GL::Window &find (GLFWwindow &glfwwindow)
  {
    auto it = m_data.find (&glfwwindow);
    return *it->second;
  }

private:
  GLFWWindowContainer () = default;
  ~GLFWWindowContainer () = default;

  std::unordered_map<GLFWwindow *, GL::Window *> m_data;
};

void keyCallback (GLFWwindow *window, int key, int /*scancode*/, int action, int /*mode*/)
{
  GL::EventCallback::keyCallback (GLFWWindowContainer::get_instance ().find (*window), GL::KeyEvent (static_cast<GL::Key> (key), static_cast<GL::KeyAction> (action)));
}

void resizeCallback (GLFWwindow *window, int width, int height)
{
  GL::EventCallback::resizeCallback (GLFWWindowContainer::get_instance ().find (*window), GL::ResizeEvent (Geometry::Size (width, height)));
}

// void mouseCallback (GLFWwindow *window, int width, int height)
// {
//   GL::EventCallback::resizeCallback (GLFWWindowContainer::get_instance ().find (*window), GL::ResizeEvent (Geometry::Size (width, height)));
// }
}  // namespace

namespace
{

}  // namespace

namespace GL
{

class WindowImpl
{
public:
  GLFWwindow *window = nullptr;
};

Window::Window () : m_pimpl (std::make_unique<WindowImpl> ()) {}

Window::~Window ()
{
  if (GLFWwindow *window = m_pimpl->window)
    {
      glfwDestroyWindow (window);
      GLFWWindowContainer::get_instance ().remove (*window);
    }
}

WindowCreateConfig Window::create (const Geometry::Size &size, const char *title)
{
  return WindowCreateConfig (*this, size, title);
}

void Window::init () {}
void Window::renderEvent () {}
void Window::resizeEvent (const ResizeEvent & /*event*/) {}
void Window::keyEvent (const KeyEvent & /*event*/) {}
void Window::mouseEvent (const MouseEvent & /*event*/) {}
void Window::exec ()
{
  glfwMakeContextCurrent (m_pimpl->window);
  glfwSwapInterval (0);
  init ();

  while (!glfwWindowShouldClose (m_pimpl->window))
    {
      renderEvent ();
      glfwSwapBuffers (m_pimpl->window);
      glfwPollEvents ();
    }
}

void Window::close () { glfwSetWindowShouldClose (m_pimpl->window, GL_TRUE); }

WindowCreateConfig::WindowCreateConfig (GL::Window &window, const Geometry::Size &size, const char *title)
  : m_window (window), m_size (size), m_title (title)
{
  glfwInit ();
  glfwDefaultWindowHints ();
}

WindowCreateConfig::~WindowCreateConfig ()
{
  m_window.m_pimpl->window = glfwCreateWindow (m_size.width (), m_size.height (), m_title, nullptr, nullptr);
  GLFWWindowContainer::get_instance ().add (*m_window.m_pimpl->window, m_window);

  glfwGetWindowSize (m_window.m_pimpl->window, &m_size.rwidth (), &m_size.rheight ());
  resizeCallback (m_window.m_pimpl->window, m_size.width (), m_size.height ());

  glfwSetKeyCallback (m_window.m_pimpl->window, keyCallback);
  glfwSetWindowSizeCallback (m_window.m_pimpl->window, resizeCallback);
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
