#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "terminal_config.h"

struct lock_state {
  uint8_t caps : 1;
  uint8_t scroll : 1;
  uint8_t num : 1;
};

enum font {
  FONT_NORMAL = 0,
  FONT_BOLD = 1,
  FONT_THIN = 2,
};

enum scroll { SCROLL_UP, SCROLL_DOWN };
typedef uint8_t color_t;
typedef uint8_t character_t;
typedef uint16_t codepoint_t;

#define DEFAULT_ACTIVE_COLOR 0xf
#define DEFAULT_INACTIVE_COLOR 0
#define CHARACTER_MAX 0xff
#define CHARACTER_DECODER_TABLE_LENGTH CHARACTER_MAX + 1

#define MAX_COLS 80
#define MAX_ROWS 24

enum screen_test {
  SCREEN_TEST_FONT1,
  SCREEN_TEST_FONT2,
  SCREEN_TEST_COLOR1,
  SCREEN_TEST_COLOR2,
};

struct terminal_callbacks {
  void (*keyboard_set_leds)(struct lock_state state);
  void (*uart_transmit)(character_t *characters, size_t size, size_t head);
  void (*screen_draw_codepoint)(struct format format, size_t row, size_t col,
                                codepoint_t codepoint, enum font font,
                                bool italic, bool underlined, bool crossedout,
                                color_t active, color_t inactive);
  void (*screen_clear_rows)(struct format format, size_t from_row,
                            size_t to_row, color_t inactive);
  void (*screen_clear_cols)(struct format format, size_t row, size_t from_col,
                            size_t to_col, color_t inactive);
  void (*screen_scroll)(struct format format, enum scroll scroll,
                        size_t from_row, size_t to_row, size_t rows,
                        color_t inactive);
  void (*screen_shift_right)(struct format format, size_t row, size_t col,
                             size_t cols, color_t inactive);
  void (*screen_shift_left)(struct format format, size_t row, size_t col,
                            size_t cols, color_t inactive);
  void (*screen_test)(struct format format, enum screen_test screen_test);
  void (*system_yield)();
  void (*system_reset)();
  void (*system_write_config)(struct terminal_config *terminal_config_copy);
};

struct terminal;

#define DEFAULT_RECEIVE CHARACTER_MAX

typedef void (*receive_t)(struct terminal *, character_t);
typedef receive_t receive_table_t[CHARACTER_DECODER_TABLE_LENGTH];

typedef codepoint_t
    codepoint_transformation_table_t[CHARACTER_DECODER_TABLE_LENGTH];

#define ESC_MAX_PARAMS_COUNT 16
#define ESC_MAX_PARAM_LENGTH 16

struct visual_props {
  uint8_t font : 2;
  uint8_t blink : 1;
  uint8_t italic : 1;
  uint8_t underlined : 1;
  uint8_t negative : 1;
  uint8_t concealed : 1;
  uint8_t crossedout : 1;

  color_t active_color;
  color_t inactive_color;
};

struct visual_cell {
  codepoint_t c;
  struct visual_props p;
};

enum gset {
  GSET_UNDEFINED = 0,
  GSET_G0 = 1,
  GSET_G1 = 2,
  GSET_G2 = 3,
  GSET_G3 = 4,
  GSET_MAX = 4,
};

enum xon_off {
  XON,
  XOFF,
};

struct visual_state {
  int16_t cursor_row;
  int16_t cursor_col;
  bool cursor_last_col;

  struct visual_props p;

  enum gset gset_gl;
  const codepoint_transformation_table_t *gset_table[GSET_MAX];
};

#define MAX_CONTROL_DATA_LENGTH 64

struct control_data {
  character_t data[MAX_CONTROL_DATA_LENGTH];
  size_t length;
};

struct terminal {
  const struct terminal_callbacks *callbacks;

  struct format format;
  bool monochrome;

  uint8_t pressed_key_code;
  volatile uint16_t repeat_counter;
  volatile bool repeat_pressed_key;

  struct lock_state lock_state;

  uint8_t shift_state : 1;
  uint8_t alt_state : 1;
  uint8_t ctrl_state : 1;

  enum charset charset;
  enum c1_mode c1_mode;

  bool auto_wrap_mode;
  bool scrolling_mode; // TODO
  bool column_mode;    // TODO
  bool screen_mode;
  bool origin_mode;
  bool insert_mode;

  bool send_receive_mode;

  bool new_line_mode;
  bool cursor_key_mode;
  bool keyboard_action_mode;
  bool auto_repeat_mode;
  bool ansi_mode;
  bool backspace_mode;

  struct visual_state vs;
  struct visual_state saved_vs;

  int16_t margin_top;
  int16_t margin_bottom;

  bool tab_stops[MAX_COLS];

  volatile uint16_t cursor_counter;
  volatile bool cursor_on;
  bool cursor_drawn;

  volatile uint16_t blink_counter;
  volatile bool blink_on;
  bool blink_drawn;

  struct visual_cell cells[MAX_ROWS * MAX_COLS];

  const receive_table_t *receive_table;

  character_t esc_params[ESC_MAX_PARAMS_COUNT][ESC_MAX_PARAM_LENGTH];
  size_t esc_params_count;
  size_t esc_last_param_length;

  character_t vt52_move_cursor_row;

  character_t *transmit_buffer;
  size_t transmit_buffer_size;
  size_t transmit_buffer_head;

  size_t utf8_codepoint_length;
  size_t utf8_buffer_length;
  character_t utf8_buffer[4];

  struct control_data dcs;
  struct control_data osc;
  struct control_data apc;
  struct control_data pm;

  enum gset gset_received;
  enum xon_off xon_off;

#ifdef DEBUG
#define DEBUG_BUFFER_LENGTH 128
  character_t debug_buffer[DEBUG_BUFFER_LENGTH];
  uint8_t debug_buffer_length;
  bool unhandled;
#endif
};

void terminal_init(struct terminal *terminal,
                   const struct terminal_callbacks *callbacks,
                   const struct terminal_config *config,
                   character_t *transmit_buffer, size_t transmit_buffer_size);
void terminal_keyboard_handle_key(struct terminal *terminal, uint8_t key);
void terminal_keyboard_handle_shift(struct terminal *terminal, bool shift);
void terminal_keyboard_handle_alt(struct terminal *terminal, bool alt);
void terminal_keyboard_handle_ctrl(struct terminal *terminal, bool ctrl);

void terminal_uart_receive_character(struct terminal *terminal,
                                     character_t character);
void terminal_uart_receive_string(struct terminal *terminal,
                                  const char *string);

void terminal_uart_transmit_character(struct terminal *terminal,
                                      character_t character);
void terminal_uart_transmit_string(struct terminal *terminal,
                                   const char *string);
void terminal_uart_transmit_printf(struct terminal *terminal,
                                   const char *format, ...);

void terminal_uart_xon_off(struct terminal *terminal, enum xon_off xon_off);

void terminal_timer_tick(struct terminal *terminal);
void terminal_screen_update(struct terminal *terminal);
void terminal_keyboard_repeat_key(struct terminal *terminal);
