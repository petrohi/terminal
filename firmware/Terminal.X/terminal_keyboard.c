#include "terminal_internal.h"

#include "terminal_keyboard.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define FIRST_REPEAT_COUNTER 500
#define NEXT_REPEAT_COUNTER 33
#define ESCAPE_KEY_BUFFER_SIZE 16

static size_t get_case(struct terminal *terminal) {
  return terminal->lock_state.caps ^ terminal->shift_state;
}

static size_t get_shift(struct terminal *terminal) {
  return terminal->shift_state;
}

static size_t get_ctrl(struct terminal *terminal) {
  return terminal->ctrl_state;
}

static size_t get_alt_gr(struct terminal *terminal) {
  return terminal->alt_gr && terminal->ralt_state;
}

static size_t get_utf8_charset(struct terminal *terminal) {
  return terminal->charset == CHARSET_UTF8;
}

static size_t get_ctrl_alt(struct terminal *terminal) {
  return terminal->ctrl_state && terminal_keyboard_get_alt_state(terminal);
}

static size_t get_new_line_mode(struct terminal *terminal) {
  return terminal->new_line_mode;
}

static size_t get_cursor_key_mode(struct terminal *terminal) {
  return terminal->cursor_key_mode;
}

static size_t get_ansi_mode(struct terminal *terminal) {
  return terminal->ansi_mode;
}

static size_t get_backspace_mode(struct terminal *terminal) {
  return terminal->backspace_mode;
}

static size_t get_num(struct terminal *terminal) {
  return terminal->lock_state.num;
}

static size_t get_keyboard_compatibility(struct terminal *terminal) {
  return terminal->keyboard_compatibility;
}

void terminal_keyboard_update_leds(struct terminal *terminal) {
  terminal->callbacks->keyboard_set_leds(terminal->lock_state);
}

static void handle_caps_lock(struct terminal *terminal) {
  terminal->lock_state.caps ^= 1;
  terminal_keyboard_update_leds(terminal);
}

static void handle_num_lock(struct terminal *terminal) {
  terminal->lock_state.num ^= 1;
  terminal_keyboard_update_leds(terminal);
}

static void handle_ctrl_alt_delete(struct terminal *terminal) {
  terminal->callbacks->activate_config();
}

static void update_scroll_lock(struct terminal *terminal, bool scroll_lock) {
  terminal->lock_state.scroll = scroll_lock;
  terminal_keyboard_update_leds(terminal);

  if (terminal->lock_state.scroll)
    terminal_uart_transmit_character(terminal, CHAR_XOFF);
  else
    terminal_uart_transmit_character(terminal, CHAR_XON);
}

static void handle_scroll_lock(struct terminal *terminal) {
  update_scroll_lock(terminal, !terminal->lock_state.scroll);
}

static void handle_ctrl_s(struct terminal *terminal) {
  update_scroll_lock(terminal, true);
}

static void handle_ctrl_q(struct terminal *terminal) {
  update_scroll_lock(terminal, false);
}

static const struct keys_entry us_entries[] = {
    [KEY_A] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('a'), KEY_CHR('A')),
                   KEY_CHR('\x01')),
    [KEY_B] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('b'), KEY_CHR('B')),
                   KEY_CHR('\x02')),
    [KEY_C] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('c'), KEY_CHR('C')),
                   KEY_CHR('\x03')),
    [KEY_D] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('d'), KEY_CHR('D')),
                   KEY_CHR('\x04')),
    [KEY_E] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('e'), KEY_CHR('E')),
                   KEY_CHR('\x05')),
    [KEY_F] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('f'), KEY_CHR('F')),
                   KEY_CHR('\x06')),
    [KEY_G] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('g'), KEY_CHR('G')),
                   KEY_CHR('\x07')),
    [KEY_H] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('h'), KEY_CHR('H')),
                   KEY_CHR('\x08')),
    [KEY_I] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('i'), KEY_CHR('I')),
                   KEY_CHR('\x09')),
    [KEY_J] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('j'), KEY_CHR('J')),
                   KEY_CHR('\x0a')),
    [KEY_K] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('k'), KEY_CHR('K')),
                   KEY_CHR('\x0b')),
    [KEY_L] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('l'), KEY_CHR('L')),
                   KEY_CHR('\x0c')),
    [KEY_M] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('m'), KEY_CHR('M')),
                   KEY_CHR('\x0d')),
    [KEY_N] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('n'), KEY_CHR('N')),
                   KEY_CHR('\x0e')),
    [KEY_O] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('o'), KEY_CHR('O')),
                   KEY_CHR('\x0f')),
    [KEY_P] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('p'), KEY_CHR('P')),
                   KEY_CHR('\x10')),
    [KEY_Q] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('q'), KEY_CHR('Q')),
                   KEY_HANDLER(handle_ctrl_q)),
    [KEY_R] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('r'), KEY_CHR('R')),
                   KEY_CHR('\x12')),
    [KEY_S] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('s'), KEY_CHR('S')),
                   KEY_HANDLER(handle_ctrl_s)),
    [KEY_T] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('t'), KEY_CHR('T')),
                   KEY_CHR('\x14')),
    [KEY_U] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('u'), KEY_CHR('U')),
                   KEY_CHR('\x15')),
    [KEY_V] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('v'), KEY_CHR('V')),
                   KEY_CHR('\x16')),
    [KEY_W] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('w'), KEY_CHR('W')),
                   KEY_CHR('\x17')),
    [KEY_X] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('x'), KEY_CHR('X')),
                   KEY_CHR('\x18')),
    [KEY_Y] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('y'), KEY_CHR('Y')),
                   KEY_CHR('\x19')),
    [KEY_Z] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('z'), KEY_CHR('Z')),
                   KEY_CHR('\x1a')),
    [KEY_1_EXCLAMATION_MARK] =
        KEY_ROUTER(get_shift, KEY_CHR('1'), KEY_CHR('!')),
    [KEY_2_AT] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('2'), KEY_CHR('@')),
                   KEY_CHR('\0')),
    [KEY_3_NUMBER_SIGN] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('3'), KEY_CHR('#')),
                   KEY_CHR('\x1b')),
    [KEY_4_DOLLAR] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('4'), KEY_CHR('$')),
                   KEY_CHR('\x1c')),
    [KEY_5_PERCENT] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('5'), KEY_CHR('%')),
                   KEY_CHR('\x1d')),
    [KEY_6_CARET] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('6'), KEY_CHR('^')),
                   KEY_CHR('\x1e')),
    [KEY_7_AMPERSAND] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('7'), KEY_CHR('&')),
                   KEY_CHR('\x1f')),
    [KEY_8_ASTERISK] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('8'), KEY_CHR('*')),
                   KEY_CHR('\x7f')),
    [KEY_9_OPARENTHESIS] = KEY_ROUTER(get_shift, KEY_CHR('9'), KEY_CHR('(')),
    [KEY_0_CPARENTHESIS] = KEY_ROUTER(get_shift, KEY_CHR('0'), KEY_CHR(')')),
    [KEY_ENTER] =
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a")),
    [KEY_ESCAPE] = KEY_CHR('\x1b'),
    [KEY_BACKSPACE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_backspace_mode, KEY_CHR('\x7f'), KEY_CHR('\x08')),
        KEY_CHR('\x08')),
    [KEY_TAB] = KEY_CHR('\x09'),
    [KEY_SPACEBAR] = KEY_ROUTER(get_ctrl, KEY_CHR(' '), KEY_CHR('\0')),
    [KEY_MINUS_UNDERSCORE] = KEY_ROUTER(get_shift, KEY_CHR('-'), KEY_CHR('_')),
    [KEY_EQUAL_PLUS] = KEY_ROUTER(get_shift, KEY_CHR('='), KEY_CHR('+')),
    [KEY_OBRACKET_AND_OBRACE] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('['), KEY_CHR('{')),
                   KEY_CHR('\x1b')),
    [KEY_CBRACKET_AND_CBRACE] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR(']'), KEY_CHR('}')),
                   KEY_CHR('\x1d')),
    [KEY_BACKSLASH_VERTICAL_BAR] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('\\'), KEY_CHR('|')),
                   KEY_CHR('\x1c')),
    [KEY_SEMICOLON_COLON] = KEY_ROUTER(get_shift, KEY_CHR(';'), KEY_CHR(':')),
    [KEY_SINGLE_AND_DOUBLE_QUOTE] =
        KEY_ROUTER(get_shift, KEY_CHR('\''), KEY_CHR('"')),
    [KEY_GRAVE_ACCENT_AND_TILDE] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('`'), KEY_CHR('~')),
                   KEY_CHR('\x1e')),
    [KEY_COMMA_AND_LESS] = KEY_ROUTER(get_shift, KEY_CHR(','), KEY_CHR('<')),
    [KEY_DOT_GREATER] = KEY_ROUTER(get_shift, KEY_CHR('.'), KEY_CHR('>')),
    [KEY_SLASH_QUESTION] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('/'), KEY_CHR('?')),
                   KEY_CHR('\x1f')),
    [KEY_CAPS_LOCK] = KEY_HANDLER(handle_caps_lock),
    [KEY_F1] = KEY_SS3_MOD_CSI("P"),
    [KEY_F2] = KEY_SS3_MOD_CSI("Q"),
    [KEY_F3] = KEY_SS3_MOD_CSI("R"),
    [KEY_F4] = KEY_SS3_MOD_CSI("S"),
    [KEY_F5] = KEY_CSI("15~"),
    [KEY_F6] = KEY_CSI("17~"),
    [KEY_F7] = KEY_CSI("18~"),
    [KEY_F8] = KEY_CSI("19~"),
    [KEY_F9] = KEY_CSI("20~"),
    [KEY_F10] = KEY_CSI("21~"),
    [KEY_F11] = KEY_CSI("23~"),
    [KEY_F12] = KEY_CSI("24~"),
    [KEY_SCROLL_LOCK] = KEY_HANDLER(handle_scroll_lock),
    [KEY_INSERT] = KEY_CSI("2~"),
    [KEY_HOME] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1_1("H"), KEY_SS3("H")),
        KEY_CSI("1~")),
    [KEY_PAGEUP] = KEY_CSI("5~"),
    [KEY_DELETE] = KEY_ROUTER(get_ctrl_alt, KEY_CSI("3~"),
                              KEY_HANDLER(handle_ctrl_alt_delete)),
    [KEY_END1] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("F"), KEY_SS3("F")),
        KEY_CSI("4~")),
    [KEY_PAGEDOWN] = KEY_CSI("6~"),
    [KEY_RIGHTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("C"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("C"), KEY_SS3("C"))),
    [KEY_LEFTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("D"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("D"), KEY_SS3("D"))),
    [KEY_DOWNARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("B"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("B"), KEY_SS3("B"))),
    [KEY_UPARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("A"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("A"), KEY_SS3("A"))),
    [KEY_KEYPAD_NUM_LOCK_AND_CLEAR] = KEY_HANDLER(handle_num_lock),
    [KEY_KEYPAD_SLASH] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?o"), KEY_SS3("o")),
        KEY_CHR('/')),
    [KEY_KEYPAD_ASTERIKS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?j"), KEY_SS3("j")),
        KEY_CHR('*')),
    [KEY_KEYPAD_MINUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?m"), KEY_SS3("m")),
        KEY_CHR('-')),
    [KEY_KEYPAD_PLUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?k"), KEY_SS3("k")),
        KEY_CHR('+')),
    [KEY_KEYPAD_ENTER] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?M"), KEY_SS3("M")),
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a"))),
    [KEY_KEYPAD_1_END] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?q"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("F"), KEY_SS3("q"))),
                   KEY_CHR('1')),
    [KEY_KEYPAD_2_DOWN_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?r"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("B"), KEY_SS3("r"))),
                   KEY_CHR('2')),
    [KEY_KEYPAD_3_PAGEDN] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?s"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("6~"), KEY_SS3("s"))),
                   KEY_CHR('3')),
    [KEY_KEYPAD_4_LEFT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?t"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("D"), KEY_SS3("t"))),
                   KEY_CHR('4')),
    [KEY_KEYPAD_5] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?u"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("E"), KEY_SS3("u"))),
                   KEY_CHR('5')),
    [KEY_KEYPAD_6_RIGHT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?v"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("C"), KEY_SS3("v"))),
                   KEY_CHR('6')),
    [KEY_KEYPAD_7_HOME] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?w"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("H"), KEY_SS3("w"))),
                   KEY_CHR('7')),
    [KEY_KEYPAD_8_UP_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?x"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("A"), KEY_SS3("x"))),
                   KEY_CHR('8')),
    [KEY_KEYPAD_9_PAGEUP] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?y"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("5~"), KEY_SS3("y"))),
                   KEY_CHR('9')),
    [KEY_KEYPAD_0_INSERT] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?p"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("2~"), KEY_SS3("p"))),
                   KEY_CHR('0')),
    [KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE] = KEY_ROUTER(
        get_num,
        KEY_ROUTER(get_ctrl_alt,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?n"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("3~"), KEY_SS3("n"))),
                   KEY_HANDLER(handle_ctrl_alt_delete)),
        KEY_CHR('.')),
};

