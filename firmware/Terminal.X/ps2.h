#pragma once

#include <stdint.h>
#include <stdbool.h>

#define PS2_MAX_PRESSED_KEYS 6
#define PS2_ECHO_ACK 0xee
#define PS2_COMMAND_ACK 0xfa
#define PS2_COMMAND_RESEND 0xfe

struct ps2 {
  volatile uint8_t lshift : 1;
  volatile uint8_t rshift : 1;
  volatile uint8_t lctrl : 1;
  volatile uint8_t rctrl : 1;
  volatile uint8_t lalt : 1;
  volatile uint8_t ralt : 1;
  volatile uint8_t lgui : 1;
  volatile uint8_t rgui : 1;

  volatile uint8_t keys[PS2_MAX_PRESSED_KEYS];
  volatile uint8_t response;

  volatile bool keyup;
  volatile bool e0;
};

void ps2_init(struct ps2 *ps2);

void ps2_handle_code(struct ps2 *ps2, uint8_t scancode);
