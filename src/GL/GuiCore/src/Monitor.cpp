#include "GL/Monitor.h"
#include <GLFW/glfw3.h>
#include "ImplCast.inl"

namespace GL
{
Monitor::Monitor (void *impl): mPimpl (impl)
{
  glfwSetMonitorUserPointer (toGLFWmonitor (mPimpl), this);
}

Monitor::~Monitor ()
{
}

Geom::Vec2I Monitor::pos () const
{
  Geom::Vec2I pos;
  glfwGetMonitorPos (toGLFWmonitor (mPimpl), &pos[0], &pos[1]);
  return pos;
}

Geom::RectI Monitor::workRect () const
{
  Geom::RectI workRect;
  glfwGetMonitorWorkarea (toGLFWmonitor (mPimpl),
                          &workRect.leftTop[0],
                          &workRect.leftTop[1],
                          &workRect.size[0],
                          &workRect.size[1]);
  return workRect;
}

Geom::Vec2I Monitor::physicalSize () const
{
  Geom::Vec2I size;
  glfwGetMonitorPhysicalSize (toGLFWmonitor (mPimpl),
                              &size[0],
                              &size[1]);
  return size;
}

Geom::Vec2F Monitor::contentScale () const
{
  Geom::Vec2F scale;
  glfwGetMonitorContentScale (toGLFWmonitor (mPimpl),
                              &scale[0],
                              &scale[1]);
  return scale;
}

const char *Monitor::name () const
{
  return glfwGetMonitorName (toGLFWmonitor (mPimpl));
}

GL::VideoMode Monitor::videoMode () const
{
  return GL::VideoMode (glfwGetVideoMode (toGLFWmonitor (mPimpl)));
}

GL::VideoModeList Monitor::videoModeList () const
{
  return GL::VideoModeList (toGLFWmonitor (mPimpl));
}

bool Monitor::operator== (const Monitor &rhs) const
{
  return mPimpl == rhs.mPimpl;
}

MonitorList::MonitorList ()
{
  mPimpl = glfwGetMonitors (&mCount);
}

MonitorList::~MonitorList () = default;

int MonitorList::count () const
{
  return mCount;
}

Monitor MonitorList::operator[] (int i) const
{
  return Monitor (*(static_cast<GLFWmonitor**> (mPimpl) + i));
}

VideoMode::~VideoMode () = default;

int VideoMode::width () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->width;
}

int VideoMode::height () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->height;
}

Geom::Vec2I VideoMode::size () const
{
  return {width (), height ()};
}

int VideoMode::redBits () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->redBits;
}

int VideoMode::greenBits () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->greenBits;
}

int VideoMode::blueBits () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->blueBits;
}

int VideoMode::refreshRate () const
{
  return static_cast<const GLFWvidmode *> (mPimpl)->refreshRate;
}

VideoMode::VideoMode (const void *videoModeImpl): mPimpl (videoModeImpl)
{
}

VideoModeList::VideoModeList (void *monitorPimpl): mPimpl (glfwGetVideoModes (toGLFWmonitor (monitorPimpl), &mCount))
{
}

VideoModeList::~VideoModeList () = default;

int VideoModeList::count () const
{
  return mCount;
}

VideoMode VideoModeList::operator[] (int i) const
{
  return VideoMode (static_cast<const GLFWvidmode*> (mPimpl) + i);
}

}  // namespace GL
