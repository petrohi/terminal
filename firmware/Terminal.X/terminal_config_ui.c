#include "terminal_config_ui.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "terminal_internal.h"
#include "terminal_keyboard.h"

#define PRINTF_BUFFER_SIZE 256

static void *
get_format_rows_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.format_rows;
}

#ifndef TERMINAL_8BIT_COLOR
static void *
get_monochrome_transform_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.monochrome_transform;
}
#endif

static void *get_baud_rate_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.baud_rate;
}

#ifdef TERMINAL_SERIAL_WORD_LENGTH
static void *
get_word_length_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.word_length;
}
#endif

#ifdef TERMINAL_SERIAL_INVERTED
static void *
get_serial_inverted_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.serial_inverted;
}
#endif

static void *get_stop_bits_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.stop_bits;
}

static void *get_parity_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.parity;
}

static void *get_start_up_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.start_up;
}

static void *get_charset_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.charset;
}

static void *
get_keyboard_compatibility_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.keyboard_compatibility;
}

static void *
get_keyboard_layout_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.keyboard_layout;
}

static void *
get_flow_control_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.flow_control;
}

static void *
current_receive_c1_mode(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.receive_c1_mode;
}

static void *
current_transmit_c1_mode(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.transmit_c1_mode;
}

static void *
get_auto_wrap_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.auto_wrap_mode;
}

static void *
get_screen_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.screen_mode;
}

static void *
get_send_receive_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.send_receive_mode;
}

static void *
get_new_line_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.new_line_mode;
}

static void *
get_cursor_key_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.cursor_key_mode;
}

static void *
get_auto_repeat_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.auto_repeat_mode;
}

static void *get_ansi_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.ansi_mode;
}

static void *
get_backspace_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.backspace_mode;
}

static void *
get_application_keypad_mode_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.application_keypad_mode;
}

static void *
get_bell_buzzer_ptr(struct terminal_config_ui *terminal_config_ui) {
  return &terminal_config_ui->terminal_config_copy.bell_buzzer;
}

static char get_option_value(struct terminal_config_ui *terminal_config_ui,
                             const struct terminal_ui_option *option) {
  char *ptr = (char *)option->getptr(terminal_config_ui);
  return *ptr;
}

static void set_option_value(struct terminal_config_ui *terminal_config_ui,
                             const struct terminal_ui_option *option,
                             char value) {
  char *ptr = (char *)option->getptr(terminal_config_ui);
  *ptr = value;
}

static const struct terminal_ui_choice off_on_choices[] = {
    {"off"},
    {"on"},
    {NULL},
};

static const struct terminal_ui_choice c1_mode_choices[] = {
    {"S7C1T"},
    {"S8C1T"},
    {NULL},
};

