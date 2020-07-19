#include "terminal_internal.h"

#define PRODUCT_NAME                                                           \
  "\33[1;91mA\33[92mS\33[93mC\33[94mI\33[95mI\33[39m Terminal\33[m\r\n"
#define PRODUCT_VERSION "Version 3.0-alpha\r\n"
#define PRODUCT_COPYRIGHT "Copyright (C) 2020 Peter Hizalev\r\n"

void terminal_timer_tick(struct terminal *terminal) {
  terminal_keyboard_update_repeat_counter(terminal);
  terminal_screen_update_cursor_counter(terminal);
  terminal_screen_update_blink_counter(terminal);
}

void terminal_init(struct terminal *terminal,
                   const struct terminal_callbacks *callbacks,
                   const struct terminal_config *config,
                   character_t *transmit_buffer, size_t transmit_buffer_size) {
  terminal->callbacks = callbacks;

  terminal->format = config->format;

  if (terminal->format.rows > MAX_ROWS)
    terminal->format.rows = MAX_ROWS;

  if (terminal->format.cols > MAX_COLS)
    terminal->format.cols = MAX_COLS;

  terminal->transmit_buffer = transmit_buffer;
  terminal->transmit_buffer_size = transmit_buffer_size;
  terminal->transmit_buffer_head = 0;

  terminal->charset = config->charset;
  terminal->c1_mode = config->c1_mode;

  terminal->auto_wrap_mode = config->auto_wrap_mode;
  terminal->scrolling_mode = false;
  terminal->column_mode = false;
  terminal->screen_mode = config->screen_mode;
  terminal->origin_mode = false;
  terminal->insert_mode = false;

  terminal->send_receive_mode = config->send_receive_mode;

  terminal->new_line_mode = config->new_line_mode;
  terminal->cursor_key_mode = config->cursor_key_mode;
  terminal->keyboard_action_mode = false;
  terminal->auto_repeat_mode = config->auto_repeat_mode;
  terminal->ansi_mode = config->ansi_mode;
  terminal->backspace_mode = config->backspace_mode;

  terminal_keyboard_init(terminal);
  terminal_screen_init(terminal);
  terminal_uart_init(terminal);

  terminal->saved_vs = terminal->vs;

  switch (config->start_up) {
  case START_UP_NONE:
    break;
  case START_UP_MESSAGE:
    terminal_uart_receive_string(
        terminal, PRODUCT_NAME PRODUCT_VERSION PRODUCT_COPYRIGHT "\r\n");
    break;
  case START_UP_TEST_COLOR1:
    terminal->callbacks->screen_test(terminal->format, SCREEN_TEST_COLOR1);
    break;
  case START_UP_TEST_COLOR2:
    terminal->callbacks->screen_test(terminal->format, SCREEN_TEST_COLOR2);
    break;
  case START_UP_TEST_FONT1:
    terminal->callbacks->screen_test(terminal->format, SCREEN_TEST_FONT1);
    break;
  case START_UP_TEST_FONT2:
    terminal->callbacks->screen_test(terminal->format, SCREEN_TEST_FONT2);
    break;
  }
}
