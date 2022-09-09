#pragma once

#include "terminal.h"

struct terminal_ui_choice {
  const char *title;
};

struct terminal_config_ui;

struct terminal_ui_option {
  const char *title;
  void *(*getptr)(struct terminal_config_ui *terminal_config_ui);
  const struct terminal_ui_choice (*choices)[];
};

struct terminal_ui_menu {
  const char *title;
  const struct terminal_ui_option (*options)[];
};

struct terminal_config_ui {
  struct terminal *terminal;
  const volatile struct terminal_config *terminal_config;
  struct terminal_config terminal_config_copy;
  bool activated;

  const struct terminal_ui_menu *current_menu;
  const struct terminal_ui_option *current_option;
  const struct terminal_ui_choice *current_choice;
};

void terminal_config_ui_init(
    struct terminal_config_ui *terminal_config_ui, struct terminal *terminal,
    const volatile struct terminal_config *terminal_config);
void terminal_config_ui_activate(struct terminal_config_ui *terminal_config_ui);
