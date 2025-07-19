#pragma once

namespace GL {
enum class MouseButton {
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

enum class MouseButtonAction {
  Press   = 1, /* GLFW_PRESS   */
  Release = 0, /* GLFW_RELEASE */
};

class MouseEvent
{
public:
  MouseEvent (GL::MouseButton button, GL::MouseButtonAction action) : mButton (button), mAction (action) {}
  ~MouseEvent () = default;

  const GL::MouseButton button () const { return mButton; }
  const GL::MouseButtonAction action () const { return mAction; }

private:
  GL::MouseButton mButton;
  GL::MouseButtonAction mAction;
};
} // namespace GL
