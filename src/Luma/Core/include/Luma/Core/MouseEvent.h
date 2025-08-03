#pragma once

#include "Luma/Geom/Vector.h"

namespace Luma::Core {
class MouseEvent
{
public:

enum class Button {
  BUTTON_1 = 0,             /* GLFW_MOUSE_BUTTON_1      */
  BUTTON_2 = 1,             /* GLFW_MOUSE_BUTTON_2      */
  BUTTON_3 = 2,             /* GLFW_MOUSE_BUTTON_3      */
  BUTTON_4 = 3,             /* GLFW_MOUSE_BUTTON_4      */
  BUTTON_5 = 4,             /* GLFW_MOUSE_BUTTON_5      */
  BUTTON_6 = 5,             /* GLFW_MOUSE_BUTTON_6      */
  BUTTON_7 = 6,             /* GLFW_MOUSE_BUTTON_7      */
  BUTTON_8 = 7,             /* GLFW_MOUSE_BUTTON_8      */
  BUTTON_9 = 8,             /* GLFW_MOUSE_BUTTON_9      */

  BUTTON_LEFT   = BUTTON_1, /* GLFW_MOUSE_BUTTON_LEFT   */
  BUTTON_RIGHT  = BUTTON_2, /* GLFW_MOUSE_BUTTON_RIGHT  */
  BUTTON_CENTER = BUTTON_3, /* GLFW_MOUSE_BUTTON_MIDDLE */
};

enum class Action {
  Press   = 1, /* GLFW_PRESS   */
  Release = 0, /* GLFW_RELEASE */
};

  MouseEvent (Button button, Action action) : mButton (button), mAction (action) {}
  ~MouseEvent () = default;

  const Button button () const { return mButton; }
  const Action action () const { return mAction; }

private:
  Button mButton;
  Action mAction;
};
} // namespace Luma::Core