static const struct terminal_ui_menu menus[] = {
    {"General",
     &(const struct terminal_ui_option[]){{"Start up", get_start_up_ptr,
                                           &(const struct terminal_ui_choice[]){
                                               {"none"},
                                               {"message"},
                                               {"font test1"},
                                               {"font test2"},
#ifdef TERMINAL_8BIT_COLOR
                                               {"color test1"},
                                               {"color test2"},
#endif
                                               {NULL},
                                           }},
                                          {"Keyboard layout",
                                           get_keyboard_layout_ptr,
                                           &(const struct terminal_ui_choice[]){
                                               {"US"},
                                               {"UK"},
                                               {"DE"},
                                               {"SE"},
                                               {"FR"},
                                               {"BE"},
                                               {NULL},
                                           }},
                                          {NULL}}},
    {"Serial",
     &(const struct terminal_ui_option[]){
         {"Baud rate", get_baud_rate_ptr,
          &(const struct terminal_ui_choice[]){
              [BAUD_RATE_110] = {"110"},
              [BAUD_RATE_150] = {"150"},
              [BAUD_RATE_300] = {"300"},
              [BAUD_RATE_1200] = {"1200"},
              [BAUD_RATE_2400] = {"2400"},
              [BAUD_RATE_4800] = {"4800"},
              [BAUD_RATE_9600] = {"9600"},
              [BAUD_RATE_19200] = {"19200"},
              [BAUD_RATE_38400] = {"38400"},
              [BAUD_RATE_57600] = {"57600"},
              [BAUD_RATE_115200] = {"115200"},
              [BAUD_RATE_230400] = {"230400"},
              [BAUD_RATE_460800] = {"460800"},
              [BAUD_RATE_921600] = {"921600"},
              [BAUD_RATE_1500000] = {"1500000"},
              {NULL},
          }},
#ifdef TERMINAL_SERIAL_WORD_LENGTH
         {"Word length", get_word_length_ptr,
          &(const struct terminal_ui_choice[]){[WORD_LENGTH_8B] = {"8 bits"},
                                               [WORD_LENGTH_9B] = {"9 bits"},
                                               {NULL}}},
#endif
#ifdef TERMINAL_SERIAL_INVERTED
         {"Signal levels", get_serial_inverted_ptr,
          &(const struct terminal_ui_choice[]){
              {"TTL"},
              {"RS232"},
              {NULL},
          }},
#endif
         {"Stop bits", get_stop_bits_ptr,
          &(const struct terminal_ui_choice[]){
              [STOP_BITS_1] = {"1 bit"}, [STOP_BITS_2] = {"2 bits"}, {NULL}}},
         {"Parity", get_parity_ptr,
          &(const struct terminal_ui_choice[]){[PARITY_NONE] = {"none"},
                                               [PARITY_EVEN] = {"even"},
                                               [PARITY_ODD] = {"odd"},
                                               {NULL}}},
         {NULL}}},
    {"Terminal",
     &(const struct terminal_ui_option[]){
         {"Character set", get_charset_ptr,
          &(const struct terminal_ui_choice[]){
              {"UTF8"},
              {"ISO 8859-1"},
              {"IBM PC 437"},
              {NULL},
          }},
         {"Keyboard compatibility", get_keyboard_compatibility_ptr,
          &(const struct terminal_ui_choice[]){
              {"PC/Sun"},
              {"VT220"},
              {NULL},
          }},
         {"XOFF/XON flow control", get_flow_control_ptr, &off_on_choices},
         {"Receive controls", current_receive_c1_mode, &c1_mode_choices},
         {"Transmit controls", current_transmit_c1_mode, &c1_mode_choices},
         {"Send/receive mode (SRM)", get_send_receive_mode_ptr,
          &(const struct terminal_ui_choice[]){
              {"echo"},
              {"normal"},
              {NULL},
          }},
         {"New line mode (LNM)", get_new_line_mode_ptr, &off_on_choices},
         {"Cursor key mode (DECCKM)", get_cursor_key_mode_ptr,
          &(const struct terminal_ui_choice[]){
              {"normal"},
              {"application"},
              {NULL},
          }},
         {"Keypad mode (DECNKM)", get_application_keypad_mode_ptr,
          &off_on_choices},
         {"ANSI mode (DECANM)", get_ansi_mode_ptr,
          &(const struct terminal_ui_choice[]){
              {"VT52"},
              {"ANSI"},
              {NULL},
          }},
         {"Auto-wraparound mode (DECAWM)", get_auto_wrap_mode_ptr,
          &off_on_choices},
         {"Auto-repeat keys mode (DECARM)", get_auto_repeat_mode_ptr,
          &off_on_choices},
         {"Screen mode (DECSCNM)", get_screen_mode_ptr,
          &(const struct terminal_ui_choice[]){
              {"normal"},
              {"inverse"},
              {NULL},
          }},
         {"Backspace mode (DECBKM)", get_backspace_mode_ptr, &off_on_choices},
         {"Bell buzzer", get_bell_buzzer_ptr, &off_on_choices},
         {NULL}}},
    {"Video",
     &(const struct terminal_ui_option[]){
         {"Number of lines", get_format_rows_ptr,
          &(const struct terminal_ui_choice[]){
              {"24"},
              {"30"},
              {NULL},
          }},
#ifndef TERMINAL_8BIT_COLOR
         {"Monochrome transformation", get_monochrome_transform_ptr,
          &(const struct terminal_ui_choice[]){
              {"Simple"},
              {"Luminance"},
              {NULL},
          }},
#endif
         {NULL}}},
    {NULL}};

void screen_printf(struct terminal_config_ui *terminal_config_ui,
                   const char *format, ...) {
  va_list args;
  va_start(args, format);

  char buffer[PRINTF_BUFFER_SIZE];
  vsnprintf(buffer, PRINTF_BUFFER_SIZE, format, args);
  terminal_uart_receive_string(terminal_config_ui->terminal, buffer);

  va_end(args);
}

void terminal_config_ui_init(
    struct terminal_config_ui *terminal_config_ui, struct terminal *terminal,
    const volatile struct terminal_config *terminal_config) {
  terminal_config_ui->terminal = terminal;
  terminal_config_ui->terminal_config = terminal_config;

  terminal_config_ui->activated = false;

  terminal_config_ui->current_menu = &menus[0];
  terminal_config_ui->current_option =
      &(*terminal_config_ui->current_menu->options)[0];
  terminal_config_ui->current_choice = NULL;
}

