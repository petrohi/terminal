#pragma once

#include <stdbool.h>
#include <stdint.h>

struct format {
  uint8_t rows;
  uint8_t cols;
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

enum word_length {
  WORD_LENGTH_8B = 0,
  WORD_LENGTH_9B = 1,
};

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
  C1_MODE_7BIT,
  C1_MODE_8BIT,
};

enum charset {
  CHARSET_ASCII,
  CHARSET_UTF8,
};

enum start_up {
  START_UP_NONE,
  START_UP_MESSAGE,
  START_UP_TEST_FONT1,
  START_UP_TEST_FONT2,
  START_UP_TEST_COLOR1,
  START_UP_TEST_COLOR2,
};

struct terminal_config {
  struct format format;
  bool monochrome;

  enum baud_rate baud_rate;
  enum word_length word_length;
  enum stop_bits stop_bits;
  enum parity parity;

  enum charset charset;
  enum c1_mode c1_mode;

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
