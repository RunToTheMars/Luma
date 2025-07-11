#include "ApplicationPrivate.h"

std::unordered_set<GL::Window *> GL::ApplicationPrivate::allWindows;
GL::Window *GL::ApplicationPrivate::focusWindow = nullptr;
std::unordered_set<GL::Window *> GL::ApplicationPrivate::allApplicationModals;
