#pragma once

namespace GL {
enum class MouseButton {
  BUTTON_1,
  BUTTON_2,
  BUTTON_3,
  BUTTON_4,
  BUTTON_5,
  BUTTON_6,
  BUTTON_7,
  BUTTON_8,
  BUTTON_9,

  BUTTON_LEFT = BUTTON_1,
  BUTTON_RIGHT = BUTTON_2,
  BUTTON_CENTER = BUTTON_3,
};

class MouseEvent {
public:
  MouseEvent(MouseButton button, double x, double y)
      : m_button(button), m_x(x), m_y(y) {}
  ~MouseEvent() = default;

  const MouseButton button() const { return m_button; }

  const double x() const { return m_x; }
  const double y() const { return m_y; }

private:
  MouseButton m_button;
  double m_x;
  double m_y;
};
} // namespace GL