static const struct keys_entry uk_entries[] = {
    [KEY_A] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('a'), KEY_CHR('A')),
                   KEY_CHR('\x01')),
        KEY_ROUTER(
            get_case,
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xe1'), KEY_STR("á")),
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xc1'), KEY_STR("Á")))),
    [KEY_B] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('b'), KEY_CHR('B')),
                   KEY_CHR('\x02')),
    [KEY_C] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('c'), KEY_CHR('C')),
                   KEY_CHR('\x03')),
    [KEY_D] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('d'), KEY_CHR('D')),
                   KEY_CHR('\x04')),
    [KEY_E] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('e'), KEY_CHR('E')),
                   KEY_CHR('\x05')),
        KEY_ROUTER(
            get_case,
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xe9'), KEY_STR("é")),
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xc9'), KEY_STR("É")))),
    [KEY_F] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('f'), KEY_CHR('F')),
                   KEY_CHR('\x06')),
    [KEY_G] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('g'), KEY_CHR('G')),
                   KEY_CHR('\x07')),
    [KEY_H] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('h'), KEY_CHR('H')),
                   KEY_CHR('\x08')),
    [KEY_I] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('i'), KEY_CHR('I')),
                   KEY_CHR('\x09')),
        KEY_ROUTER(
            get_case,
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xed'), KEY_STR("í")),
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xcd'), KEY_STR("Í")))),
    [KEY_J] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('j'), KEY_CHR('J')),
                   KEY_CHR('\x0a')),
    [KEY_K] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('k'), KEY_CHR('K')),
                   KEY_CHR('\x0b')),
    [KEY_L] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('l'), KEY_CHR('L')),
                   KEY_CHR('\x0c')),
    [KEY_M] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('m'), KEY_CHR('M')),
                   KEY_CHR('\x0d')),
    [KEY_N] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('n'), KEY_CHR('N')),
                   KEY_CHR('\x0e')),
    [KEY_O] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('o'), KEY_CHR('O')),
                   KEY_CHR('\x0f')),
        KEY_ROUTER(
            get_case,
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xf3'), KEY_STR("ó")),
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xd3'), KEY_STR("Ó")))),
    [KEY_P] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('p'), KEY_CHR('P')),
                   KEY_CHR('\x10')),
    [KEY_Q] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('q'), KEY_CHR('Q')),
                   KEY_HANDLER(handle_ctrl_q)),
    [KEY_R] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('r'), KEY_CHR('R')),
                   KEY_CHR('\x12')),
    [KEY_S] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('s'), KEY_CHR('S')),
                   KEY_HANDLER(handle_ctrl_s)),
    [KEY_T] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('t'), KEY_CHR('T')),
                   KEY_CHR('\x14')),
    [KEY_U] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('u'), KEY_CHR('U')),
                   KEY_CHR('\x15')),
        KEY_ROUTER(
            get_case,
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xda'), KEY_STR("ú")),
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xfa'), KEY_STR("Ú")))),
    [KEY_V] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('v'), KEY_CHR('V')),
                   KEY_CHR('\x16')),
    [KEY_W] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('w'), KEY_CHR('W')),
                   KEY_CHR('\x17')),
    [KEY_X] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('x'), KEY_CHR('X')),
                   KEY_CHR('\x18')),
    [KEY_Y] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('y'), KEY_CHR('Y')),
                   KEY_CHR('\x19')),
    [KEY_Z] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('z'), KEY_CHR('Z')),
                   KEY_CHR('\x1a')),
    [KEY_1_EXCLAMATION_MARK] =
        KEY_ROUTER(get_shift, KEY_CHR('1'), KEY_CHR('!')),
    [KEY_2_AT] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('2'), KEY_CHR('"')),
                   KEY_CHR('\0')),
    [KEY_3_NUMBER_SIGN] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_shift, KEY_CHR('3'),
                   KEY_ROUTER(get_utf8_charset, KEY_CHR('\xa3'), KEY_STR("£"))),
        KEY_CHR('\x1b')),
    [KEY_4_DOLLAR] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('4'), KEY_CHR('$')),
                   KEY_CHR('\x1c')),
        KEY_STR("€")),
    [KEY_5_PERCENT] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('5'), KEY_CHR('%')),
                   KEY_CHR('\x1d')),
    [KEY_6_CARET] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('6'), KEY_CHR('^')),
                   KEY_CHR('\x1e')),
    [KEY_7_AMPERSAND] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('7'), KEY_CHR('&')),
                   KEY_CHR('\x1f')),
    [KEY_8_ASTERISK] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('8'), KEY_CHR('*')),
                   KEY_CHR('\x7f')),
    [KEY_9_OPARENTHESIS] = KEY_ROUTER(get_shift, KEY_CHR('9'), KEY_CHR('(')),
    [KEY_0_CPARENTHESIS] = KEY_ROUTER(get_shift, KEY_CHR('0'), KEY_CHR(')')),
    [KEY_ENTER] =
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a")),
    [KEY_ESCAPE] = KEY_CHR('\x1b'),
    [KEY_BACKSPACE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_backspace_mode, KEY_CHR('\x7f'), KEY_CHR('\x08')),
        KEY_CHR('\x08')),
    [KEY_TAB] = KEY_CHR('\x09'),
    [KEY_SPACEBAR] = KEY_ROUTER(get_ctrl, KEY_CHR(' '), KEY_CHR('\0')),
    [KEY_MINUS_UNDERSCORE] = KEY_ROUTER(get_shift, KEY_CHR('-'), KEY_CHR('_')),
    [KEY_EQUAL_PLUS] = KEY_ROUTER(get_shift, KEY_CHR('='), KEY_CHR('+')),
    [KEY_OBRACKET_AND_OBRACE] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('['), KEY_CHR('{')),
                   KEY_CHR('\x1b')),
    [KEY_CBRACKET_AND_CBRACE] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR(']'), KEY_CHR('}')),
                   KEY_CHR('\x1d')),
    [KEY_BACKSLASH_VERTICAL_BAR] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('#'), KEY_CHR('~')),
                   KEY_CHR('\x1c')),
        KEY_ROUTER(get_shift, KEY_CHR('\\'), KEY_CHR('|'))),

    [KEY_NONUS_BACK_SLASH_VERTICAL_BAR] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('\\'), KEY_CHR('|')),
                   KEY_CHR('\x1c')),
    [KEY_SEMICOLON_COLON] = KEY_ROUTER(get_shift, KEY_CHR(';'), KEY_CHR(':')),
    [KEY_SINGLE_AND_DOUBLE_QUOTE] =
        KEY_ROUTER(get_shift, KEY_CHR('\''), KEY_CHR('@')),
    [KEY_GRAVE_ACCENT_AND_TILDE] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl,
                   KEY_ROUTER(get_shift, KEY_CHR('`'),
                              KEY_ROUTER(get_utf8_charset, KEY_CHR('\xac'),
                                         KEY_STR("¬"))),
                   KEY_CHR('\x1e')),
        KEY_ROUTER(get_utf8_charset, KEY_CHR('\xa6'), KEY_STR("¦"))),
    [KEY_COMMA_AND_LESS] = KEY_ROUTER(get_shift, KEY_CHR(','), KEY_CHR('<')),
    [KEY_DOT_GREATER] = KEY_ROUTER(get_shift, KEY_CHR('.'), KEY_CHR('>')),
    [KEY_SLASH_QUESTION] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('/'), KEY_CHR('?')),
                   KEY_CHR('\x1f')),
    [KEY_CAPS_LOCK] = KEY_HANDLER(handle_caps_lock),
    [KEY_F1] = KEY_SS3_MOD_CSI("P"),
    [KEY_F2] = KEY_SS3_MOD_CSI("Q"),
    [KEY_F3] = KEY_SS3_MOD_CSI("R"),
    [KEY_F4] = KEY_SS3_MOD_CSI("S"),
    [KEY_F5] = KEY_CSI("15~"),
    [KEY_F6] = KEY_CSI("17~"),
    [KEY_F7] = KEY_CSI("18~"),
    [KEY_F8] = KEY_CSI("19~"),
    [KEY_F9] = KEY_CSI("20~"),
    [KEY_F10] = KEY_CSI("21~"),
    [KEY_F11] = KEY_CSI("23~"),
    [KEY_F12] = KEY_CSI("24~"),
    [KEY_SCROLL_LOCK] = KEY_HANDLER(handle_scroll_lock),
    [KEY_INSERT] = KEY_CSI("2~"),
    [KEY_HOME] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1_1("H"), KEY_SS3("H")),
        KEY_CSI("1~")),
    [KEY_PAGEUP] = KEY_CSI("5~"),
    [KEY_DELETE] = KEY_ROUTER(get_ctrl_alt, KEY_CSI("3~"),
                              KEY_HANDLER(handle_ctrl_alt_delete)),
    [KEY_END1] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("F"), KEY_SS3("F")),
        KEY_CSI("4~")),
    [KEY_PAGEDOWN] = KEY_CSI("6~"),
    [KEY_RIGHTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("C"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("C"), KEY_SS3("C"))),
    [KEY_LEFTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("D"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("D"), KEY_SS3("D"))),
    [KEY_DOWNARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("B"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("B"), KEY_SS3("B"))),
    [KEY_UPARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("A"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("A"), KEY_SS3("A"))),
    [KEY_KEYPAD_NUM_LOCK_AND_CLEAR] = KEY_HANDLER(handle_num_lock),
    [KEY_KEYPAD_SLASH] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?o"), KEY_SS3("o")),
        KEY_CHR('/')),
    [KEY_KEYPAD_ASTERIKS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?j"), KEY_SS3("j")),
        KEY_CHR('*')),
    [KEY_KEYPAD_MINUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?m"), KEY_SS3("m")),
        KEY_CHR('-')),
    [KEY_KEYPAD_PLUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?k"), KEY_SS3("k")),
        KEY_CHR('+')),
    [KEY_KEYPAD_ENTER] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?M"), KEY_SS3("M")),
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a"))),
    [KEY_KEYPAD_1_END] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?q"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("F"), KEY_SS3("q"))),
                   KEY_CHR('1')),
    [KEY_KEYPAD_2_DOWN_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?r"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("B"), KEY_SS3("r"))),
                   KEY_CHR('2')),
    [KEY_KEYPAD_3_PAGEDN] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?s"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("6~"), KEY_SS3("s"))),
                   KEY_CHR('3')),
    [KEY_KEYPAD_4_LEFT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?t"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("D"), KEY_SS3("t"))),
                   KEY_CHR('4')),
    [KEY_KEYPAD_5] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?u"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("E"), KEY_SS3("u"))),
                   KEY_CHR('5')),
    [KEY_KEYPAD_6_RIGHT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?v"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("C"), KEY_SS3("v"))),
                   KEY_CHR('6')),
    [KEY_KEYPAD_7_HOME] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?w"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("H"), KEY_SS3("w"))),
                   KEY_CHR('7')),
    [KEY_KEYPAD_8_UP_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?x"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("A"), KEY_SS3("x"))),
                   KEY_CHR('8')),
    [KEY_KEYPAD_9_PAGEUP] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?y"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("5~"), KEY_SS3("y"))),
                   KEY_CHR('9')),
    [KEY_KEYPAD_0_INSERT] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?p"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("2~"), KEY_SS3("p"))),
                   KEY_CHR('0')),
    [KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE] = KEY_ROUTER(
        get_num,
        KEY_ROUTER(get_ctrl_alt,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?n"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("3~"), KEY_SS3("n"))),
                   KEY_HANDLER(handle_ctrl_alt_delete)),
        KEY_CHR('.')),
};

