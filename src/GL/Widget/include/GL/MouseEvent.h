#pragma once

#include "Geometry/PointF.h"

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
  MouseEvent (GL::MouseButton button, const Geometry::PointF &position) : m_button (button), m_position (position) {}
  ~MouseEvent () = default;

  const GL::MouseButton button () const { return m_button; }
  const Geometry::PointF &position () const { return m_position; }

private:
  GL::MouseButton m_button;
  Geometry::PointF m_position;
};
} // namespace GL
