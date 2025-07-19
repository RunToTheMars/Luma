#pragma once

#include <unordered_set>
#include <vector>

namespace Luma::Core
{
class Object;
class Window;
class ApplicationImpl
{
public:
  static bool isExecuted;
  static bool exit;
  static int returnCode;

  static std::unordered_set<Window *> allWindows;
  static std::unordered_set<Window *> allApplicationModalWindows;
  static Window *focusWindow;

  static std::vector<Window *> windowsForRepaint[2];
  static std::vector<Window *> *windowsForCurrRepaint;
  static std::vector<Window *> *windowsForPostRepaint;

  static std::unordered_set<Object *> objectToDelete;
};
}  // namespace Luma::Core