static void clear_screen(struct terminal_config_ui *terminal_config_ui) {
  screen_printf(terminal_config_ui, "\x1b[2J");
}

static void move_cursor(struct terminal_config_ui *terminal_config_ui,
                        size_t row, size_t col) {
  screen_printf(terminal_config_ui, "\x1b[%d;%dH", row, col);
}

static const char *const choice_help[] = {
    "↑↓ - Change value",
    "<Enter> - OK",
    "<Esc> - Cancel",
    NULL,
};

static const char *const default_help[] = {
    "←→ Select menu",           "↑↓ - Select option",
    "<Enter> - Edit option",    "<Esc> - Discard and restart",
    "<F12> - Save and restart", NULL,
};

#define UI_ROWS terminal_config_ui->terminal->format.rows
#define UI_COLS terminal_config_ui->terminal->format.cols

#define TITLE_ROW 2
#define MENU_ROW 4
#define TOP_ROW 5
#define MAIN_ROW 8
#define BOTTOM_ROW UI_ROWS

#define LEFT_COL 1
#define OPTIONS_COL 4
#define CHOICES_COL 35
#define DIVIDER_COL 50
#define HELP_COL 52
#define RIGHT_COL UI_COLS

static void clear_help(struct terminal_config_ui *terminal_config_ui) {
  const char *const *help =
      terminal_config_ui->current_choice ? &choice_help[0] : &default_help[0];

  size_t i = 0;
  while (*help) {
    move_cursor(terminal_config_ui, i + MAIN_ROW, HELP_COL);
    screen_printf(terminal_config_ui, "\x1b[%dX", UI_COLS - HELP_COL);
    help++;
    i++;

    terminal_config_ui->terminal->callbacks->yield();
  }
}

static void render_help(struct terminal_config_ui *terminal_config_ui) {
  const char *const *help =
      terminal_config_ui->current_choice ? &choice_help[0] : &default_help[0];

  size_t i = 0;
  while (*help) {
    move_cursor(terminal_config_ui, i + MAIN_ROW, HELP_COL);
    screen_printf(terminal_config_ui, *help);
    help++;
    i++;

    terminal_config_ui->terminal->callbacks->yield();
  }
}

static void render_menu(struct terminal_config_ui *terminal_config_ui) {
  const struct terminal_ui_menu *menu = &menus[0];

  size_t col = 1;
  while (menu->title) {
    bool current_menu = menu == terminal_config_ui->current_menu;
    move_cursor(terminal_config_ui, MENU_ROW, col);
    screen_printf(terminal_config_ui, "\x1b[%dm%s\x1b[27m",
                  current_menu ? 7 : 27, menu->title);

    col += (strlen(menu->title) + 4);
    menu++;

    terminal_config_ui->terminal->callbacks->yield();
  }
}

static void render_borders(struct terminal_config_ui *terminal_config_ui) {
  move_cursor(terminal_config_ui, TOP_ROW, LEFT_COL);
  screen_printf(terminal_config_ui, "╔");
  move_cursor(terminal_config_ui, TOP_ROW, RIGHT_COL);
  screen_printf(terminal_config_ui, "╗");

  for (size_t row = TOP_ROW + 1; row <= BOTTOM_ROW - 1; row++) {
    move_cursor(terminal_config_ui, row, LEFT_COL);
    screen_printf(terminal_config_ui, "║");
    move_cursor(terminal_config_ui, row, DIVIDER_COL);
    screen_printf(terminal_config_ui, "│");
    move_cursor(terminal_config_ui, row, RIGHT_COL);
    screen_printf(terminal_config_ui, "║");

    terminal_config_ui->terminal->callbacks->yield();
  }

  move_cursor(terminal_config_ui, UI_ROWS, LEFT_COL);
  screen_printf(terminal_config_ui, "╚");
  move_cursor(terminal_config_ui, UI_ROWS, RIGHT_COL);
  screen_printf(terminal_config_ui, "╝");

  move_cursor(terminal_config_ui, TOP_ROW, LEFT_COL + 1);
  for (size_t col = LEFT_COL + 1; col <= RIGHT_COL - 1; col++) {
    if (col == DIVIDER_COL)
      screen_printf(terminal_config_ui, "╤");
    else
      screen_printf(terminal_config_ui, "═");

    terminal_config_ui->terminal->callbacks->yield();
  }

  move_cursor(terminal_config_ui, BOTTOM_ROW, LEFT_COL + 1);
  for (size_t col = LEFT_COL + 1; col <= RIGHT_COL - 1; col++) {
    if (col == DIVIDER_COL)
      screen_printf(terminal_config_ui, "╧");
    else
      screen_printf(terminal_config_ui, "═");

    terminal_config_ui->terminal->callbacks->yield();
  }
}

