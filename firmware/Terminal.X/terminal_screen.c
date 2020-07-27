#include "terminal_internal.h"

#include "luminance.h"
#include <string.h>

#define CURSOR_ON_COUNTER 650
#define CURSOR_OFF_COUNTER 350

#define BLINK_ON_COUNTER 500
#define BLINK_OFF_COUNTER 500

#define CELL_SIZE sizeof(struct visual_cell)
#define CELLS_ROW_SIZE CELL_SIZE *COLS
#define CELLS_SIZE CELLS_ROW_SIZE *ROWS

static void clear_cells_rows(struct terminal *terminal, int16_t from_row,
                             int16_t to_row) {
  if (to_row <= from_row)
    return;

  if (to_row > ROWS)
    return;

  uint16_t rows = to_row - from_row;
  size_t offset = from_row * COLS;
  struct visual_cell *cells = terminal->cells + offset;

  for (uint16_t i = 0; i < rows; ++i, cells += COLS) {
    memset(cells, 0, CELLS_ROW_SIZE);

    terminal->callbacks->system_yield();
  }
}

static void clear_cells_cols(struct terminal *terminal, int16_t row,
                             int16_t from_col, int16_t to_col) {
  if (row >= ROWS)
    return;

  if (to_col <= from_col)
    return;

  if (to_col > COLS)
    return;

  size_t offset = row * COLS + from_col;
  struct visual_cell *cells = terminal->cells + offset;

  memset(cells, 0, CELL_SIZE * (to_col - from_col));
}

static void scroll_cells(struct terminal *terminal, enum scroll scroll,
                         int16_t from_row, int16_t to_row, int16_t rows) {
  if (to_row <= from_row)
    return;

  if (to_row > ROWS)
    return;

  if (to_row <= from_row + rows) {
    clear_cells_rows(terminal, from_row, to_row);
    return;
  }

  size_t disp = COLS * rows;
  uint16_t rows_diff = to_row - from_row - rows;

  if (scroll == SCROLL_DOWN) {
    size_t offset = COLS * to_row - COLS;
    struct visual_cell *cells = terminal->cells + offset;

    for (uint16_t i = 0; i < rows_diff; ++i, cells -= COLS) {
      memcpy(cells, cells - disp, CELLS_ROW_SIZE);

      terminal->callbacks->system_yield();
    }

    clear_cells_rows(terminal, from_row, from_row + rows);
  } else if (scroll == SCROLL_UP) {
    size_t offset = COLS * from_row;
    struct visual_cell *cells = terminal->cells + offset;

    for (uint16_t i = 0; i < rows_diff; ++i, cells += COLS) {
      memcpy(cells, cells + disp, CELLS_ROW_SIZE);

      terminal->callbacks->system_yield();
    }

    clear_cells_rows(terminal, to_row - rows, to_row);
  }
}

static void shift_cells_right(struct terminal *terminal, int16_t row,
                              int16_t col, size_t cols) {
  if (row >= ROWS)
    return;

  if (col >= COLS)
    return;

  if (col + cols > COLS)
    return;

  size_t size = CELL_SIZE * (COLS - col - cols);
  size_t offset = COLS * row + col;
  struct visual_cell *cells = terminal->cells + offset;

  struct visual_cell tmp[COLS];

  memcpy(tmp, cells, size);
  memcpy(cells + cols, tmp, size);

  clear_cells_cols(terminal, row, col, col + cols);
}

static void shift_cells_left(struct terminal *terminal, int16_t row,
                             int16_t col, size_t cols) {
  if (row >= ROWS)
    return;

  if (col >= COLS)
    return;

  if (col + cols > COLS)
    return;

  size_t size = CELL_SIZE * (COLS - col - cols);
  size_t offset = COLS * row + col;
  struct visual_cell *cells = terminal->cells + offset;

  memcpy(cells, cells + cols, size);

  clear_cells_cols(terminal, row, COLS - cols, COLS);
}

