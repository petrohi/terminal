#include "ps2.h"

#include "keys.h"
#include <string.h>

void ps2_init(struct ps2 *ps2) {

  ps2->lshift = 0;
  ps2->rshift = 0;
  ps2->lctrl = 0;
  ps2->rctrl = 0;
  ps2->lalt = 0;
  ps2->ralt = 0;
  ps2->lgui = 0;
  ps2->rgui = 0;
  ps2->menu = 0;

  for (size_t i = 0; i < PS2_MAX_PRESSED_KEYS; ++i)
    ps2->keys[i] = KEY_NONE;

  ps2->keyup = false;
  ps2->e0 = false;

  ps2->response = 0;
}

static const uint8_t decoder[0x100] = {
    [0x1c] = KEY_A,
    [0x32] = KEY_B,
    [0x21] = KEY_C,
    [0x23] = KEY_D,
    [0x24] = KEY_E,
    [0x2b] = KEY_F,
    [0x34] = KEY_G,
    [0x33] = KEY_H,
    [0x43] = KEY_I,
    [0x3b] = KEY_J,
    [0x42] = KEY_K,
    [0x4b] = KEY_L,
    [0x3a] = KEY_M,
    [0x31] = KEY_N,
    [0x44] = KEY_O,
    [0x4d] = KEY_P,
    [0x15] = KEY_Q,
    [0x2d] = KEY_R,
    [0x1b] = KEY_S,
    [0x2c] = KEY_T,
    [0x3c] = KEY_U,
    [0x2a] = KEY_V,
    [0x1d] = KEY_W,
    [0x22] = KEY_X,
    [0x35] = KEY_Y,
    [0x1a] = KEY_Z,
    [0x16] = KEY_1_EXCLAMATION_MARK,
    [0x1e] = KEY_2_AT,
    [0x26] = KEY_3_NUMBER_SIGN,
    [0x25] = KEY_4_DOLLAR,
    [0x2e] = KEY_5_PERCENT,
    [0x36] = KEY_6_CARET,
    [0x3d] = KEY_7_AMPERSAND,
    [0x3e] = KEY_8_ASTERISK,
    [0x46] = KEY_9_OPARENTHESIS,
    [0x45] = KEY_0_CPARENTHESIS,
    [0x5a] = KEY_ENTER,
    [0x76] = KEY_ESCAPE,
    [0x66] = KEY_BACKSPACE,
    [0x0d] = KEY_TAB,
    [0x29] = KEY_SPACEBAR,
    [0x4e] = KEY_MINUS_UNDERSCORE,
    [0x55] = KEY_EQUAL_PLUS,
    [0x54] = KEY_OBRACKET_AND_OBRACE,
    [0x5b] = KEY_CBRACKET_AND_CBRACE,
    [0x5d] = KEY_BACKSLASH_VERTICAL_BAR,
    [0x4c] = KEY_SEMICOLON_COLON,
    [0x52] = KEY_SINGLE_AND_DOUBLE_QUOTE,
    [0x0e] = KEY_GRAVE_ACCENT_AND_TILDE,
    [0x41] = KEY_COMMA_AND_LESS,
    [0x49] = KEY_DOT_GREATER,
    [0x4a] = KEY_SLASH_QUESTION,
    [0x58] = KEY_CAPS_LOCK,
    [0x05] = KEY_F1,
    [0x06] = KEY_F2,
    [0x04] = KEY_F3,
    [0x0c] = KEY_F4,
    [0x03] = KEY_F5,
    [0x0b] = KEY_F6,
    [0x83] = KEY_F7,
    [0x0a] = KEY_F8,
    [0x01] = KEY_F9,
    [0x09] = KEY_F10,
    [0x78] = KEY_F11,
    [0x07] = KEY_F12,
    [0x7e] = KEY_SCROLL_LOCK,
    [0x77] = KEY_KEYPAD_NUM_LOCK_AND_CLEAR,
    [0x7c] = KEY_KEYPAD_ASTERIKS,
    [0x7b] = KEY_KEYPAD_MINUS,
    [0x79] = KEY_KEYPAD_PLUS,
    [0x69] = KEY_KEYPAD_1_END,
    [0x72] = KEY_KEYPAD_2_DOWN_ARROW,
    [0x7a] = KEY_KEYPAD_3_PAGEDN,
    [0x6b] = KEY_KEYPAD_4_LEFT_ARROW,
    [0x73] = KEY_KEYPAD_5,
    [0x74] = KEY_KEYPAD_6_RIGHT_ARROW,
    [0x6c] = KEY_KEYPAD_7_HOME,
    [0x75] = KEY_KEYPAD_8_UP_ARROW,
    [0x7d] = KEY_KEYPAD_9_PAGEUP,
    [0x70] = KEY_KEYPAD_0_INSERT,
    [0x71] = KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE,
};