static const struct keys_entry de_entries[] = {
    [KEY_A] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('a'), KEY_CHR('A')),
                   KEY_CHR('\x01')),
    [KEY_B] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('b'), KEY_CHR('B')),
                   KEY_CHR('\x02')),
    [KEY_C] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('c'), KEY_CHR('C')),
                   KEY_CHR('\x03')),
    [KEY_D] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('d'), KEY_CHR('D')),
                   KEY_CHR('\x04')),
    [KEY_E] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('e'), KEY_CHR('E')),
                   KEY_CHR('\x05')),
        KEY_STR("€")),
    [KEY_F] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('f'), KEY_CHR('F')),
                   KEY_CHR('\x06')),
    [KEY_G] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('g'), KEY_CHR('G')),
                   KEY_CHR('\x07')),
    [KEY_H] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('h'), KEY_CHR('H')),
                   KEY_CHR('\x08')),
    [KEY_I] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('i'), KEY_CHR('I')),
                   KEY_CHR('\x09')),
    [KEY_J] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('j'), KEY_CHR('J')),
                   KEY_CHR('\x0a')),
    [KEY_K] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('k'), KEY_CHR('K')),
                   KEY_CHR('\x0b')),
    [KEY_L] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('l'), KEY_CHR('L')),
                   KEY_CHR('\x0c')),
    [KEY_M] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('m'), KEY_CHR('M')),
                   KEY_CHR('\x0d')),
        KEY_ROUTER(get_utf8_charset, KEY_CHR('\xb5'), KEY_STR("µ"))),
    [KEY_N] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('n'), KEY_CHR('N')),
                   KEY_CHR('\x0e')),
    [KEY_O] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('o'), KEY_CHR('O')),
                   KEY_CHR('\x0f')),
    [KEY_P] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('p'), KEY_CHR('P')),
                   KEY_CHR('\x10')),
    [KEY_Q] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('q'), KEY_CHR('Q')),
                   KEY_HANDLER(handle_ctrl_q)),
        KEY_CHR('@')),
    [KEY_R] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('r'), KEY_CHR('R')),
                   KEY_CHR('\x12')),
    [KEY_S] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('s'), KEY_CHR('S')),
                   KEY_HANDLER(handle_ctrl_s)),
    [KEY_T] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('t'), KEY_CHR('T')),
                   KEY_CHR('\x14')),
    [KEY_U] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('u'), KEY_CHR('U')),
                   KEY_CHR('\x15')),
    [KEY_V] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('v'), KEY_CHR('V')),
                   KEY_CHR('\x16')),
    [KEY_W] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('w'), KEY_CHR('W')),
                   KEY_CHR('\x17')),
    [KEY_X] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('x'), KEY_CHR('X')),
                   KEY_CHR('\x18')),
    [KEY_Y] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('z'), KEY_CHR('Z')),
                   KEY_CHR('\x1a')),
    [KEY_Z] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('y'), KEY_CHR('Y')),
                   KEY_CHR('\x19')),
    [KEY_1_EXCLAMATION_MARK] =
        KEY_ROUTER(get_shift, KEY_CHR('1'), KEY_CHR('!')),
    [KEY_2_AT] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('2'), KEY_CHR('"')),
                   KEY_CHR('\0')),
        KEY_ROUTER(get_utf8_charset, KEY_CHR('\xb2'), KEY_STR("²"))),
    [KEY_3_NUMBER_SIGN] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl,
                   KEY_ROUTER(get_shift, KEY_CHR('3'),
                              KEY_ROUTER(get_utf8_charset, KEY_CHR('\xa7'),
                                         KEY_STR("§"))),
                   KEY_CHR('\x1b')),
        KEY_ROUTER(get_utf8_charset, KEY_CHR('\xb3'), KEY_STR("³"))),
    [KEY_4_DOLLAR] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('4'), KEY_CHR('$')),
                   KEY_CHR('\x1c')),
    [KEY_5_PERCENT] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('5'), KEY_CHR('%')),
                   KEY_CHR('\x1d')),
    [KEY_6_CARET] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('6'), KEY_CHR('&')),
                   KEY_CHR('\x1e')),
    [KEY_7_AMPERSAND] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('7'), KEY_CHR('/')),
                   KEY_CHR('\x1f')),
        KEY_CHR('{')),
    [KEY_8_ASTERISK] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('8'), KEY_CHR('(')),
                   KEY_CHR('\x7f')),
        KEY_CHR('[')),
    [KEY_9_OPARENTHESIS] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('9'), KEY_CHR(')')),
        KEY_CHR(']')),
    [KEY_0_CPARENTHESIS] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('0'), KEY_CHR('=')),
        KEY_CHR('}')),
    [KEY_ENTER] =
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a")),
    [KEY_ESCAPE] = KEY_CHR('\x1b'),
    [KEY_BACKSPACE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_backspace_mode, KEY_CHR('\x7f'), KEY_CHR('\x08')),
        KEY_CHR('\x08')),
    [KEY_TAB] = KEY_CHR('\x09'),
    [KEY_SPACEBAR] = KEY_ROUTER(get_ctrl, KEY_CHR(' '), KEY_CHR('\0')),
    [KEY_MINUS_UNDERSCORE] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_shift,
                   KEY_ROUTER(get_utf8_charset, KEY_CHR('\xdf'), KEY_STR("ß")),
                   KEY_CHR('?')),
        KEY_ROUTER(
            get_shift, KEY_CHR('\\'),
            KEY_ROUTER(get_utf8_charset, KEY_CHR('\xdf'), KEY_STR("ß")))),
    [KEY_EQUAL_PLUS] = KEY_ROUTER(
        get_shift, KEY_ROUTER(get_utf8_charset, KEY_CHR('\xb4'), KEY_STR("´")),
        KEY_CHR('`')),
    [KEY_OBRACKET_AND_OBRACE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_shift,
                   KEY_ROUTER(get_utf8_charset, KEY_CHR('\xfc'), KEY_STR("ü")),
                   KEY_ROUTER(get_utf8_charset, KEY_CHR('\xdc'), KEY_STR("Ü"))),
        KEY_CHR('\x1b')),
    [KEY_CBRACKET_AND_CBRACE] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('+'), KEY_CHR('*')),
                   KEY_CHR('\x78')),
        KEY_CHR('~')),
    [KEY_BACKSLASH_VERTICAL_BAR] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('#'), KEY_CHR('\'')),
                   KEY_CHR('\x1c')),
    [KEY_NONUS_BACK_SLASH_VERTICAL_BAR] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('<'), KEY_CHR('>')),
                   KEY_CHR('\x1c')),
        KEY_CHR('|')),
    [KEY_SEMICOLON_COLON] = KEY_ROUTER(
        get_shift, KEY_ROUTER(get_utf8_charset, KEY_CHR('\xf6'), KEY_STR("ö")),
        KEY_ROUTER(get_utf8_charset, KEY_CHR('\xd6'), KEY_STR("Ö"))),
    [KEY_SINGLE_AND_DOUBLE_QUOTE] = KEY_ROUTER(
        get_shift, KEY_ROUTER(get_utf8_charset, KEY_CHR('\xe4'), KEY_STR("ä")),
        KEY_ROUTER(get_utf8_charset, KEY_CHR('\xc4'), KEY_STR("Ä"))),
    [KEY_GRAVE_ACCENT_AND_TILDE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_shift, KEY_CHR('^'),
                   KEY_ROUTER(get_utf8_charset, KEY_CHR('\xb0'), KEY_STR("°"))),
        KEY_CHR('\x1e')),
    [KEY_COMMA_AND_LESS] = KEY_ROUTER(get_shift, KEY_CHR(','), KEY_CHR(';')),
    [KEY_DOT_GREATER] = KEY_ROUTER(get_shift, KEY_CHR('.'), KEY_CHR(':')),
    [KEY_SLASH_QUESTION] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('-'), KEY_CHR('_')),
                   KEY_CHR('\x1f')),

    [KEY_CAPS_LOCK] = KEY_HANDLER(handle_caps_lock),
    [KEY_F1] = KEY_SS3_MOD_CSI("P"),
    [KEY_F2] = KEY_SS3_MOD_CSI("Q"),
    [KEY_F3] = KEY_SS3_MOD_CSI("R"),
    [KEY_F4] = KEY_SS3_MOD_CSI("S"),
    [KEY_F5] = KEY_CSI("15~"),
    [KEY_F6] = KEY_CSI("17~"),
    [KEY_F7] = KEY_CSI("18~"),
    [KEY_F8] = KEY_CSI("19~"),
    [KEY_F9] = KEY_CSI("20~"),
    [KEY_F10] = KEY_CSI("21~"),
    [KEY_F11] = KEY_CSI("23~"),
    [KEY_F12] = KEY_CSI("24~"),
    [KEY_SCROLL_LOCK] = KEY_HANDLER(handle_scroll_lock),
    [KEY_INSERT] = KEY_CSI("2~"),
    [KEY_HOME] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1_1("H"), KEY_SS3("H")),
        KEY_CSI("1~")),
    [KEY_PAGEUP] = KEY_CSI("5~"),
    [KEY_DELETE] = KEY_ROUTER(get_ctrl_alt, KEY_CSI("3~"),
                              KEY_HANDLER(handle_ctrl_alt_delete)),
    [KEY_END1] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("F"), KEY_SS3("F")),
        KEY_CSI("4~")),
    [KEY_PAGEDOWN] = KEY_CSI("6~"),
    [KEY_RIGHTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("C"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("C"), KEY_SS3("C"))),
    [KEY_LEFTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("D"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("D"), KEY_SS3("D"))),
    [KEY_DOWNARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("B"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("B"), KEY_SS3("B"))),
    [KEY_UPARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("A"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("A"), KEY_SS3("A"))),
    [KEY_KEYPAD_NUM_LOCK_AND_CLEAR] = KEY_HANDLER(handle_num_lock),
    [KEY_KEYPAD_SLASH] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?o"), KEY_SS3("o")),
        KEY_CHR('/')),
    [KEY_KEYPAD_ASTERIKS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?j"), KEY_SS3("j")),
        KEY_CHR('*')),
    [KEY_KEYPAD_MINUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?m"), KEY_SS3("m")),
        KEY_CHR('-')),
    [KEY_KEYPAD_PLUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?k"), KEY_SS3("k")),
        KEY_CHR('+')),
    [KEY_KEYPAD_ENTER] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?M"), KEY_SS3("M")),
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a"))),
    [KEY_KEYPAD_1_END] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?q"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("F"), KEY_SS3("q"))),
                   KEY_CHR('1')),
    [KEY_KEYPAD_2_DOWN_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?r"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("B"), KEY_SS3("r"))),
                   KEY_CHR('2')),
    [KEY_KEYPAD_3_PAGEDN] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?s"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("6~"), KEY_SS3("s"))),
                   KEY_CHR('3')),
    [KEY_KEYPAD_4_LEFT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?t"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("D"), KEY_SS3("t"))),
                   KEY_CHR('4')),
    [KEY_KEYPAD_5] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?u"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("E"), KEY_SS3("u"))),
                   KEY_CHR('5')),
    [KEY_KEYPAD_6_RIGHT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?v"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("C"), KEY_SS3("v"))),
                   KEY_CHR('6')),
    [KEY_KEYPAD_7_HOME] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?w"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("H"), KEY_SS3("w"))),
                   KEY_CHR('7')),
    [KEY_KEYPAD_8_UP_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?x"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("A"), KEY_SS3("x"))),
                   KEY_CHR('8')),
    [KEY_KEYPAD_9_PAGEUP] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?y"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("5~"), KEY_SS3("y"))),
                   KEY_CHR('9')),
    [KEY_KEYPAD_0_INSERT] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?p"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("2~"), KEY_SS3("p"))),
                   KEY_CHR('0')),
    [KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE] = KEY_ROUTER(
        get_num,
        KEY_ROUTER(get_ctrl_alt,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?n"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("3~"), KEY_SS3("n"))),
                   KEY_HANDLER(handle_ctrl_alt_delete)),
        KEY_CHR(',')),
};