struct visual_cell *get_cell(struct terminal *terminal, int16_t row,
                             int16_t col) {
  return &terminal->cells[row * COLS + col];
}

static void swap_colors(color_t *color1, color_t *color2) {
  color_t tmp = *color1;
  *color1 = *color2;
  *color2 = tmp;
}

static void render_character(struct terminal *terminal, int16_t row,
                             int16_t col, bool cursor, bool blink) {
  struct visual_cell *cell = get_cell(terminal, row, col);

  color_t active = cell->c ? cell->p.active_color : terminal->vs.p.active_color;
  color_t inactive =
      cell->c ? cell->p.inactive_color : terminal->vs.p.inactive_color;

  if (cell->p.negative != terminal->screen_mode)
    swap_colors(&active, &inactive);

  if (cursor) {
    active = ~active;
    inactive = ~inactive;
  }

  if (terminal->vs.p.concealed || blink) {
    active = inactive;
  }

  if (terminal->monochrome)
    luminance_to_monochrome(&active, &inactive);

  terminal->callbacks->screen_draw_codepoint(
      terminal->format, row, col, cell->c, cell->p.font, cell->p.italic,
      cell->p.underlined, cell->p.crossedout, active, inactive);
}

static void draw_cursor(struct terminal *terminal) {
  if (!terminal->cursor_drawn) {
    render_character(
        terminal, terminal->vs.cursor_row, terminal->vs.cursor_col, true,
        terminal->blink_drawn &&
            get_cell(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col)
                ->p.blink);
    terminal->cursor_drawn = true;
  }
}

static void clear_cursor(struct terminal *terminal) {
  if (terminal->cursor_drawn) {
    render_character(
        terminal, terminal->vs.cursor_row, terminal->vs.cursor_col, false,
        terminal->blink_drawn &&
            get_cell(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col)
                ->p.blink);
    terminal->cursor_drawn = false;
  }
}

static void update_cursor(struct terminal *terminal) {
  if (terminal->cursor_on) {
    draw_cursor(terminal);
  } else {
    clear_cursor(terminal);
  }
}

static void draw_blink(struct terminal *terminal, bool blink) {
  if (terminal->blink_drawn != blink) {
    for (int16_t row = 0; row < ROWS; ++row)
      for (int16_t col = 0; col < COLS; ++col)
        if (get_cell(terminal, row, col)->p.blink)
          render_character(terminal, row, col,
                           terminal->cursor_drawn &&
                               terminal->vs.cursor_row == row &&
                               terminal->vs.cursor_col == col,
                           blink);

    terminal->blink_drawn = blink;
  }
}

static void clear_blink(struct terminal *terminal) {
  if (terminal->blink_drawn) {
    draw_blink(terminal, false);
  }
}

static void update_blink(struct terminal *terminal) {
  draw_blink(terminal, terminal->blink_on);
}

static void draw_codepoint(struct terminal *terminal, codepoint_t codepoint) {
  struct visual_cell *cell =
      get_cell(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col);

  cell->p = terminal->vs.p;
  cell->c = codepoint;

  render_character(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col,
                   terminal->cursor_drawn,
                   terminal->blink_drawn && cell->p.blink);
}

static void draw_screen(struct terminal *terminal) {
  for (int16_t row = 0; row < ROWS; ++row)
    for (int16_t col = 0; col < COLS; ++col) {
      struct visual_cell *cell = get_cell(terminal, row, col);
      render_character(terminal, row, col,
                       terminal->cursor_drawn &&
                           terminal->vs.cursor_row == row &&
                           terminal->vs.cursor_col == col,
                       terminal->blink_drawn && cell->p.blink);
    }
}

static color_t inactive_color(struct terminal *terminal) {
  color_t active = terminal->vs.p.active_color;
  color_t inactive = terminal->vs.p.inactive_color;

  if (terminal->monochrome)
    luminance_to_monochrome(&active, &inactive);

  return terminal->screen_mode ? active : inactive;
}

