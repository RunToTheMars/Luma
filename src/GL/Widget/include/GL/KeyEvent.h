#pragma once

namespace GL
{
enum class KeyAction {
  Press,
  Release,
  Repeat,
};

enum class Key {
  Key_0,
  Key_1,
  Key_2,
  Key_3,
  Key_4,
  Key_5,
  Key_6,
  Key_7,
  Key_8,
  Key_9,

  Key_A,
  Key_B,
  Key_C,
  Key_D,
  Key_E,
  Key_F,
  Key_G,
  Key_H,
  Key_I,
  Key_J,
  Key_K,
  Key_L,
  Key_M,
  Key_N,
  Key_O,
  Key_P,
  Key_Q,
  Key_R,
  Key_S,
  Key_T,
  Key_U,
  Key_V,
  Key_W,
  Key_X,
  Key_Y,
  Key_Z,

  Key_ESCAPE,
};

class KeyEvent {
public:
  KeyEvent(Key key, KeyAction action)
      : m_key(key), m_action(action) {}
  ~KeyEvent () = default;

  const Key key() const { return m_key; }
  const KeyAction action() const { return m_action; }

private:
  const Key m_key;
  const KeyAction m_action;
};
}