static const struct keys_entry se_entries[] = {
    [KEY_A] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('a'), KEY_CHR('A')),
                   KEY_CHR('\x01')),
    [KEY_B] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('b'), KEY_CHR('B')),
                   KEY_CHR('\x02')),
    [KEY_C] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('c'), KEY_CHR('C')),
                   KEY_CHR('\x03')),
    [KEY_D] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('d'), KEY_CHR('D')),
                   KEY_CHR('\x04')),
    [KEY_E] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('e'), KEY_CHR('E')),
                   KEY_CHR('\x05')),
        KEY_STR("€")),
    [KEY_F] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('f'), KEY_CHR('F')),
                   KEY_CHR('\x06')),
    [KEY_G] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('g'), KEY_CHR('G')),
                   KEY_CHR('\x07')),
    [KEY_H] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('h'), KEY_CHR('H')),
                   KEY_CHR('\x08')),
    [KEY_I] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('i'), KEY_CHR('I')),
                   KEY_CHR('\x09')),
    [KEY_J] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('j'), KEY_CHR('J')),
                   KEY_CHR('\x0A')),
    [KEY_K] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('k'), KEY_CHR('K')),
                   KEY_CHR('\x0B')),
    [KEY_L] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('l'), KEY_CHR('L')),
                   KEY_CHR('\x0C')),
    [KEY_M] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('m'), KEY_CHR('M')),
                   KEY_CHR('\x0D')),
        KEY_STR("µ")),
    [KEY_N] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('n'), KEY_CHR('N')),
                   KEY_CHR('\x0E')),
    [KEY_O] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('o'), KEY_CHR('O')),
                   KEY_CHR('\x0F')),
    [KEY_P] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('p'), KEY_CHR('P')),
                   KEY_CHR('\x10')),
    [KEY_Q] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('q'), KEY_CHR('Q')),
                   KEY_HANDLER(handle_ctrl_q)),
    [KEY_R] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('r'), KEY_CHR('R')),
                   KEY_CHR('\x12')),
    [KEY_S] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('s'), KEY_CHR('S')),
                   KEY_HANDLER(handle_ctrl_s)),
    [KEY_T] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('t'), KEY_CHR('T')),
                   KEY_CHR('\x14')),
    [KEY_U] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('u'), KEY_CHR('U')),
                   KEY_CHR('\x15')),
    [KEY_V] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('v'), KEY_CHR('V')),
                   KEY_CHR('\x16')),
    [KEY_W] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('w'), KEY_CHR('W')),
                   KEY_CHR('\x17')),
    [KEY_X] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('x'), KEY_CHR('X')),
                   KEY_CHR('\x18')),
    [KEY_Y] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('y'), KEY_CHR('Y')),
                   KEY_CHR('\x19')),
    [KEY_Z] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('z'), KEY_CHR('Z')),
                   KEY_CHR('\x1A')),
    [KEY_1_EXCLAMATION_MARK] =
        KEY_ROUTER(get_shift, KEY_CHR('1'), KEY_CHR('!')),
    [KEY_2_AT] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('2'), KEY_CHR('"')),
                   KEY_CHR('\0')),
        KEY_CHR('@')),
    [KEY_3_NUMBER_SIGN] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('3'), KEY_CHR('#')),
                   KEY_CHR('\x1B')),
        KEY_STR("£")),
    [KEY_4_DOLLAR] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('4'), KEY_STR("¤")),
                   KEY_CHR('\x1C')),
        KEY_CHR('$')),
    [KEY_5_PERCENT] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('5'), KEY_CHR('%')),
                   KEY_CHR('\x1D')),
    [KEY_6_CARET] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('6'), KEY_CHR('&')),
                   KEY_CHR('\x1E')),
    [KEY_7_AMPERSAND] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('7'), KEY_CHR('/')),
                   KEY_CHR('\x1f')),
        KEY_CHR('{')),
    [KEY_8_ASTERISK] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('8'), KEY_CHR('(')),
                   KEY_CHR('\x7F')),
        KEY_CHR('[')),
    [KEY_9_OPARENTHESIS] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('9'), KEY_CHR(')')),
        KEY_CHR(']')),
    [KEY_0_CPARENTHESIS] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('0'), KEY_CHR('=')),
        KEY_CHR('}')),
    [KEY_ENTER] =
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0D'), KEY_STR("\x0D\x0D")),
    [KEY_ESCAPE] = KEY_CHR('\x1B'),
    [KEY_BACKSPACE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_backspace_mode, KEY_CHR('\x7F'), KEY_CHR('\x08')),
        KEY_CHR('\x08')),
    [KEY_TAB] = KEY_CHR('\x09'),
    [KEY_SPACEBAR] = KEY_ROUTER(get_ctrl, KEY_CHR(' '), KEY_CHR('\0')),
    [KEY_MINUS_UNDERSCORE] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('+'), KEY_CHR('?')),
        KEY_CHR('\\')),
    [KEY_EQUAL_PLUS] = // TODO! Should be a dead key
                       // (https://en.wikipedia.org/wiki/Dead_key)
    KEY_ROUTER(get_shift, KEY_STR("´"), KEY_CHR('`')),
    [KEY_OBRACKET_AND_OBRACE] =
        KEY_ROUTER(get_shift, KEY_STR("å"), KEY_STR("Å")),
    [KEY_CBRACKET_AND_CBRACE] = // TODO! Should be a dead key
                                // (https://en.wikipedia.org/wiki/Dead_key))
    KEY_ROUTER(get_alt_gr, KEY_ROUTER(get_shift, KEY_STR("¨"), KEY_CHR('^')),
               KEY_CHR('~')),
    [KEY_BACKSLASH_VERTICAL_BAR] =
        KEY_ROUTER(get_shift, KEY_CHR('\''), KEY_CHR('*')),
    [KEY_SEMICOLON_COLON] = KEY_ROUTER(get_shift, KEY_STR("ö"), KEY_STR("Ö")),
    [KEY_SINGLE_AND_DOUBLE_QUOTE] =
        KEY_ROUTER(get_shift, KEY_STR("ä"), KEY_STR("Ä")),
    [KEY_GRAVE_ACCENT_AND_TILDE] =
        KEY_ROUTER(get_shift, KEY_STR("§"), KEY_STR("½")),
    [KEY_COMMA_AND_LESS] = KEY_ROUTER(get_shift, KEY_CHR(','), KEY_CHR(';')),
    [KEY_DOT_GREATER] = KEY_ROUTER(get_shift, KEY_CHR('.'), KEY_CHR(':')),
    [KEY_SLASH_QUESTION] = KEY_ROUTER(get_shift, KEY_CHR('-'), KEY_CHR('_')),
    [KEY_CAPS_LOCK] = KEY_HANDLER(handle_caps_lock),
    [KEY_F1] = KEY_SS3_MOD_CSI("P"),
    [KEY_F2] = KEY_SS3_MOD_CSI("Q"),
    [KEY_F3] = KEY_SS3_MOD_CSI("R"),
    [KEY_F4] = KEY_SS3_MOD_CSI("S"),
    [KEY_F5] = KEY_CSI("15~"),
    [KEY_F6] = KEY_CSI("17~"),
    [KEY_F7] = KEY_CSI("18~"),
    [KEY_F8] = KEY_CSI("19~"),
    [KEY_F9] = KEY_CSI("20~"),
    [KEY_F10] = KEY_CSI("21~"),
    [KEY_F11] = KEY_CSI("23~"),
    [KEY_F12] = KEY_CSI("24~"),
    [KEY_SCROLL_LOCK] = KEY_HANDLER(handle_scroll_lock),
    [KEY_INSERT] = KEY_CSI("2~"),
    [KEY_HOME] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1_1("H"), KEY_SS3("H")),
        KEY_CSI("1~")),
    [KEY_PAGEUP] = KEY_CSI("5~"),
    [KEY_DELETE] = KEY_ROUTER(get_ctrl_alt, KEY_CSI("3~"),
                              KEY_HANDLER(handle_ctrl_alt_delete)),
    [KEY_END1] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("F"), KEY_SS3("F")),
        KEY_CSI("4~")),
    [KEY_PAGEDOWN] = KEY_CSI("6~"),
    [KEY_RIGHTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("C"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("C"), KEY_SS3("C"))),
    [KEY_LEFTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("D"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("D"), KEY_SS3("D"))),
    [KEY_DOWNARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("B"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("B"), KEY_SS3("B"))),
    [KEY_UPARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("A"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("A"), KEY_SS3("A"))),
    [KEY_KEYPAD_NUM_LOCK_AND_CLEAR] = KEY_HANDLER(handle_num_lock),
    [KEY_KEYPAD_SLASH] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?o"), KEY_SS3("o")),
        KEY_CHR('/')),
    [KEY_KEYPAD_ASTERIKS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?j"), KEY_SS3("j")),
        KEY_CHR('*')),
    [KEY_KEYPAD_MINUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?m"), KEY_SS3("m")),
        KEY_CHR('-')),
    [KEY_KEYPAD_PLUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?k"), KEY_SS3("k")),
        KEY_CHR('+')),
    [KEY_KEYPAD_ENTER] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?M"), KEY_SS3("M")),
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a"))),
    [KEY_KEYPAD_1_END] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?q"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("F"), KEY_SS3("q"))),
                   KEY_CHR('1')),
    [KEY_KEYPAD_2_DOWN_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?r"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("B"), KEY_SS3("r"))),
                   KEY_CHR('2')),
    [KEY_KEYPAD_3_PAGEDN] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?s"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("6~"), KEY_SS3("s"))),
                   KEY_CHR('3')),
    [KEY_KEYPAD_4_LEFT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?t"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("D"), KEY_SS3("t"))),
                   KEY_CHR('4')),
    [KEY_KEYPAD_5] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?u"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("E"), KEY_SS3("u"))),
                   KEY_CHR('5')),
    [KEY_KEYPAD_6_RIGHT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?v"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("C"), KEY_SS3("v"))),
                   KEY_CHR('6')),
    [KEY_KEYPAD_7_HOME] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?w"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("H"), KEY_SS3("w"))),
                   KEY_CHR('7')),
    [KEY_KEYPAD_8_UP_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?x"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("A"), KEY_SS3("x"))),
                   KEY_CHR('8')),
    [KEY_KEYPAD_9_PAGEUP] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?y"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("5~"), KEY_SS3("y"))),
                   KEY_CHR('9')),
    [KEY_KEYPAD_0_INSERT] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?p"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("2~"), KEY_SS3("p"))),
                   KEY_CHR('0')),
    [KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE] = KEY_ROUTER(
        get_num,
        KEY_ROUTER(get_ctrl_alt,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?n"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("3~"), KEY_SS3("n"))),
                   KEY_HANDLER(handle_ctrl_alt_delete)),
        KEY_CHR(',')),
    [KEY_NONUS_BACK_SLASH_VERTICAL_BAR] =
        KEY_ROUTER(get_alt_gr, KEY_ROUTER(get_case, KEY_CHR('<'), KEY_CHR('>')),
                   KEY_CHR('|')),

};

