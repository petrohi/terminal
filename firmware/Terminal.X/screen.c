#include "screen.h"

#ifdef TERMINAL_MANDELBROT
#include <complex.h>
#endif
#include <string.h>

#define REPLACEMENT_CODEPOINT 0xfffd

#define ROWS screen->format.rows
#define COLS screen->format.cols

#define CHAR_WIDTH_PIXELS screen->char_width
#define CHAR_HEIGHT_LINES screen->char_height

#ifdef TERMINAL_8BIT_COLOR
#define PIXELS_PER_BYTE 1
#define BYTES_PER_PIXEL 1
#define LEFT_PADDING_PIXELS 0
#define RIGHT_PADDING_PIXELS 0
#else
#define PIXELS_PER_BYTE 8
#define BYTES_PER_PIXEL 0
#define LEFT_PADDING_PIXELS 48
#define RIGHT_PADDING_PIXELS 16
#endif

#define LEFT_PADDING_BYTES (LEFT_PADDING_PIXELS / PIXELS_PER_BYTE)
#define RIGHT_PADDING_BYTES (RIGHT_PADDING_PIXELS / PIXELS_PER_BYTE)
#define CHAR_WIDTH_BYTES (CHAR_WIDTH_PIXELS / PIXELS_PER_BYTE)

#define SCREEN_WIDTH_PIXELS                                                    \
  (LEFT_PADDING_PIXELS + COLS * CHAR_WIDTH_PIXELS + RIGHT_PADDING_PIXELS)
#define SCREEN_WIDTH_BYTES                                                     \
  (LEFT_PADDING_BYTES + COLS * CHAR_WIDTH_BYTES + RIGHT_PADDING_BYTES)

#define SCREEN_HEIGHT_LINES (ROWS * CHAR_HEIGHT_LINES)

void screen_clear_rows(struct screen *screen, size_t from_row, size_t to_row,
                       color_t inactive) {
  if (to_row <= from_row)
    return;

  if (to_row > ROWS)
    return;

  size_t size = SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * (to_row - from_row);
  size_t offset = SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * from_row;

  memset(screen->buffer + offset, inactive, size);
}

void screen_clear_cols(struct screen *screen, size_t row, size_t from_col,
                       size_t to_col, color_t inactive) {
  if (row >= ROWS)
    return;

  if (to_col <= from_col)
    return;

  if (to_col > COLS)
    return;

  size_t size = CHAR_WIDTH_BYTES * (to_col - from_col);
  size_t offset = SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * row +
                  CHAR_WIDTH_BYTES * from_col + LEFT_PADDING_BYTES;

  for (size_t i = 0; i < CHAR_HEIGHT_LINES; ++i)
    memset(screen->buffer + offset + (SCREEN_WIDTH_BYTES * i), inactive, size);
}

void screen_shift_right(struct screen *screen, size_t row, size_t col,
                        size_t cols, color_t inactive) {
  if (row >= ROWS)
    return;

  if (col >= COLS)
    return;

  if (col + cols > COLS)
    return;

  if (col + cols < COLS) {
    size_t size = CHAR_WIDTH_BYTES * (COLS - col - cols);
    size_t offset = SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * row +
                    CHAR_WIDTH_BYTES * col + LEFT_PADDING_BYTES;
    size_t disp = CHAR_WIDTH_BYTES * cols;

    for (size_t i = 0; i < CHAR_HEIGHT_LINES; ++i)
      memmove(screen->buffer + offset + (SCREEN_WIDTH_BYTES * i) + disp,
              screen->buffer + offset + (SCREEN_WIDTH_BYTES * i), size);
  }

  screen_clear_cols(screen, row, col, col + cols, inactive);
}

void screen_shift_left(struct screen *screen, size_t row, size_t col,
                       size_t cols, color_t inactive) {
  if (row >= ROWS)
    return;

  if (col >= COLS)
    return;

  if (col + cols > COLS)
    return;

  if (col + cols < COLS) {
    size_t size = CHAR_WIDTH_BYTES * (COLS - col - cols);
    size_t offset = SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * row +
                    CHAR_WIDTH_BYTES * col + LEFT_PADDING_BYTES;
    size_t disp = CHAR_WIDTH_BYTES * cols;

    for (size_t i = 0; i < CHAR_HEIGHT_LINES; ++i)
      memcpy(screen->buffer + offset + (SCREEN_WIDTH_BYTES * i),
             screen->buffer + offset + (SCREEN_WIDTH_BYTES * i) + disp, size);
  }

  screen_clear_cols(screen, row, COLS - cols, COLS, inactive);
}

