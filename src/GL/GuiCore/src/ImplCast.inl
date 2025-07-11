#pragma once

namespace GL
{
class Window;
class Monitor;
};

struct GLFWmonitor;
struct GLFWwindow;

inline GLFWmonitor *toGLFWmonitor (void *ptr) { return static_cast<GLFWmonitor *> (ptr); }
inline GL::Monitor *toGLmonitor (void *ptr) { return static_cast<GL::Monitor *> (ptr); }

inline GLFWwindow *toGLFWwindow (void *ptr) { return static_cast<GLFWwindow *> (ptr); }
inline GL::Window *toGLwindow (void *ptr) { return static_cast<GL::Window *> (ptr); }