static const struct keys_entry fr_entries[] = {
    [KEY_A] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('q'), KEY_CHR('Q')),
                   KEY_HANDLER(handle_ctrl_q)),
    [KEY_B] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('b'), KEY_CHR('B')),
                   KEY_CHR('\x02')),
    [KEY_C] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('c'), KEY_CHR('C')),
                   KEY_CHR('\x03')),
    [KEY_D] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('d'), KEY_CHR('D')),
                   KEY_CHR('\x04')),
    [KEY_E] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('e'), KEY_CHR('E')),
                   KEY_CHR('\x05')),
        KEY_STR("€")),
    [KEY_F] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('f'), KEY_CHR('F')),
                   KEY_CHR('\x06')),
    [KEY_G] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('g'), KEY_CHR('G')),
                   KEY_CHR('\x07')),
    [KEY_H] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('h'), KEY_CHR('H')),
                   KEY_CHR('\x08')),
    [KEY_I] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('i'), KEY_CHR('I')),
                   KEY_CHR('\x09')),
    [KEY_J] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('j'), KEY_CHR('J')),
                   KEY_CHR('\x0a')),
    [KEY_K] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('k'), KEY_CHR('K')),
                   KEY_CHR('\x0b')),
    [KEY_L] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('l'), KEY_CHR('L')),
                   KEY_CHR('\x0c')),
    [KEY_M] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR(','), KEY_CHR('?')),
                   KEY_CHR('\x1f')),
    [KEY_N] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('n'), KEY_CHR('N')),
                   KEY_CHR('\x0e')),
    [KEY_O] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('o'), KEY_CHR('O')),
                   KEY_CHR('\x0f')),
    [KEY_P] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('p'), KEY_CHR('P')),
                   KEY_CHR('\x10')),
    [KEY_Q] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('a'), KEY_CHR('A')),
                   KEY_CHR('\x01')),
    [KEY_R] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('r'), KEY_CHR('R')),
                   KEY_CHR('\x12')),
    [KEY_S] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('s'), KEY_CHR('S')),
                   KEY_HANDLER(handle_ctrl_s)),
    [KEY_T] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('t'), KEY_CHR('T')),
                   KEY_CHR('\x14')),
    [KEY_U] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('u'), KEY_CHR('U')),
                   KEY_CHR('\x15')),
    [KEY_V] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('v'), KEY_CHR('V')),
                   KEY_CHR('\x16')),
    [KEY_W] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('z'), KEY_CHR('Z')),
                   KEY_CHR('\x1a')),
    [KEY_X] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('x'), KEY_CHR('X')),
                   KEY_CHR('\x18')),
    [KEY_Y] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('y'), KEY_CHR('Y')),
                   KEY_CHR('\x19')),
    [KEY_Z] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('w'), KEY_CHR('W')),
                   KEY_CHR('\x17')),
    [KEY_1_EXCLAMATION_MARK] =
        KEY_ROUTER(get_shift, KEY_CHR('&'), KEY_CHR('1')),
    [KEY_2_AT] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_STR("é"), KEY_CHR('2')),
                   KEY_CHR('\x1e')),
        KEY_CHR('~')),
    [KEY_3_NUMBER_SIGN] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('\"'), KEY_CHR('3')),
        KEY_CHR('#')),
    [KEY_4_DOLLAR] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('\''), KEY_CHR('4')),
        KEY_CHR('{')),
    [KEY_5_PERCENT] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('('), KEY_CHR('5')),
                   KEY_CHR('\x1b')),
        KEY_CHR('[')),
    [KEY_6_CARET] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('-'), KEY_CHR('6')),
        KEY_CHR('|')),
    [KEY_7_AMPERSAND] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_STR("è"), KEY_CHR('7')),
                   KEY_CHR('\x1e')),
        KEY_CHR('`')),
    [KEY_8_ASTERISK] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('_'), KEY_CHR('8')),
                   KEY_CHR('\x1c')),
        KEY_CHR('\\')),
    [KEY_9_OPARENTHESIS] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_STR("ç"), KEY_CHR('9')),
                   KEY_CHR('\x1e')),
        KEY_CHR('^')),
    [KEY_0_CPARENTHESIS] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_STR("à"), KEY_CHR('0')),
                   KEY_CHR('\x00')),
        KEY_CHR('@')),
    [KEY_ENTER] =
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a")),
    [KEY_ESCAPE] = KEY_CHR('\x1b'),
    [KEY_BACKSPACE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_backspace_mode, KEY_CHR('\x7f'), KEY_CHR('\x08')),
        KEY_CHR('\x08')),
    [KEY_TAB] = KEY_CHR('\x09'),
    [KEY_SPACEBAR] = KEY_ROUTER(get_ctrl, KEY_CHR(' '), KEY_CHR('\0')),
    [KEY_MINUS_UNDERSCORE] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR(')'), KEY_STR("°")),
                   KEY_CHR('\x1d')),
        KEY_CHR(']')),
    [KEY_EQUAL_PLUS] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('='), KEY_CHR('+')),
        KEY_CHR('}')),
    [KEY_OBRACKET_AND_OBRACE] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('^'), KEY_STR("¨")),
                   KEY_CHR('\x1e')),
    [KEY_CBRACKET_AND_CBRACE] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('$'), KEY_STR("£")),
        KEY_STR("¤")),
    [KEY_BACKSLASH_VERTICAL_BAR] =
        KEY_ROUTER(get_shift, KEY_CHR('*'), KEY_STR("µ")),
    [KEY_NONUS_BACK_SLASH_VERTICAL_BAR] =
        KEY_ROUTER(get_shift, KEY_CHR('<'), KEY_CHR('>')),
    [KEY_SEMICOLON_COLON] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('m'), KEY_CHR('M')),
                   KEY_CHR('\x0d')),
    [KEY_SINGLE_AND_DOUBLE_QUOTE] =
        KEY_ROUTER(get_shift, KEY_STR("ù"), KEY_CHR('%')),
    [KEY_GRAVE_ACCENT_AND_TILDE] = KEY_STR("²"),
    [KEY_COMMA_AND_LESS] = KEY_ROUTER(get_shift, KEY_CHR(';'), KEY_CHR('.')),
    [KEY_DOT_GREATER] = KEY_ROUTER(get_shift, KEY_CHR(':'), KEY_CHR('/')),
    [KEY_SLASH_QUESTION] = KEY_ROUTER(get_shift, KEY_CHR('!'), KEY_STR("§")),
    [KEY_CAPS_LOCK] = KEY_HANDLER(handle_caps_lock),
    [KEY_F1] = KEY_SS3_MOD_CSI("P"),
    //[KEY_F1] = KEY_CSI("11~"),
    [KEY_F2] = KEY_SS3_MOD_CSI("Q"),
    //[KEY_F2] = KEY_CSI("12~"),
    [KEY_F3] = KEY_SS3_MOD_CSI("R"),
    //[KEY_F3] = KEY_CSI("13~"),
    [KEY_F4] = KEY_SS3_MOD_CSI("S"),
    //[KEY_F4] = KEY_CSI("14~"),
    [KEY_F5] = KEY_CSI("15~"),
    [KEY_F6] = KEY_CSI("17~"),
    [KEY_F7] = KEY_CSI("18~"),
    [KEY_F8] = KEY_CSI("19~"),
    [KEY_F9] = KEY_CSI("20~"),
    [KEY_F10] = KEY_CSI("21~"),
    [KEY_F11] = KEY_CSI("23~"),
    [KEY_F12] = KEY_CSI("24~"),
    [KEY_SCROLL_LOCK] = KEY_HANDLER(handle_scroll_lock),
    [KEY_INSERT] = KEY_CSI("2~"),
    [KEY_HOME] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1_1("H"), KEY_SS3("H")),
        KEY_CSI("1~")),
    [KEY_PAGEUP] = KEY_CSI("5~"),
    [KEY_DELETE] = KEY_ROUTER(get_ctrl_alt, KEY_CSI("3~"),
                              KEY_HANDLER(handle_ctrl_alt_delete)),
    [KEY_END1] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("F"), KEY_SS3("F")),
        KEY_CSI("4~")),
    [KEY_PAGEDOWN] = KEY_CSI("6~"),
    [KEY_RIGHTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("C"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("C"), KEY_SS3("C"))),
    [KEY_LEFTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("D"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("D"), KEY_SS3("D"))),
    [KEY_DOWNARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("B"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("B"), KEY_SS3("B"))),
    [KEY_UPARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("A"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("A"), KEY_SS3("A"))),
    [KEY_KEYPAD_NUM_LOCK_AND_CLEAR] = KEY_HANDLER(handle_num_lock),
    [KEY_KEYPAD_SLASH] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?o"), KEY_SS3("o")),
        KEY_CHR('/')),
    [KEY_KEYPAD_ASTERIKS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?j"), KEY_SS3("j")),
        KEY_CHR('*')),
    [KEY_KEYPAD_MINUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?m"), KEY_SS3("m")),
        KEY_CHR('-')),
    [KEY_KEYPAD_PLUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?k"), KEY_SS3("k")),
        KEY_CHR('+')),
    [KEY_KEYPAD_ENTER] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?M"), KEY_SS3("M")),
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a"))),
    [KEY_KEYPAD_1_END] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?q"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("F"), KEY_SS3("q"))),
                   KEY_CHR('1')),
    [KEY_KEYPAD_2_DOWN_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?r"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("B"), KEY_SS3("r"))),
                   KEY_CHR('2')),
    [KEY_KEYPAD_3_PAGEDN] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?s"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("6~"), KEY_SS3("s"))),
                   KEY_CHR('3')),
    [KEY_KEYPAD_4_LEFT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?t"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("D"), KEY_SS3("t"))),
                   KEY_CHR('4')),
    [KEY_KEYPAD_5] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?u"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("E"), KEY_SS3("u"))),
                   KEY_CHR('5')),
    [KEY_KEYPAD_6_RIGHT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?v"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("C"), KEY_SS3("v"))),
                   KEY_CHR('6')),
    [KEY_KEYPAD_7_HOME] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?w"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("H"), KEY_SS3("w"))),
                   KEY_CHR('7')),
    [KEY_KEYPAD_8_UP_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?x"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("A"), KEY_SS3("x"))),
                   KEY_CHR('8')),
    [KEY_KEYPAD_9_PAGEUP] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?y"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("5~"), KEY_SS3("y"))),
                   KEY_CHR('9')),
    [KEY_KEYPAD_0_INSERT] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?p"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("2~"), KEY_SS3("p"))),
                   KEY_CHR('0')),
    [KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE] = KEY_ROUTER(
        get_num,
        KEY_ROUTER(get_ctrl_alt,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?n"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("3~"), KEY_SS3("n"))),
                   KEY_HANDLER(handle_ctrl_alt_delete)),
        KEY_CHR('.')),
};

