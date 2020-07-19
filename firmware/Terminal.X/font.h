#pragma once

#include <stdint.h>
#include <stdlib.h>

struct bitmap_font {
  int height;
  int width;
  const uint8_t *data;
  size_t codepoints_length;
  const int *codepoints;
  const int *codepoints_map;
};

const unsigned char *find_glyph(const struct bitmap_font *font,
                                unsigned short codepoint);
