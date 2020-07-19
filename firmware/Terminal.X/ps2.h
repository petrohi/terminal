#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_PRESSED_KEYS 6

struct ps2 {
  volatile uint8_t lshift:1;
  volatile uint8_t rshift:1;
  volatile uint8_t lctrl:1;
  volatile uint8_t rctrl:1;
  volatile uint8_t lalt:1;
  volatile uint8_t ralt:1;
  volatile uint8_t lgui:1;
  volatile uint8_t rgui:1;

  volatile uint8_t keys[MAX_PRESSED_KEYS];
  
  bool keyup;
  bool e0;
};

void ps2_init(struct ps2 *ps2);

void ps2_handle_scancode(struct ps2 *ps2, uint8_t scancode);