static void render_options(struct terminal_config_ui *terminal_config_ui) {
  size_t i = 0;
  const struct terminal_ui_option *option =
      &(*terminal_config_ui->current_menu->options)[0];

  while (option->title) {
    bool current_option = option == terminal_config_ui->current_option;

    move_cursor(terminal_config_ui, i + MAIN_ROW, OPTIONS_COL);
    screen_printf(terminal_config_ui, "\x1b[%dm%s:\x1b[27m",
                  !terminal_config_ui->current_choice && current_option ? 7
                                                                        : 27,
                  option->title);

    move_cursor(terminal_config_ui, i + MAIN_ROW, CHOICES_COL);

    const struct terminal_ui_choice *choice =
        terminal_config_ui->current_choice && current_option
            ? terminal_config_ui->current_choice
            : &(*option->choices)[get_option_value(terminal_config_ui, option)];

    screen_printf(terminal_config_ui, "\x1b[%dm[%11s]\x1b[27m",
                  terminal_config_ui->current_choice && current_option ? 7 : 27,
                  choice->title);

    i++;
    option++;

    terminal_config_ui->terminal->callbacks->yield();
  }
}

static void render_screen(struct terminal_config_ui *terminal_config_ui) {
  move_cursor(terminal_config_ui, TITLE_ROW, 30);
  screen_printf(terminal_config_ui, "\x1b[1mASCII TERMINAL SETUP\x1b[22m");

  render_menu(terminal_config_ui);
  render_borders(terminal_config_ui);
  render_options(terminal_config_ui);
  render_help(terminal_config_ui);
}

static void prev_option(struct terminal_config_ui *terminal_config_ui) {
  if (terminal_config_ui->current_choice) {
    const struct terminal_ui_choice *first_choice =
        &(*terminal_config_ui->current_option->choices)[0];
    if (terminal_config_ui->current_choice != first_choice)
      terminal_config_ui->current_choice--;
    else {
      terminal_config_ui->current_choice = first_choice;
      while ((terminal_config_ui->current_choice + 1)->title)
        terminal_config_ui->current_choice++;
    }
  } else {
    const struct terminal_ui_option *first_option =
        &(*terminal_config_ui->current_menu->options)[0];
    if (terminal_config_ui->current_option != first_option)
      terminal_config_ui->current_option--;
    else {
      terminal_config_ui->current_option = first_option;
      while ((terminal_config_ui->current_option + 1)->title)
        terminal_config_ui->current_option++;
    }
  }

  render_options(terminal_config_ui);
}

static void next_option(struct terminal_config_ui *terminal_config_ui) {
  if (terminal_config_ui->current_choice) {
    terminal_config_ui->current_choice++;

    if (!terminal_config_ui->current_choice->title)
      terminal_config_ui->current_choice =
          &(*terminal_config_ui->current_option->choices)[0];
  } else {
    terminal_config_ui->current_option++;

    if (!terminal_config_ui->current_option->title)
      terminal_config_ui->current_option =
          &(*terminal_config_ui->current_menu->options)[0];
  }

  render_options(terminal_config_ui);
}

static void prev_menu(struct terminal_config_ui *terminal_config_ui) {
  if (!terminal_config_ui->current_choice) {
    const struct terminal_ui_menu *first_menu = &menus[0];
    if (terminal_config_ui->current_menu != first_menu)
      terminal_config_ui->current_menu--;
    else {
      terminal_config_ui->current_menu = first_menu;
      while ((terminal_config_ui->current_menu + 1)->title)
        terminal_config_ui->current_menu++;
    }

    terminal_config_ui->current_option =
        &(*terminal_config_ui->current_menu->options)[0];

    clear_screen(terminal_config_ui);
    render_screen(terminal_config_ui);
  }
}

static void next_menu(struct terminal_config_ui *terminal_config_ui) {
  if (!terminal_config_ui->current_choice) {
    terminal_config_ui->current_menu++;

    if (!terminal_config_ui->current_menu->title)
      terminal_config_ui->current_menu = &menus[0];

    terminal_config_ui->current_option =
        &(*terminal_config_ui->current_menu->options)[0];

    clear_screen(terminal_config_ui);
    render_screen(terminal_config_ui);
  }
}