static void clear_rows(struct terminal *terminal, int16_t from_row,
                       int16_t to_row) {
  terminal->callbacks->screen_clear_rows(terminal->format, from_row, to_row,
                                         inactive_color(terminal));

  clear_cells_rows(terminal, from_row, to_row);
}

static void clear_cols(struct terminal *terminal, int16_t row, int16_t from_col,
                       int16_t to_col) {
  terminal->callbacks->screen_clear_cols(terminal->format, row, from_col,
                                         to_col, inactive_color(terminal));

  clear_cells_cols(terminal, row, from_col, to_col);
}

static void screen_scroll(struct terminal *terminal, enum scroll scroll,
                          int16_t from_row, int16_t rows) {
  if (from_row < terminal->margin_bottom) {
    terminal->callbacks->screen_scroll(terminal->format, scroll, from_row,
                                       terminal->margin_bottom, rows,
                                       inactive_color(terminal));

    scroll_cells(terminal, scroll, from_row, terminal->margin_bottom, rows);
  }
}

static bool inside_margins(struct terminal *terminal) {
  return (terminal->vs.cursor_row >= terminal->margin_top &&
          terminal->vs.cursor_row < terminal->margin_bottom);
}

void terminal_screen_move_cursor_absolute(struct terminal *terminal,
                                          int16_t row, int16_t col) {
  clear_cursor(terminal);

  if (terminal->origin_mode) {
    row = terminal->margin_top + row;

    if (row < terminal->margin_top)
      row = terminal->margin_top;

    if (row >= terminal->margin_bottom)
      row = terminal->margin_bottom - 1;
  } else {
    if (row < 0)
      row = 0;

    if (row >= ROWS)
      row = ROWS - 1;
  }

  if (col < 0)
    col = 0;

  if (col >= COLS)
    col = COLS - 1;

  terminal->vs.cursor_row = row;
  terminal->vs.cursor_col = col;
  terminal->vs.cursor_last_col = false;

  update_cursor(terminal);
}

int16_t get_terminal_screen_cursor_row(struct terminal *terminal) {
  return terminal->vs.cursor_row -
         (terminal->origin_mode ? terminal->margin_top : 0);
}

int16_t get_terminal_screen_cursor_col(struct terminal *terminal) {
  return terminal->vs.cursor_col;
}

void terminal_screen_move_cursor(struct terminal *terminal, int16_t rows,
                                 int16_t cols) {
  clear_cursor(terminal);

  int16_t row = terminal->vs.cursor_row + rows;
  int16_t col = terminal->vs.cursor_col + cols;

  if (inside_margins(terminal) || terminal->origin_mode) {
    if (row < terminal->margin_top)
      row = terminal->margin_top;

    if (row >= terminal->margin_bottom)
      row = terminal->margin_bottom - 1;
  } else {
    if (row < 0)
      row = 0;

    if (row >= ROWS)
      row = ROWS - 1;
  }

  if (col < 0)
    col = 0;

  if (col >= COLS)
    col = COLS - 1;

  terminal->vs.cursor_row = row;
  terminal->vs.cursor_col = col;
  terminal->vs.cursor_last_col = false;

  update_cursor(terminal);
}

void terminal_screen_carriage_return(struct terminal *terminal) {
  clear_cursor(terminal);

  terminal->vs.cursor_col = 0;
  terminal->vs.cursor_last_col = false;

  update_cursor(terminal);
}

