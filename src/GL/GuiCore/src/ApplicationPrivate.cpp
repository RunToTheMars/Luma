#include "ApplicationPrivate.h"

std::unordered_set<GL::Window *> GL::ApplicationPrivate::allWindows;
std::unordered_set<GL::Window *> GL::ApplicationPrivate::allApplicationModals;
GL::Window *GL::ApplicationPrivate::focusWindow = nullptr;

std::set<GL::Monitor *> GL::ApplicationPrivate::allMonitors;

