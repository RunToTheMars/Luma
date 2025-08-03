#pragma once

namespace Luma::Core
{
class KeyEvent
{
public:

enum class Action
{
  Press   = 1, /* GLFW_PRESS   */
  Repeat  = 2, /* GLFW_REPEAT  */
  Release = 0, /* GLFW_RELEASE */
};

enum class Key
{
  Key_Space             = 32,                     /* GLFW_KEY_SPACE         */
  Key_Apostrophe        = 39,      /* ' */        /* GLFW_KEY_APOSTROPHE    */
  Key_Comma             = 44,      /* , */        /* GLFW_KEY_COMMA         */
  Key_Minus             = 45,      /* - */        /* GLFW_KEY_MINUS         */
  Key_Period            = 46,      /* . */        /* GLFW_KEY_PERIOD        */
  Key_Slash             = 47,      /* / */        /* GLFW_KEY_SLASH         */
  Key_0                 = 48,      /* 0 */        /* GLFW_KEY_0             */
  Key_1                 = 49,      /* 1 */        /* GLFW_KEY_1             */
  Key_2                 = 50,      /* 2 */        /* GLFW_KEY_2             */
  Key_3                 = 51,      /* 3 */        /* GLFW_KEY_3             */
  Key_4                 = 52,      /* 4 */        /* GLFW_KEY_4             */
  Key_5                 = 53,      /* 5 */        /* GLFW_KEY_5             */
  Key_6                 = 54,      /* 6 */        /* GLFW_KEY_6             */
  Key_7                 = 55,      /* 7 */        /* GLFW_KEY_7             */
  Key_8                 = 56,      /* 8 */        /* GLFW_KEY_8             */
  Key_9                 = 57,      /* 9 */        /* GLFW_KEY_9             */
  Key_Semicolon         = 59,      /* ; */        /* GLFW_KEY_SEMICOLON     */
  Key_Equal             = 61,      /* = */        /* GLFW_KEY_EQUAL         */
  Key_A                 = 65,      /* A */        /* GLFW_KEY_A             */
  Key_B                 = 66,      /* B */        /* GLFW_KEY_B             */
  Key_C                 = 67,      /* C */        /* GLFW_KEY_C             */
  Key_D                 = 68,      /* D */        /* GLFW_KEY_D             */
  Key_E                 = 69,      /* E */        /* GLFW_KEY_E             */
  Key_F                 = 70,      /* F */        /* GLFW_KEY_F             */
  Key_G                 = 71,      /* G */        /* GLFW_KEY_G             */
  Key_H                 = 72,      /* H */        /* GLFW_KEY_H             */
  Key_I                 = 73,      /* I */        /* GLFW_KEY_I             */
  Key_J                 = 74,      /* J */        /* GLFW_KEY_J             */
  Key_K                 = 75,      /* K */        /* GLFW_KEY_K             */
  Key_L                 = 76,      /* L */        /* GLFW_KEY_L             */
  Key_M                 = 77,      /* M */        /* GLFW_KEY_M             */
  Key_N                 = 78,      /* N */        /* GLFW_KEY_N             */
  Key_O                 = 79,      /* O */        /* GLFW_KEY_O             */
  Key_P                 = 80,      /* P */        /* GLFW_KEY_P             */
  Key_Q                 = 81,      /* Q */        /* GLFW_KEY_Q             */
  Key_R                 = 82,      /* R */        /* GLFW_KEY_R             */
  Key_S                 = 83,      /* S */        /* GLFW_KEY_S             */
  Key_T                 = 84,      /* T */        /* GLFW_KEY_T             */
  Key_U                 = 85,      /* U */        /* GLFW_KEY_U             */
  Key_V                 = 86,      /* V */        /* GLFW_KEY_V             */
  Key_W                 = 87,      /* W */        /* GLFW_KEY_W             */
  Key_X                 = 88,      /* X */        /* GLFW_KEY_X             */
  Key_Y                 = 89,      /* Y */        /* GLFW_KEY_Y             */
  Key_Z                 = 90,      /* Z */        /* GLFW_KEY_Z             */
  Key_LeftBracket       = 91,      /* [ */        /* GLFW_KEY_LEFT_BRACKET  */
  Key_BackSlash         = 92,      /* \ */        /* GLFW_KEY_BACKSLASH     */
  Key_RightBracket      = 93,      /* ] */        /* GLFW_KEY_RIGHT_BRACKET */
  Key_GraveAcent        = 96,      /* ` */        /* GLFW_KEY_GRAVE_ACCENT  */