void screen_scroll(struct screen *screen, enum scroll scroll, size_t from_row,
                   size_t to_row, size_t rows, color_t inactive) {
  if (to_row <= from_row)
    return;

  if (to_row > ROWS)
    return;

  if (to_row <= from_row + rows) {
    screen_clear_rows(screen, from_row, to_row, inactive);
    return;
  }

  size_t disp = SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * rows;
  size_t size =
      SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * (to_row - from_row - rows);
  size_t offset = SCREEN_WIDTH_BYTES * CHAR_HEIGHT_LINES * from_row;

  if (scroll == SCROLL_DOWN) {
    memmove((void *)screen->buffer + offset + disp,
            (void *)screen->buffer + offset, size);

    screen_clear_rows(screen, from_row, from_row + rows, inactive);
  } else if (scroll == SCROLL_UP) {
    memcpy((void *)screen->buffer + offset,
           (void *)screen->buffer + offset + disp, size);

    screen_clear_rows(screen, to_row - rows, to_row, inactive);
  }
}

static inline size_t pixel_offset(struct screen *screen, size_t line,
                                  size_t pixel) {
  return (SCREEN_WIDTH_BYTES * line) + (pixel / PIXELS_PER_BYTE) +
         LEFT_PADDING_BYTES;
}

#ifdef TERMINAL_8BIT_COLOR
static inline void set_color(struct screen *screen, size_t offset, size_t pixel,
                             color_t color) {
  screen->buffer[offset] = color;
}
#else
static inline void set_color(struct screen *screen, size_t offset, size_t pixel,
                             color_t color) {
  size_t shift = (CHAR_WIDTH_PIXELS - (pixel % PIXELS_PER_BYTE)) - 1;

  if (color)
    screen->buffer[offset] |= (1 << shift);
  else
    screen->buffer[offset] &= ~(1 << shift);
}
#endif

void screen_draw_codepoint(struct screen *screen, size_t row, size_t col,
                           codepoint_t codepoint, enum font font, bool italic,
                           bool underlined, bool crossedout, color_t active,
                           color_t inactive) {
  if (row >= ROWS)
    return;

  if (col >= COLS)
    return;

  size_t base_line = row * CHAR_HEIGHT_LINES;
  size_t base_pixel = col * CHAR_WIDTH_PIXELS;

  const struct bitmap_font *bitmap_font;
  if (font == FONT_BOLD) {
    bitmap_font = screen->bold_bitmap_font;
  } else {
    bitmap_font = screen->normal_bitmap_font;
  }

  const unsigned char *glyph = NULL;

  if (codepoint) {
    glyph = find_glyph(bitmap_font, codepoint);

    if (!glyph)
      glyph = find_glyph(bitmap_font, REPLACEMENT_CODEPOINT);
  }

  for (size_t char_line = 0; char_line < CHAR_HEIGHT_LINES; char_line++) {

    for (size_t char_pixel = 0; char_pixel < CHAR_WIDTH_PIXELS; char_pixel++) {

      size_t offset =
          pixel_offset(screen, base_line + char_line, base_pixel + char_pixel);

      color_t color = inactive;

      if (glyph && ((underlined && char_line == CHAR_HEIGHT_LINES - 2) ||
                    (crossedout && char_line == CHAR_HEIGHT_LINES / 2) ||

                    (char_pixel < bitmap_font->width &&
                     char_line < bitmap_font->height &&

                     glyph[char_line] & (1 << char_pixel)))) {

        color = active;
      }

      set_color(screen, offset, base_pixel + char_pixel, color);
    }
  }
}

void screen_test_fonts(struct screen *screen, enum font font) {
  for (size_t row = 0; row < 24; row++) {
    for (size_t col = 0; col < 64; col++) {
      codepoint_t codepoint = ((row * 64) + col);

      screen_draw_codepoint(screen, row, col, codepoint, font, false, false,
                            false, 0xf, 0);
    }
  }
}

#ifdef TERMINAL_MANDELBROT
#define MANDELBROT_ITERATIONS 200

static float mandelbrot(float complex z) {
  float complex v = 0;

  for (size_t n = 0; n < MANDELBROT_ITERATIONS; ++n) {
    v = v * v + z;
    if (cabsf(v) > 2.0) {
      return (float)n / (float)MANDELBROT_ITERATIONS;
    }
  }

  return 0.0;
}

#define MARGIN_X                                                               \
  ((SCREEN_WIDTH_PIXELS - LEFT_PADDING_PIXELS - RIGHT_PADDING_PIXELS -         \
    SCREEN_HEIGHT_LINES) /                                                     \
   2)

void screen_test_mandelbrot(struct screen *screen, float window_x,
                            float window_y, float window_r, bool (*cancel)()) {

  float x_min = window_x - window_r;
  float y_min = window_y - window_r;
  float x_max = window_x + window_r;
  float y_max = window_y + window_r;

  for (size_t screen_x = MARGIN_X; screen_x < SCREEN_WIDTH_PIXELS - MARGIN_X;
       ++screen_x) {

    float x = ((float)screen_x / (float)SCREEN_HEIGHT_LINES) * (x_max - x_min) +
              x_min;

    for (size_t screen_y = 0; screen_y < SCREEN_HEIGHT_LINES; ++screen_y) {

      float y =
          ((float)screen_y / (float)SCREEN_HEIGHT_LINES) * (y_max - y_min) +
          y_min;

      color_t color = (color_t)(mandelbrot(x + y * I) * 6.0 * 6.0 * 6.0) + 16;
      size_t offset = pixel_offset(screen, screen_y, screen_x);

      set_color(screen, offset, screen_x, color);

      if (cancel && cancel()) {
        return;
      }
    }
  }
}
#endif