static const struct keys_entry be_entries[] = {
    [KEY_A] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('q'), KEY_CHR('Q')),
                   KEY_HANDLER(handle_ctrl_q)),
    [KEY_B] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('b'), KEY_CHR('B')),
                   KEY_CHR('\x02')),
    [KEY_C] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('c'), KEY_CHR('C')),
                   KEY_CHR('\x03')),
    [KEY_D] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('d'), KEY_CHR('D')),
                   KEY_CHR('\x04')),
    [KEY_E] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('e'), KEY_CHR('E')),
                   KEY_CHR('\x05')),
        KEY_STR("€")),
    [KEY_F] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('f'), KEY_CHR('F')),
                   KEY_CHR('\x06')),
    [KEY_G] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('g'), KEY_CHR('G')),
                   KEY_CHR('\x07')),
    [KEY_H] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('h'), KEY_CHR('H')),
                   KEY_CHR('\x08')),
    [KEY_I] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('i'), KEY_CHR('I')),
                   KEY_CHR('\x09')),
    [KEY_J] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('j'), KEY_CHR('J')),
                   KEY_CHR('\x0a')),
    [KEY_K] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('k'), KEY_CHR('K')),
                   KEY_CHR('\x0b')),
    [KEY_L] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('l'), KEY_CHR('L')),
                   KEY_CHR('\x0c')),
    [KEY_M] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR(','), KEY_CHR('?')),
                   KEY_CHR('\x1f')),
    [KEY_N] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('n'), KEY_CHR('N')),
                   KEY_CHR('\x0e')),
    [KEY_O] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('o'), KEY_CHR('O')),
                   KEY_CHR('\x0f')),
    [KEY_P] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('p'), KEY_CHR('P')),
                   KEY_CHR('\x10')),
    [KEY_Q] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('a'), KEY_CHR('A')),
                   KEY_CHR('\x01')),
    [KEY_R] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('r'), KEY_CHR('R')),
                   KEY_CHR('\x12')),
    [KEY_S] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('s'), KEY_CHR('S')),
                   KEY_HANDLER(handle_ctrl_s)),
    [KEY_T] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('t'), KEY_CHR('T')),
                   KEY_CHR('\x14')),
    [KEY_U] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('u'), KEY_CHR('U')),
                   KEY_CHR('\x15')),
    [KEY_V] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('v'), KEY_CHR('V')),
                   KEY_CHR('\x16')),
    [KEY_W] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('z'), KEY_CHR('Z')),
                   KEY_CHR('\x1a')),
    [KEY_X] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('x'), KEY_CHR('X')),
                   KEY_CHR('\x18')),
    [KEY_Y] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('y'), KEY_CHR('Y')),
                   KEY_CHR('\x19')),
    [KEY_Z] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('w'), KEY_CHR('W')),
                   KEY_CHR('\x17')),
    [KEY_1_EXCLAMATION_MARK] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('&'), KEY_CHR('1')),
        KEY_CHR('|')),
    [KEY_2_AT] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_STR("é"), KEY_CHR('2')),
                   KEY_CHR('\x00')),
        KEY_CHR('@')),
    [KEY_3_NUMBER_SIGN] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_CHR('\"'), KEY_CHR('3')),
        KEY_CHR('#')),
    [KEY_4_DOLLAR] = KEY_ROUTER(get_shift, KEY_CHR('\''), KEY_CHR('4')),
    [KEY_5_PERCENT] = KEY_ROUTER(get_shift, KEY_CHR('('), KEY_CHR('5')),
    [KEY_6_CARET] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_STR("§"), KEY_CHR('6')),
        KEY_CHR('^')),
    [KEY_7_AMPERSAND] = KEY_ROUTER(get_shift, KEY_STR("è"), KEY_CHR('7')),
    [KEY_8_ASTERISK] = KEY_ROUTER(get_shift, KEY_CHR('!'), KEY_CHR('8')),
    [KEY_9_OPARENTHESIS] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_STR("ç"), KEY_CHR('9')),
        KEY_CHR('{')),
    [KEY_0_CPARENTHESIS] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_STR("à"), KEY_CHR('0')),
        KEY_CHR('}')),
    [KEY_ENTER] =
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a")),
    [KEY_ESCAPE] = KEY_CHR('\x1b'),
    [KEY_BACKSPACE] = KEY_ROUTER(
        get_ctrl,
        KEY_ROUTER(get_backspace_mode, KEY_CHR('\x7f'), KEY_CHR('\x08')),
        KEY_CHR('\x08')),
    [KEY_TAB] = KEY_CHR('\x09'),
    [KEY_SPACEBAR] = KEY_ROUTER(get_ctrl, KEY_CHR(' '), KEY_CHR('\0')),
    [KEY_MINUS_UNDERSCORE] = KEY_ROUTER(get_shift, KEY_CHR(')'), KEY_STR("°")),
    [KEY_EQUAL_PLUS] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('-'), KEY_CHR('_')),
                   KEY_CHR('\x1f')),
    [KEY_OBRACKET_AND_OBRACE] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('^'), KEY_STR("¨")),
                   KEY_CHR('\x1b')),
        KEY_CHR('[')),
    [KEY_CBRACKET_AND_CBRACE] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('$'), KEY_CHR('*')),
                   KEY_CHR('\x1d')),
        KEY_CHR(']')),
    [KEY_BACKSLASH_VERTICAL_BAR] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_STR("µ"), KEY_STR("£")),
                   KEY_CHR('\x1e')),
        KEY_CHR('`')),
    [KEY_NONUS_BACK_SLASH_VERTICAL_BAR] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('<'), KEY_CHR('>')),
                   KEY_CHR('\x1c')),
        KEY_CHR('\\')),
    [KEY_SEMICOLON_COLON] =
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_case, KEY_CHR('m'), KEY_CHR('M')),
                   KEY_CHR('\x0d')),
    [KEY_SINGLE_AND_DOUBLE_QUOTE] = KEY_ROUTER(
        get_alt_gr, KEY_ROUTER(get_shift, KEY_STR("ù"), KEY_CHR('%')),
        KEY_STR("´")),
    [KEY_GRAVE_ACCENT_AND_TILDE] =
        KEY_ROUTER(get_shift, KEY_STR("²"), KEY_STR("³")),
    [KEY_COMMA_AND_LESS] = KEY_ROUTER(get_shift, KEY_CHR(';'), KEY_CHR('.')),
    [KEY_DOT_GREATER] = KEY_ROUTER(get_shift, KEY_CHR(':'), KEY_CHR('/')),
    [KEY_SLASH_QUESTION] = KEY_ROUTER(
        get_alt_gr,
        KEY_ROUTER(get_ctrl, KEY_ROUTER(get_shift, KEY_CHR('='), KEY_CHR('+')),
                   KEY_CHR('\x1e')),
        KEY_CHR('~')),
    [KEY_CAPS_LOCK] = KEY_HANDLER(handle_caps_lock),
    [KEY_F1] = KEY_SS3_MOD_CSI("P"),
    [KEY_F2] = KEY_SS3_MOD_CSI("Q"),
    [KEY_F3] = KEY_SS3_MOD_CSI("R"),
    [KEY_F4] = KEY_SS3_MOD_CSI("S"),
    [KEY_F5] = KEY_CSI("15~"),
    [KEY_F6] = KEY_CSI("17~"),
    [KEY_F7] = KEY_CSI("18~"),
    [KEY_F8] = KEY_CSI("19~"),
    [KEY_F9] = KEY_CSI("20~"),
    [KEY_F10] = KEY_CSI("21~"),
    [KEY_F11] = KEY_CSI("23~"),
    [KEY_F12] = KEY_CSI("24~"),
    [KEY_SCROLL_LOCK] = KEY_HANDLER(handle_scroll_lock),
    [KEY_INSERT] = KEY_CSI("2~"),
    [KEY_HOME] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1_1("H"), KEY_SS3("H")),
        KEY_CSI("1~")),
    [KEY_PAGEUP] = KEY_CSI("5~"),
    [KEY_DELETE] = KEY_ROUTER(get_ctrl_alt, KEY_CSI("3~"),
                              KEY_HANDLER(handle_ctrl_alt_delete)),
    [KEY_END1] = KEY_ROUTER(
        get_keyboard_compatibility,
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("F"), KEY_SS3("F")),
        KEY_CSI("4~")),
    [KEY_PAGEDOWN] = KEY_CSI("6~"),
    [KEY_RIGHTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("C"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("C"), KEY_SS3("C"))),
    [KEY_LEFTARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("D"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("D"), KEY_SS3("D"))),
    [KEY_DOWNARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("B"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("B"), KEY_SS3("B"))),
    [KEY_UPARROW] = KEY_ROUTER(
        get_ansi_mode, KEY_ESC("A"),
        KEY_ROUTER(get_cursor_key_mode, KEY_CSI_MOD_1("A"), KEY_SS3("A"))),
    [KEY_KEYPAD_NUM_LOCK_AND_CLEAR] = KEY_HANDLER(handle_num_lock),
    [KEY_KEYPAD_SLASH] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?o"), KEY_SS3("o")),
        KEY_CHR('/')),
    [KEY_KEYPAD_ASTERIKS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?j"), KEY_SS3("j")),
        KEY_CHR('*')),
    [KEY_KEYPAD_MINUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?m"), KEY_SS3("m")),
        KEY_CHR('-')),
    [KEY_KEYPAD_PLUS] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?k"), KEY_SS3("k")),
        KEY_CHR('+')),
    [KEY_KEYPAD_ENTER] = KEY_ROUTER(
        get_num, KEY_ROUTER(get_ansi_mode, KEY_ESC("?M"), KEY_SS3("M")),
        KEY_ROUTER(get_new_line_mode, KEY_CHR('\x0d'), KEY_STR("\x0d\x0a"))),
    [KEY_KEYPAD_1_END] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?q"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("F"), KEY_SS3("q"))),
                   KEY_CHR('1')),
    [KEY_KEYPAD_2_DOWN_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?r"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("B"), KEY_SS3("r"))),
                   KEY_CHR('2')),
    [KEY_KEYPAD_3_PAGEDN] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?s"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("6~"), KEY_SS3("s"))),
                   KEY_CHR('3')),
    [KEY_KEYPAD_4_LEFT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?t"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("D"), KEY_SS3("t"))),
                   KEY_CHR('4')),
    [KEY_KEYPAD_5] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?u"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("E"), KEY_SS3("u"))),
                   KEY_CHR('5')),
    [KEY_KEYPAD_6_RIGHT_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?v"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("C"), KEY_SS3("v"))),
                   KEY_CHR('6')),
    [KEY_KEYPAD_7_HOME] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?w"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("H"), KEY_SS3("w"))),
                   KEY_CHR('7')),
    [KEY_KEYPAD_8_UP_ARROW] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?x"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_SS3("A"), KEY_SS3("x"))),
                   KEY_CHR('8')),
    [KEY_KEYPAD_9_PAGEUP] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?y"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("5~"), KEY_SS3("y"))),
                   KEY_CHR('9')),
    [KEY_KEYPAD_0_INSERT] =
        KEY_ROUTER(get_num,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?p"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("2~"), KEY_SS3("p"))),
                   KEY_CHR('0')),
    [KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE] = KEY_ROUTER(
        get_num,
        KEY_ROUTER(get_ctrl_alt,
                   KEY_ROUTER(get_ansi_mode, KEY_ESC("?n"),
                              KEY_ROUTER(get_keyboard_compatibility,
                                         KEY_CSI("3~"), KEY_SS3("n"))),
                   KEY_HANDLER(handle_ctrl_alt_delete)),
        KEY_CHR('.')),
};

