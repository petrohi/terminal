#include "terminal_config_ui.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "keys.h"

#define PRINTF_BUFFER_SIZE 256

static size_t current_baud_rate(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.baud_rate;
}

static void change_baud_rate(struct terminal_config_ui *terminal_config_ui,
                             size_t baud_rate) {
  terminal_config_ui->terminal_config_copy.baud_rate = baud_rate;
}

static size_t
current_word_length(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.word_length;
}

static void change_word_length(struct terminal_config_ui *terminal_config_ui,
                               size_t word_length) {
  terminal_config_ui->terminal_config_copy.word_length = word_length;
}

static size_t current_stop_bits(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.stop_bits;
}

static void change_stop_bits(struct terminal_config_ui *terminal_config_ui,
                             size_t stop_bits) {
  terminal_config_ui->terminal_config_copy.stop_bits = stop_bits;
}

static size_t current_parity(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.parity;
}

static void change_parity(struct terminal_config_ui *terminal_config_ui,
                          size_t parity) {
  terminal_config_ui->terminal_config_copy.parity = parity;
}

static size_t current_start_up(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.start_up;
}

static void change_start_up(struct terminal_config_ui *terminal_config_ui,
                            size_t start_up) {
  terminal_config_ui->terminal_config_copy.start_up = start_up;
}

static size_t current_charset(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.charset;
}

static void change_charset(struct terminal_config_ui *terminal_config_ui,
                           size_t charset) {
  terminal_config_ui->terminal_config_copy.charset = charset;
}

static size_t current_c1_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.c1_mode;
}

static void change_c1_mode(struct terminal_config_ui *terminal_config_ui,
                           size_t c1_mode) {
  terminal_config_ui->terminal_config_copy.c1_mode = c1_mode;
}

static size_t
current_auto_wrap_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.auto_wrap_mode;
}

static void change_auto_wrap_mode(struct terminal_config_ui *terminal_config_ui,
                                  size_t auto_wrap_mode) {
  terminal_config_ui->terminal_config_copy.auto_wrap_mode = auto_wrap_mode;
}

static size_t
current_screen_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.screen_mode;
}

static void change_screen_mode(struct terminal_config_ui *terminal_config_ui,
                               size_t screen_mode) {
  terminal_config_ui->terminal_config_copy.screen_mode = screen_mode;
}

static size_t
current_send_receive_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.send_receive_mode;
}

static void
change_send_receive_mode(struct terminal_config_ui *terminal_config_ui,
                         size_t send_receive_mode) {
  terminal_config_ui->terminal_config_copy.send_receive_mode =
      send_receive_mode;
}

static size_t
current_new_line_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.new_line_mode;
}

static void change_new_line_mode(struct terminal_config_ui *terminal_config_ui,
                                 size_t new_line_mode) {
  terminal_config_ui->terminal_config_copy.new_line_mode = new_line_mode;
}

static size_t
current_cursor_key_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.cursor_key_mode;
}

static void
change_cursor_key_mode(struct terminal_config_ui *terminal_config_ui,
                       size_t cursor_key_mode) {
  terminal_config_ui->terminal_config_copy.cursor_key_mode = cursor_key_mode;
}

static size_t
current_auto_repeat_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.auto_repeat_mode;
}

static void
change_auto_repeat_mode(struct terminal_config_ui *terminal_config_ui,
                        size_t auto_repeat_mode) {
  terminal_config_ui->terminal_config_copy.auto_repeat_mode = auto_repeat_mode;
}

static size_t current_ansi_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.ansi_mode;
}

static void change_ansi_mode(struct terminal_config_ui *terminal_config_ui,
                             size_t ansi_mode) {
  terminal_config_ui->terminal_config_copy.ansi_mode = ansi_mode;
}

static size_t
current_backspace_mode(struct terminal_config_ui *terminal_config_ui) {
  return terminal_config_ui->terminal_config_copy.backspace_mode;
}

static void change_backspace_mode(struct terminal_config_ui *terminal_config_ui,
                                  size_t backspace_mode) {
  terminal_config_ui->terminal_config_copy.backspace_mode = backspace_mode;
}

static const struct terminal_ui_choice off_on_choices[] = {
    {"off"},
    {"on"},
    {NULL},
};