#define COLOR_TEST_BASE_COLORS 16

#define COLOR_TEST_CUBE_SIZE 6
#define COLOR_TEST_GRAYSCALE 24

#define COLOR_TEST_ROWS (2 + COLOR_TEST_CUBE_SIZE)
#define COLOR_TEST_ROW_HEIGHT_LINES (SCREEN_HEIGHT_LINES / COLOR_TEST_ROWS)
#define COLOR_TEST_ROW_PADDING_LINES 2

#define COLOR_TEST_BASE_COLORS_WIDTH_PIXELS                                    \
  (SCREEN_WIDTH_PIXELS / COLOR_TEST_BASE_COLORS)
#define COLOR_TEST_GRAYSCALE_WIDTH_PIXELS                                      \
  (SCREEN_WIDTH_PIXELS / COLOR_TEST_GRAYSCALE)
#define COLOR_TEST_GRAYSCALE_OFFSET_PIXELS 4
#define COLOR_TEST_CUBE_WIDTH_PIXELS                                           \
  (SCREEN_WIDTH_PIXELS / (COLOR_TEST_CUBE_SIZE * COLOR_TEST_CUBE_SIZE))

#define COLOR_TEST_CUBE_OFFSET 10

void screen_test_colors(struct screen *screen) {
  size_t line = 0;

  for (size_t sub_line = 0;
       sub_line < COLOR_TEST_ROW_HEIGHT_LINES - COLOR_TEST_ROW_PADDING_LINES;
       ++sub_line) {

    for (size_t color = 0; color < COLOR_TEST_BASE_COLORS; ++color) {
      size_t pixel = COLOR_TEST_BASE_COLORS_WIDTH_PIXELS * color;

      for (size_t sub_pixel = 0;
           sub_pixel < COLOR_TEST_BASE_COLORS_WIDTH_PIXELS; ++sub_pixel) {
        size_t offset =
            pixel_offset(screen, line + sub_line, pixel + sub_pixel);
        set_color(screen, offset, pixel + sub_pixel, color);
      }
    }
  }

  line += COLOR_TEST_ROW_HEIGHT_LINES;

  for (size_t cube = 0; cube < COLOR_TEST_CUBE_SIZE; ++cube) {

    for (size_t sub_line = 0;
         sub_line < COLOR_TEST_ROW_HEIGHT_LINES - COLOR_TEST_ROW_PADDING_LINES;
         ++sub_line) {

      for (size_t color = 0;
           color < COLOR_TEST_CUBE_SIZE * COLOR_TEST_CUBE_SIZE; ++color) {
        size_t pixel =
            COLOR_TEST_CUBE_OFFSET + COLOR_TEST_CUBE_WIDTH_PIXELS * color;

        for (size_t sub_pixel = 0; sub_pixel < COLOR_TEST_CUBE_WIDTH_PIXELS;
             ++sub_pixel) {
          size_t offset =
              pixel_offset(screen, line + sub_line, pixel + sub_pixel);
          set_color(screen, offset, pixel + sub_pixel,
                    COLOR_TEST_BASE_COLORS +
                        (cube * COLOR_TEST_CUBE_SIZE * COLOR_TEST_CUBE_SIZE) +
                        color);
        }
      }
    }

    line += COLOR_TEST_ROW_HEIGHT_LINES;
  }

  for (size_t sub_line = 0;
       sub_line < COLOR_TEST_ROW_HEIGHT_LINES - COLOR_TEST_ROW_PADDING_LINES;
       ++sub_line) {

    for (size_t color = 0; color < COLOR_TEST_GRAYSCALE; ++color) {

      size_t pixel = COLOR_TEST_GRAYSCALE_OFFSET_PIXELS +
                     COLOR_TEST_GRAYSCALE_WIDTH_PIXELS * color;

      for (size_t sub_pixel = 0; sub_pixel < COLOR_TEST_GRAYSCALE_WIDTH_PIXELS;
           ++sub_pixel) {
        size_t offset =
            pixel_offset(screen, line + sub_line, pixel + sub_pixel);
        set_color(screen, offset, pixel + sub_pixel,
                  COLOR_TEST_BASE_COLORS +
                      (COLOR_TEST_CUBE_SIZE * COLOR_TEST_CUBE_SIZE *
                       COLOR_TEST_CUBE_SIZE) +
                      color);
      }
    }
  }
}