static void transmit_escape_key(struct terminal *terminal, character_t control,
                                const char *string, size_t mod_prepend_1s,
                                bool mod_force_csi) {
  size_t l = strlen(string);
  char buffer[ESCAPE_KEY_BUFFER_SIZE];
  memset(buffer, 0, ESCAPE_KEY_BUFFER_SIZE);
  size_t i = 0;

  buffer[i++] = '\033';

  if (control) {
    if (mod_force_csi &&
        (terminal->shift_state || terminal_keyboard_get_alt_state(terminal) ||
         terminal->ctrl_state))
      buffer[i++] = '[';
    else
      buffer[i++] = control;
  }

  while (l--) {
    if (control && !l) {
      uint8_t modifier = terminal->shift_state |
                         (terminal_keyboard_get_alt_state(terminal) << 1) |
                         (terminal->ctrl_state << 2);
      if (modifier) {

        if (isdigit((int)buffer[i - 1]))
          buffer[i++] = ';';

        while (mod_prepend_1s--) {
          buffer[i++] = '1';
          buffer[i++] = ';';
        }

        i += sprintf(buffer + i, "%d", modifier + 1);
      }
    }

    buffer[i++] = *(string++);
  }

  terminal_uart_transmit_string(terminal, buffer);
}

static void transmit_character_key(struct terminal *terminal,
                                   character_t character) {
  if (terminal_keyboard_get_alt_state(terminal))
    terminal_uart_transmit_character(terminal, '\033');

  terminal_uart_transmit_character(terminal, character);
}

