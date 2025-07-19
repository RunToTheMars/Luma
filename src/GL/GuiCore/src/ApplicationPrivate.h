#pragma once

#include <unordered_set>
#include <list>
#include <set>

namespace GL
{
class Window;
class Monitor;
namespace ApplicationPrivate
{
extern std::unordered_set<GL::Window *> allWindows;
extern std::unordered_set<GL::Window *> allApplicationModals;
extern GL::Window *focusWindow;

extern std::set<GL::Monitor *> allMonitors;
}  // namespace ApplicationPrivate
}  // namespace GL
