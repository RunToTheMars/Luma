#pragma once

namespace Luma::Core
{
class Window;
class Monitor;
};

struct GLFWmonitor;
struct GLFWwindow;

inline GLFWmonitor *toGLFWmonitor (void *ptr) { return static_cast<GLFWmonitor *> (ptr); }
inline Luma::Core::Monitor *toGLmonitor (void *ptr) { return static_cast<Luma::Core::Monitor *> (ptr); }

inline GLFWwindow *toGLFWwindow (void *ptr) { return static_cast<GLFWwindow *> (ptr); }
inline Luma::Core::Window *toGLwindow (void *ptr) { return static_cast<Luma::Core::Window *> (ptr); }
