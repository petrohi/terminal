#pragma once

#include <stdbool.h>
#include <stdint.h>

enum format_rows {
  FORMAT_24_ROWS = 0,
  FORMAT_30_ROWS = 1,
};

enum baud_rate {
  BAUD_RATE_110 = 0,
  BAUD_RATE_150 = 1,
  BAUD_RATE_300 = 2,
  BAUD_RATE_1200 = 3,
  BAUD_RATE_2400 = 4,
  BAUD_RATE_4800 = 5,
  BAUD_RATE_9600 = 6,
  BAUD_RATE_19200 = 7,
  BAUD_RATE_38400 = 8,
  BAUD_RATE_57600 = 9,
  BAUD_RATE_115200 = 10,
  BAUD_RATE_230400 = 11,
  BAUD_RATE_460800 = 12,
  BAUD_RATE_921600 = 13,
};

#ifdef TERMINAL_SERIAL_WORD_LENGTH
enum word_length {
  WORD_LENGTH_8B = 0,
  WORD_LENGTH_9B = 1,
};
#endif

enum stop_bits {
  STOP_BITS_1 = 0,
  STOP_BITS_2 = 1,
};

enum parity {
  PARITY_NONE = 0,
  PARITY_EVEN = 1,
  PARITY_ODD = 2,
};

enum c1_mode {
  C1_MODE_7BIT = 0,
  C1_MODE_8BIT = 1,
};

enum charset {
  CHARSET_UTF8 = 0,
  CHARSET_ISO_8859_1 = 1,
  CHARSET_IBM_PC = 2,
};

enum start_up {
  START_UP_NONE = 0,
  START_UP_MESSAGE = 1,
  START_UP_TEST_FONT1 = 2,
  START_UP_TEST_FONT2 = 3,
#ifdef TERMINAL_8BIT_COLOR
  START_UP_TEST_COLOR1 = 4,
  START_UP_TEST_COLOR2 = 5,
#endif
};

struct terminal_config {
  enum format_rows format_rows;
  bool monochrome;

  enum baud_rate baud_rate;
#ifdef TERMINAL_SERIAL_WORD_LENGTH
  enum word_length word_length;
#endif
  enum stop_bits stop_bits;
  enum parity parity;
  bool flow_control;
#ifdef TERMINAL_SERIAL_INVERTED
  bool serial_inverted;
#endif

  enum charset charset;
  enum c1_mode receive_c1_mode;
  enum c1_mode transmit_c1_mode;

  bool auto_wrap_mode;
  bool screen_mode;

  bool send_receive_mode;

  bool new_line_mode;
  bool cursor_key_mode;
  bool auto_repeat_mode;
  bool ansi_mode;
  bool backspace_mode;

  enum start_up start_up;
};

uint32_t terminal_config_get_baud_rate(struct terminal_config *terminal_config);
