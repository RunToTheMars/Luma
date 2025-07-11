#pragma once

#include <unordered_set>

namespace GL
{
class Window;
namespace ApplicationPrivate
{
extern std::unordered_set<GL::Window *> allWindows;
extern GL::Window *focusWindow;
extern std::unordered_set<GL::Window *> allApplicationModals;
}  // namespace ApplicationPrivate
}  // namespace GL