static const struct terminal_ui_menu menus[] = {
    {"General",
     &(const struct terminal_ui_option[]){{"Start up", current_start_up,
                                           change_start_up,
                                           &(const struct terminal_ui_choice[]){
                                               {"none"},
                                               {"message"},
                                               {"font test1"},
                                               {"font test2"},
                                               {"color test1"},
                                               {"color test2"},
                                               {NULL},
                                           }},
                                          {NULL}}},
    {"Serial",
     &(const struct terminal_ui_option[]){
         {"Baud rate", current_baud_rate, change_baud_rate,
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
              {NULL},
          }},
         {"Word length", current_word_length, change_word_length,
          &(const struct terminal_ui_choice[]){[WORD_LENGTH_8B] = {"8 bits"},
                                               [WORD_LENGTH_9B] = {"9 bits"},
                                               {NULL}}},
         {"Stop bits", current_stop_bits, change_stop_bits,
          &(const struct terminal_ui_choice[]){
              [STOP_BITS_1] = {"1 bit"}, [STOP_BITS_2] = {"2 bits"}, {NULL}}},
         {"Parity", current_parity, change_parity,
          &(const struct terminal_ui_choice[]){[PARITY_NONE] = {"none"},
                                               [PARITY_EVEN] = {"even"},
                                               [PARITY_ODD] = {"odd"},
                                               {NULL}}},
         {NULL}}},
    {"Terminal",
     &(const struct terminal_ui_option[]){
         {"Character set", current_charset, change_charset,
          &(const struct terminal_ui_choice[]){
              {"ASCII"},
              {"UTF8"},
              {NULL},
          }},
         {"Controls", current_c1_mode, change_c1_mode,
          &(const struct terminal_ui_choice[]){
              {"S7C1T"},
              {"S8C1T"},
              {NULL},
          }},
         {"Send/receive mode (SRM)", current_send_receive_mode,
          change_send_receive_mode,
          &(const struct terminal_ui_choice[]){
              {"echo"},
              {"normal"},
              {NULL},
          }},
         {"New line mode (LNM)", current_new_line_mode, change_new_line_mode,
          &off_on_choices},
         {"Cursor key mode (DECCKM)", current_cursor_key_mode,
          change_cursor_key_mode,
          &(const struct terminal_ui_choice[]){
              {"normal"},
              {"application"},
              {NULL},
          }},
         {"ANSI mode (DECANM)", current_ansi_mode, change_ansi_mode,
          &(const struct terminal_ui_choice[]){
              {"VT52"},
              {"ANSI"},
              {NULL},
          }},
         {"Auto-wraparound mode (DECAWM)", current_auto_wrap_mode,
          change_auto_wrap_mode, &off_on_choices},
         {"Auto-repeat keys mode (DECARM)", current_auto_repeat_mode,
          change_auto_repeat_mode, &off_on_choices},
         {"Screen mode (DECSCNM)", current_screen_mode, change_screen_mode,
          &(const struct terminal_ui_choice[]){
              {"normal"},
              {"inverse"},
              {NULL},
          }},
         {"Backspace mode (DECBKM)", current_backspace_mode,
          change_backspace_mode, &off_on_choices},
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

void terminal_config_ui_init(struct terminal_config_ui *terminal_config_ui,
                             struct terminal *terminal,
                             struct terminal_config *terminal_config) {
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

static const char *choice_help[] = {
    "↑↓ - Change value",
    "<Enter> - OK",
    "<Esc> - Cancel",
    NULL,
};

static const char *default_help[] = {
    "←→ Select menu",           "↑↓ - Select option",
    "<Enter> - Edit option",    "<Esc> - Discard and restart",
    "<F12> - Save and restart", NULL,
};

#define ROWS terminal_config_ui->terminal->format.rows
#define COLS terminal_config_ui->terminal->format.cols

#define TITLE_ROW 2
#define MENU_ROW 4
#define TOP_ROW 5
#define MAIN_ROW 8
#define BOTTOM_ROW ROWS

#define LEFT_COL 1
#define OPTIONS_COL 4
#define CHOICES_COL 35
#define DIVIDER_COL 50
#define HELP_COL 52
#define RIGHT_COL COLS

static void clear_help(struct terminal_config_ui *terminal_config_ui) {
  const char **help =
      terminal_config_ui->current_choice ? &choice_help[0] : &default_help[0];

  size_t i = 0;
  while (*help) {
    move_cursor(terminal_config_ui, i + MAIN_ROW, HELP_COL);
    screen_printf(terminal_config_ui, "\x1b[%dX",
                  COLS - HELP_COL);
    help++;
    i++;
  }
}

static void render_help(struct terminal_config_ui *terminal_config_ui) {
  const char **help =
      terminal_config_ui->current_choice ? &choice_help[0] : &default_help[0];

  size_t i = 0;
  while (*help) {
    move_cursor(terminal_config_ui, i + MAIN_ROW, HELP_COL);
    screen_printf(terminal_config_ui, *help);
    help++;
    i++;
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
  }

  move_cursor(terminal_config_ui, 24, LEFT_COL);
  screen_printf(terminal_config_ui, "╚");
  move_cursor(terminal_config_ui, 24, RIGHT_COL);
  screen_printf(terminal_config_ui, "╝");

  move_cursor(terminal_config_ui, TOP_ROW, LEFT_COL + 1);
  for (size_t col = LEFT_COL + 1; col <= RIGHT_COL - 1; col++) {
    if (col == DIVIDER_COL)
      screen_printf(terminal_config_ui, "╤");
    else
      screen_printf(terminal_config_ui, "═");
  }

  move_cursor(terminal_config_ui, BOTTOM_ROW, LEFT_COL + 1);
  for (size_t col = LEFT_COL + 1; col <= RIGHT_COL - 1; col++) {
    if (col == DIVIDER_COL)
      screen_printf(terminal_config_ui, "╧");
    else
      screen_printf(terminal_config_ui, "═");
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
            : &(*option->choices)[option->current(terminal_config_ui)];

    screen_printf(terminal_config_ui, "\x1b[%dm[%11s]\x1b[27m",
                  terminal_config_ui->current_choice && current_option ? 7 : 27,
                  choice->title);

    i++;
    option++;
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

void terminal_config_ui_enter(struct terminal_config_ui *terminal_config_ui) {
  if (!terminal_config_ui->activated) {
    terminal_config_ui->activated = true;
    memcpy(&terminal_config_ui->terminal_config_copy,
           terminal_config_ui->terminal_config, sizeof(struct terminal_config));
    terminal_uart_xon_off(terminal_config_ui->terminal, XOFF);
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
                                      "\x1b[97;48;5;54m");

    clear_screen(terminal_config_ui);
    render_screen(terminal_config_ui);
  }
}

static void enter(struct terminal_config_ui *terminal_config_ui) {
  clear_help(terminal_config_ui);

  if (terminal_config_ui->current_choice) {
    terminal_config_ui->current_option->change(
        terminal_config_ui,
        terminal_config_ui->current_choice -
            &(*terminal_config_ui->current_option->choices)[0]);
    terminal_config_ui->current_choice = NULL;
  } else
    terminal_config_ui->current_choice =
        &(*terminal_config_ui->current_option
               ->choices)[terminal_config_ui->current_option->current(
            terminal_config_ui)];

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
  screen_printf(terminal_config_ui, "\x1b[0m");
  clear_screen(terminal_config_ui);
  move_cursor(terminal_config_ui, 1, 1);
  screen_printf(terminal_config_ui, "Writing config...");
  terminal_config_ui->terminal->callbacks->system_write_config(
      &terminal_config_ui->terminal_config_copy);

  terminal_uart_xon_off(terminal_config_ui->terminal, XON);
  clear_screen(terminal_config_ui);
  screen_printf(terminal_config_ui, "\033c");
}

void terminal_config_ui_handle_key(
    struct terminal_config_ui *terminal_config_ui, uint8_t key) {

  switch (key) {
  case KEY_ENTER:
    enter(terminal_config_ui);
    break;
  case KEY_ESCAPE:
    leave(terminal_config_ui);
    break;
  case KEY_UPARROW:
    prev_option(terminal_config_ui);
    break;
  case KEY_DOWNARROW:
    next_option(terminal_config_ui);
    break;
  case KEY_LEFTARROW:
    prev_menu(terminal_config_ui);
    break;
  case KEY_RIGHTARROW:
    next_menu(terminal_config_ui);
    break;
  case KEY_F12:
    apply(terminal_config_ui);
    break;
  }
}
