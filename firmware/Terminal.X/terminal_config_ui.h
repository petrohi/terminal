#pragma once

#include "terminal.h"

struct terminal_ui_choice {
  const char *title;
};

struct terminal_config_ui;

struct terminal_ui_option {
  const char *title;
  size_t (*current)(struct terminal_config_ui *terminal_config_ui);
  void (*change)(struct terminal_config_ui *terminal_config_ui, size_t index);
  const struct terminal_ui_choice (*choices)[];
};

struct terminal_ui_menu {
  const char *title;
  const struct terminal_ui_option (*options)[];
};

struct terminal_config_ui {
  struct terminal *terminal;
  struct terminal_config *terminal_config;
  struct terminal_config terminal_config_copy;
  bool activated;
  uint8_t previous_key;

  const struct terminal_ui_menu *current_menu;
  const struct terminal_ui_option *current_option;
  const struct terminal_ui_choice *current_choice;
};

void terminal_config_ui_init(struct terminal_config_ui *terminal_config_ui,
                             struct terminal *terminal,
                             struct terminal_config *terminal_config);
void terminal_config_ui_enter(struct terminal_config_ui *terminal_config_ui);
void terminal_config_ui_handle_key(
    struct terminal_config_ui *terminal_config_ui, uint8_t key);