void terminal_screen_scroll(struct terminal *terminal, enum scroll scroll,
                            size_t from_row, size_t rows) {
  clear_cursor(terminal);
  clear_blink(terminal);

  if (terminal->origin_mode) {
    from_row = terminal->margin_top + from_row;
  }

  screen_scroll(terminal, scroll, from_row, rows);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_clear_to_right(struct terminal *terminal) {
  clear_blink(terminal);
  clear_cursor(terminal);

  clear_cols(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col, COLS);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_clear_to_left(struct terminal *terminal) {
  clear_blink(terminal);
  clear_cursor(terminal);

  clear_cols(terminal, terminal->vs.cursor_row, 0, terminal->vs.cursor_col + 1);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_clear_to_top(struct terminal *terminal) {
  clear_blink(terminal);
  clear_cursor(terminal);

  clear_rows(terminal, 0, terminal->vs.cursor_row);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_clear_row(struct terminal *terminal) {
  clear_blink(terminal);
  clear_cursor(terminal);

  clear_cols(terminal, terminal->vs.cursor_row, 0, COLS);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_clear_to_bottom(struct terminal *terminal) {
  clear_blink(terminal);
  clear_cursor(terminal);

  clear_rows(terminal, terminal->vs.cursor_row + 1, ROWS);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_clear_all(struct terminal *terminal) {
  clear_blink(terminal);
  clear_cursor(terminal);

  clear_rows(terminal, 0, ROWS);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_index(struct terminal *terminal, int16_t rows) {
  clear_cursor(terminal);

  if (inside_margins(terminal)) {
    if (terminal->vs.cursor_row + rows >= terminal->margin_bottom) {
      clear_blink(terminal);

      screen_scroll(
          terminal, SCROLL_UP, terminal->margin_top,
          rows - (terminal->margin_bottom - 1 - terminal->vs.cursor_row));
      terminal->vs.cursor_row = terminal->margin_bottom - 1;

      update_blink(terminal);
    } else
      terminal->vs.cursor_row += rows;
  } else
    terminal_screen_move_cursor_absolute(
        terminal, terminal->vs.cursor_row + rows, terminal->vs.cursor_col);

  update_cursor(terminal);
}

void terminal_screen_reverse_index(struct terminal *terminal, int16_t rows) {
  clear_cursor(terminal);
  if (inside_margins(terminal)) {
    if (terminal->vs.cursor_row - rows < terminal->margin_top) {
      clear_blink(terminal);

      screen_scroll(terminal, SCROLL_DOWN, terminal->margin_top,
                    rows - (terminal->vs.cursor_row - terminal->margin_top));
      terminal->vs.cursor_row = terminal->margin_top;

      update_blink(terminal);
    } else
      terminal->vs.cursor_row -= rows;
  } else
    terminal_screen_move_cursor_absolute(
        terminal, terminal->vs.cursor_row - rows, terminal->vs.cursor_col);

  update_cursor(terminal);
}

void terminal_screen_put_codepoint(struct terminal *terminal,
                                   codepoint_t codepoint) {
  if (terminal->vs.cursor_last_col) {
    terminal_screen_carriage_return(terminal);
    terminal_screen_index(terminal, 1);
  }

  if (terminal->insert_mode)
    terminal_screen_insert(terminal, 1);

  clear_cursor(terminal);

  draw_codepoint(terminal, codepoint);

  if (terminal->vs.cursor_col == COLS - 1) {
    if (terminal->auto_wrap_mode)
      terminal->vs.cursor_last_col = true;
  } else
    terminal->vs.cursor_col++;

  update_cursor(terminal);
}

void terminal_screen_insert(struct terminal *terminal, size_t cols) {
  clear_cursor(terminal);
  clear_blink(terminal);

  terminal->callbacks->screen_shift_right(
      terminal->format, terminal->vs.cursor_row, terminal->vs.cursor_col, cols,
      inactive_color(terminal));

  shift_cells_right(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col,
                    cols);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_delete(struct terminal *terminal, size_t cols) {
  clear_cursor(terminal);
  clear_blink(terminal);

  terminal->callbacks->screen_shift_left(
      terminal->format, terminal->vs.cursor_row, terminal->vs.cursor_col, cols,
      inactive_color(terminal));

  shift_cells_left(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col,
                   cols);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_erase(struct terminal *terminal, size_t cols) {
  clear_cursor(terminal);
  clear_blink(terminal);

  clear_cols(terminal, terminal->vs.cursor_row, terminal->vs.cursor_col,
             terminal->vs.cursor_col + cols);

  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_enable_cursor(struct terminal *terminal, bool enable) {
  clear_cursor(terminal);

  if (enable) {
    terminal->cursor_counter = CURSOR_ON_COUNTER;
    terminal->cursor_on = true;
  } else {
    terminal->cursor_counter = 0;
    terminal->cursor_on = false;
  }

  update_cursor(terminal);
}

void terminal_screen_save_visual_state(struct terminal *terminal) {
  terminal->saved_vs = terminal->vs;
}

void terminal_screen_restore_visual_state(struct terminal *terminal) {
  clear_cursor(terminal);

  terminal->vs = terminal->saved_vs;

  if (terminal->origin_mode) {
    if (terminal->vs.cursor_row < terminal->margin_top)
      terminal->vs.cursor_row = terminal->margin_top;

    if (terminal->vs.cursor_row >= terminal->margin_bottom)
      terminal->vs.cursor_row = terminal->margin_bottom - 1;
  }

  update_cursor(terminal);
}

void terminal_screen_update_cursor_counter(struct terminal *terminal) {
  if (terminal->cursor_counter) {
    terminal->cursor_counter--;

    if (!terminal->cursor_counter) {
      if (terminal->cursor_on) {
        terminal->cursor_on = false;
        terminal->cursor_counter = CURSOR_OFF_COUNTER;
      } else {
        terminal->cursor_on = true;
        terminal->cursor_counter = CURSOR_ON_COUNTER;
      }
    }
  }
}

void terminal_screen_update_blink_counter(struct terminal *terminal) {
  if (terminal->blink_counter) {
    terminal->blink_counter--;

    if (!terminal->blink_counter) {
      if (terminal->blink_on) {
        terminal->blink_on = false;
        terminal->blink_counter = BLINK_OFF_COUNTER;
      } else {
        terminal->blink_on = true;
        terminal->blink_counter = BLINK_ON_COUNTER;
      }
    }
  }
}

void terminal_screen_update(struct terminal *terminal) {
  update_cursor(terminal);
  update_blink(terminal);
}

void terminal_screen_set_screen_mode(struct terminal *terminal, bool mode) {
  if (terminal->screen_mode != mode) {
    terminal->screen_mode = mode;
    draw_screen(terminal);
  }
}

void terminal_screen_init(struct terminal *terminal) {
  terminal->vs.cursor_row = 0;
  terminal->vs.cursor_col = 0;
  terminal->vs.cursor_last_col = false;

  terminal->vs.p.font = FONT_NORMAL;
  terminal->vs.p.italic = false;
  terminal->vs.p.underlined = false;
  terminal->vs.p.blink = false;
  terminal->vs.p.negative = false;
  terminal->vs.p.concealed = false;
  terminal->vs.p.crossedout = false;
  terminal->vs.p.active_color = DEFAULT_ACTIVE_COLOR;
  terminal->vs.p.inactive_color = DEFAULT_INACTIVE_COLOR;

  terminal->margin_top = 0;
  terminal->margin_bottom = ROWS;

  for (size_t i = 0; i < COLS; ++i)
    terminal->tab_stops[i] = ((i + 1) % 8) == 0;

  terminal->cursor_counter = CURSOR_ON_COUNTER;
  terminal->cursor_on = true;
  terminal->cursor_drawn = false;

  terminal->blink_counter = BLINK_ON_COUNTER;
  terminal->blink_on = true;
  terminal->blink_drawn = false;

  terminal_screen_clear_all(terminal);
  update_cursor(terminal);
}