static const uint8_t e0_decoder[0x100] = {
    [0x70] = KEY_INSERT,       [0x6c] = KEY_HOME,
    [0x7d] = KEY_PAGEUP,       [0x71] = KEY_DELETE,
    [0x69] = KEY_END1,         [0x7a] = KEY_PAGEDOWN,
    [0x74] = KEY_RIGHTARROW,   [0x6b] = KEY_LEFTARROW,
    [0x72] = KEY_DOWNARROW,    [0x75] = KEY_UPARROW,
    [0x4a] = KEY_KEYPAD_SLASH, [0x5a] = KEY_KEYPAD_ENTER,
};

static void handle_keyup(struct ps2 *ps2, uint8_t key) {
  if (key == KEY_NONE)
    return;

  for (size_t i = 0; i < PS2_MAX_PRESSED_KEYS; ++i)
    if (ps2->keys[i] == key) {
      for (; i < PS2_MAX_PRESSED_KEYS - 1; ++i)
        ps2->keys[i] = ps2->keys[i + 1];

      ps2->keys[PS2_MAX_PRESSED_KEYS - 1] = KEY_NONE;
      break;
    }
}

static void handle_keydown(struct ps2 *ps2, uint8_t key) {
  if (key == KEY_NONE)
    return;

  for (size_t i = 0; i < PS2_MAX_PRESSED_KEYS; ++i)
    if (ps2->keys[i] == key)
      return;

  for (size_t i = PS2_MAX_PRESSED_KEYS - 1; i > 0; --i)
    if (ps2->keys[i - 1] != KEY_NONE)
      ps2->keys[i] = ps2->keys[i - 1];

  ps2->keys[0] = key;
}

static void handle_key(struct ps2 *ps2, uint8_t key) {
  if (ps2->keyup)
    handle_keyup(ps2, key);
  else
    handle_keydown(ps2, key);
}

void ps2_handle_code(struct ps2 *ps2, uint8_t code) {
  if (code == 0xaa || code == 0xff) {
    return;
  } else if (code == 0xf0) {
    ps2->keyup = true;
    return;
  } else if (code == 0xe0) {
    ps2->e0 = true;
    return;
  } else if (code == PS2_ECHO_ACK || code == PS2_COMMAND_ACK ||
             code == PS2_COMMAND_RESEND) {
    ps2->response = code;
    return;
  }

  if (ps2->e0) {
    switch (code) {
    case 0x14: // RCTRL
      ps2->rctrl = ps2->keyup ? 0 : 1;
      break;
    case 0x11: // RALT
      ps2->ralt = ps2->keyup ? 0 : 1;
      break;
    case 0x1f: // LGUI
      ps2->lgui = ps2->keyup ? 0 : 1;
      break;
    case 0x27: // RGUI
      ps2->rgui = ps2->keyup ? 0 : 1;
      break;
    case 0x2f: // MENU
      ps2->menu = ps2->keyup ? 0 : 1;
      break;
    default:
      handle_key(ps2, e0_decoder[code]);
      break;
    }
    ps2->e0 = false;
  } else {
    switch (code) {
    case 0x12: // LSHIFT
      ps2->lshift = ps2->keyup ? 0 : 1;
      break;
    case 0x59: // RSHIFT
      ps2->rshift = ps2->keyup ? 0 : 1;
      break;
    case 0x14: // LCTRL
      ps2->lctrl = ps2->keyup ? 0 : 1;
      break;
    case 0x11: // LALT
      ps2->lalt = ps2->keyup ? 0 : 1;
      break;
    default:
      handle_key(ps2, decoder[code]);
      break;
    }
  }

  if (ps2->keyup)
    ps2->keyup = false;

  if (ps2->response)
    ps2->response = 0;
}