static void transmit_string_key(struct terminal *terminal, const char *string) {
  character_t character = 0;
  while ((character = *(string++)))
    transmit_character_key(terminal, character);
}

static void handle_key(struct terminal *terminal,
                       const struct keys_entry *key) {
  const struct keys_router *router;

  switch (key->type) {
  case IGNORE:
    break;
  case CHR:
    transmit_character_key(terminal, key->data.character);
    break;
  case STR:
    transmit_string_key(terminal, key->data.string);
    break;
  case ESC:
    transmit_escape_key(terminal, 0, key->data.string, 0, false);
    break;
  case CSI:
  case CSI_MOD_1:
  case CSI_MOD_1_1:
    transmit_escape_key(
        terminal, '[', key->data.string,
        (key->type == CSI_MOD_1_1 ? 2 : (key->type == CSI_MOD_1 ? 1 : 0)),
        false);
    break;
  case SS3:
  case SS3_MOD_CSI:
    transmit_escape_key(terminal, 'O', key->data.string,
                        key->type == SS3_MOD_CSI ? 1 : 0,
                        key->type == SS3_MOD_CSI);
    break;
  case HANDLER:
    key->data.handler(terminal);
    break;
  case ROUTER:
    router = key->data.router;
    handle_key(terminal, &router->entries[router->router(terminal)]);
    break;
  }
}

void terminal_keyboard_handle_key(struct terminal *terminal, bool shift,
                                  bool lalt, bool ralt, bool ctrl,
                                  uint8_t key_code) {
  terminal->shift_state = shift;
  terminal->lalt_state = lalt;
  terminal->ralt_state = ralt;
  terminal->ctrl_state = ctrl;

  if (terminal->keyboard_action_mode)
    return;

  if (terminal->pressed_key_code == key_code)
    return;

  terminal->pressed_key_code = key_code;
  terminal->repeat_pressed_key = false;

  if (terminal->auto_repeat_mode && key_code != KEY_NONE &&
      key_code != KEY_ESCAPE && key_code != KEY_TAB && key_code != KEY_ENTER &&
      key_code != KEY_CAPS_LOCK && key_code != KEY_KEYPAD_NUM_LOCK_AND_CLEAR &&
      key_code != KEY_SCROLL_LOCK && !terminal->ctrl_state) {
    terminal->repeat_counter = FIRST_REPEAT_COUNTER;
  } else {
    terminal->repeat_counter = 0;
  }

  handle_key(terminal, &terminal->keys_entries[key_code]);
}

void terminal_keyboard_update_repeat_counter(struct terminal *terminal) {
  if (terminal->repeat_counter) {
    terminal->repeat_counter--;

    if (!terminal->repeat_counter && !terminal->repeat_pressed_key)
      terminal->repeat_pressed_key = true;
  }
}

void terminal_keyboard_repeat_key(struct terminal *terminal) {
  if (terminal->repeat_pressed_key) {
    terminal->repeat_counter = NEXT_REPEAT_COUNTER;
    terminal->repeat_pressed_key = false;

    handle_key(terminal, &terminal->keys_entries[terminal->pressed_key_code]);
  }
}

void terminal_keyboard_init(struct terminal *terminal,
                            enum keyboard_layout keyboard_layout) {
  terminal->pressed_key_code = 0;
  terminal->repeat_counter = 0;
  terminal->repeat_pressed_key = false;

  switch (keyboard_layout) {
  case KEYBOARD_LAYOUT_UK:
    terminal->keys_entries = uk_entries;
    terminal->alt_gr = true;
    break;
  case KEYBOARD_LAYOUT_DE:
    terminal->keys_entries = de_entries;
    terminal->alt_gr = true;
    break;
  case KEYBOARD_LAYOUT_SE:
    terminal->keys_entries = se_entries;
    terminal->alt_gr = true;
    break;
  case KEYBOARD_LAYOUT_FR:
    terminal->keys_entries = fr_entries;
    terminal->alt_gr = true;
    break;
  case KEYBOARD_LAYOUT_BE:
    terminal->keys_entries = be_entries;
    terminal->alt_gr = true;
    break;
  case KEYBOARD_LAYOUT_US:
  default:
    terminal->keys_entries = us_entries;
    terminal->alt_gr = false;
    break;
  }
}

void terminal_keyboard_set_keys_entries(struct terminal *terminal,
                                        const struct keys_entry *keys_entries,
                                        bool alt_gr) {
  terminal_keyboard_handle_key(terminal, false, false, false, false, KEY_NONE);
  terminal->keys_entries = keys_entries;
  terminal->alt_gr = alt_gr;
}

bool terminal_keyboard_get_alt_state(struct terminal *terminal) {
  if (terminal->alt_gr)
    return terminal->lalt_state;

  return terminal->lalt_state || terminal->ralt_state;
}