  Key_Escape            = 256,                    /* GLFW_KEY_ESCAPE       */
  Key_Enter             = 257,                    /* GLFW_KEY_ENTER        */
  Key_Tab               = 258,                    /* GLFW_KEY_TAB          */
  Key_BackSpace         = 259,                    /* GLFW_KEY_BACKSPACE    */
  Key_Insert            = 260,                    /* GLFW_KEY_INSERT       */
  Key_Delete            = 261,                    /* GLFW_KEY_DELETE       */
  Key_Right             = 262,                    /* GLFW_KEY_RIGHT        */
  Key_Left              = 263,                    /* GLFW_KEY_LEFT         */
  Key_Down              = 264,                    /* GLFW_KEY_DOWN         */
  Key_Up                = 265,                    /* GLFW_KEY_UP           */
  Key_PageUp            = 266,                    /* GLFW_KEY_PAGE_UP      */
  Key_PageDown          = 267,                    /* GLFW_KEY_PAGE_DOWN    */
  Key_Home              = 268,                    /* GLFW_KEY_HOME         */
  Key_End               = 269,                    /* GLFW_KEY_END          */
  Key_CapsLock          = 280,                    /* GLFW_KEY_CAPS_LOCK    */
  Key_ScrollLock        = 281,                    /* GLFW_KEY_SCROLL_LOCK  */
  Key_NumLock           = 282,                    /* GLFW_KEY_NUM_LOCK     */
  Key_PrintScreen       = 283,                    /* GLFW_KEY_PRINT_SCREEN */
  Key_Pause             = 284,                    /* GLFW_KEY_PAUSE        */
  Key_F1                = 290,                    /* GLFW_KEY_F1           */
  Key_F2                = 291,                    /* GLFW_KEY_F2           */
  Key_F3                = 292,                    /* GLFW_KEY_F3           */
  Key_F4                = 293,                    /* GLFW_KEY_F4           */
  Key_F5                = 294,                    /* GLFW_KEY_F5           */
  Key_F6                = 295,                    /* GLFW_KEY_F6           */
  Key_F7                = 296,                    /* GLFW_KEY_F7           */
  Key_F8                = 297,                    /* GLFW_KEY_F8           */
  Key_F9                = 298,                    /* GLFW_KEY_F9           */
  Key_F10               = 299,                    /* GLFW_KEY_F10          */
  Key_F11               = 300,                    /* GLFW_KEY_F11          */
  Key_F12               = 301,                    /* GLFW_KEY_F12          */
  Key_F13               = 302,                    /* GLFW_KEY_F13          */
  Key_F14               = 303,                    /* GLFW_KEY_F14          */
  Key_F15               = 304,                    /* GLFW_KEY_F15          */
  Key_F16               = 305,                    /* GLFW_KEY_F16          */
  Key_F17               = 306,                    /* GLFW_KEY_F17          */
  Key_F18               = 307,                    /* GLFW_KEY_F18          */
  Key_F19               = 308,                    /* GLFW_KEY_F19          */
  Key_F20               = 309,                    /* GLFW_KEY_F20          */
  Key_F21               = 310,                    /* GLFW_KEY_F21          */
  Key_F22               = 311,                    /* GLFW_KEY_F22          */
  Key_F23               = 312,                    /* GLFW_KEY_F23          */
  Key_F24               = 313,                    /* GLFW_KEY_F24          */
  Key_F25               = 314,                    /* GLFW_KEY_F25          */
  Key_KeyPad_0          = 320,                    /* GLFW_KEY_KP_0         */
  Key_KeyPad_1          = 321,                    /* GLFW_KEY_KP_1         */
  Key_KeyPad_2          = 322,                    /* GLFW_KEY_KP_2         */
  Key_KeyPad_3          = 323,                    /* GLFW_KEY_KP_3         */
  Key_KeyPad_4          = 324,                    /* GLFW_KEY_KP_4         */
  Key_KeyPad_5          = 325,                    /* GLFW_KEY_KP_5         */
  Key_KeyPad_6          = 326,                    /* GLFW_KEY_KP_6         */
  Key_KeyPad_7          = 327,                    /* GLFW_KEY_KP_7         */
  Key_KeyPad_8          = 328,                    /* GLFW_KEY_KP_8         */
  Key_KeyPad_9          = 329,                    /* GLFW_KEY_KP_9         */
};

enum Mode
{
  Shift    = 0x0001, /* GLFW_MOD_SHIFT     */
  Ctrl     = 0x0002, /* GLFW_MOD_CONTROL   */
  Alt      = 0x0004, /* GLFW_MOD_ALT       */
  Super    = 0x0008, /* GLFW_MOD_SUPER     */
  CapsLock = 0x0010, /* GLFW_MOD_CAPS_LOCK */
  NumLock  = 0x0020  /* GLFW_MOD_NUM_LOCK  */
};

  KeyEvent (Key key, Action action, Mode mode) : mKey (key), mAction (action), mMode (mode) {}
  ~KeyEvent () = default;

  const Key key () const { return mKey; }
  const Action action () const { return mAction; }
  const Mode mode () const { return mMode; }

private:
  const Key mKey;
  const Action mAction;
  const Mode mMode;
};
}