static void enter(struct terminal_config_ui *terminal_config_ui) {
  clear_help(terminal_config_ui);

  char *ptr =
      (char *)terminal_config_ui->current_option->getptr(terminal_config_ui);

  if (terminal_config_ui->current_choice) {
    set_option_value(terminal_config_ui, terminal_config_ui->current_option,
                     terminal_config_ui->current_choice -
                         &(*terminal_config_ui->current_option->choices)[0]);

    terminal_config_ui->current_choice = NULL;
  } else
    terminal_config_ui->current_choice =
        &(*terminal_config_ui->current_option->choices)[get_option_value(
            terminal_config_ui, terminal_config_ui->current_option)];

  render_options(terminal_config_ui);
  render_help(terminal_config_ui);
}

static void leave(struct terminal_config_ui *terminal_config_ui) {
  if (terminal_config_ui->current_choice) {
    clear_help(terminal_config_ui);

    terminal_config_ui->current_choice = NULL;

    render_screen(terminal_config_ui);
  } else {
    terminal_uart_xon_off(terminal_config_ui->terminal, XON);
    screen_printf(terminal_config_ui, "\x1b[0m");
    clear_screen(terminal_config_ui);
    screen_printf(terminal_config_ui, "\033c");
  }
}

static void apply(struct terminal_config_ui *terminal_config_ui) {
  if (!terminal_config_ui->current_choice) {
    screen_printf(terminal_config_ui, "\x1b[0m");
    clear_screen(terminal_config_ui);
    move_cursor(terminal_config_ui, 1, 1);
    screen_printf(terminal_config_ui, "Writing config...");
    terminal_config_ui->terminal->callbacks->write_config(
        &terminal_config_ui->terminal_config_copy);

    terminal_uart_xon_off(terminal_config_ui->terminal, XON);
    clear_screen(terminal_config_ui);
    screen_printf(terminal_config_ui, "\033c");
  }
}

extern struct terminal_config_ui *global_terminal_config_ui;

static void handle_enter(struct terminal *terminal) {
  enter(global_terminal_config_ui);
}

static void handle_esc(struct terminal *terminal) {
  leave(global_terminal_config_ui);
}

static void handle_up(struct terminal *terminal) {
  prev_option(global_terminal_config_ui);
}

static void handle_down(struct terminal *terminal) {
  next_option(global_terminal_config_ui);
}

static void handle_left(struct terminal *terminal) {
  prev_menu(global_terminal_config_ui);
}

static void handle_right(struct terminal *terminal) {
  next_menu(global_terminal_config_ui);
}

static void handle_f12(struct terminal *terminal) {
  apply(global_terminal_config_ui);
}

static const struct keys_entry config_entries[] = {
    [KEY_ENTER] = KEY_HANDLER(handle_enter),
    [KEY_ESCAPE] = KEY_HANDLER(handle_esc),
    [KEY_UPARROW] = KEY_HANDLER(handle_up),
    [KEY_DOWNARROW] = KEY_HANDLER(handle_down),
    [KEY_LEFTARROW] = KEY_HANDLER(handle_left),
    [KEY_RIGHTARROW] = KEY_HANDLER(handle_right),
    [KEY_F12] = KEY_HANDLER(handle_f12),
};

void terminal_config_ui_activate(
    struct terminal_config_ui *terminal_config_ui) {
  if (!terminal_config_ui->activated) {
    terminal_config_ui->activated = true;
    memcpy((void *)&terminal_config_ui->terminal_config_copy,
           (const void *)terminal_config_ui->terminal_config,
           sizeof(struct terminal_config));

    terminal_uart_xon_off(terminal_config_ui->terminal, XOFF);
    terminal_keyboard_set_keys_entries(terminal_config_ui->terminal,
                                       config_entries, false);

    screen_printf(terminal_config_ui, "\x1b<"
                                      "\x1b F"
                                      "\x1b%%G"
                                      "\x1b[2l"
                                      "\x1b[4l"
                                      "\x1b[12h"
                                      "\x1b[20l"
                                      "\x1b[?1l"
                                      "\x1b[?2h"
                                      "\x1b[?5l"
                                      "\x1b[?6l"
                                      "\x1b[?7l"
                                      "\x1b[?8l"
                                      "\x1b[?25l"
#ifdef TERMINAL_8BIT_COLOR
                                      "\x1b[97;48;5;54m"
#endif
    );

    clear_screen(terminal_config_ui);
    render_screen(terminal_config_ui);
  }
}
