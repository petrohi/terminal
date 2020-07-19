#include "font.h"

#include <stdlib.h>

const uint8_t *find_glyph(const struct bitmap_font *font,
                          unsigned short codepoint) {
  size_t first = 0;
  size_t last = font->codepoints_length - 1;
  size_t middle = (first + last) / 2;

  while (first <= last) {
    if (font->codepoints[middle] < codepoint)
      first = middle + 1;
    else if (font->codepoints[middle] == codepoint)
      return font->data + (font->codepoints_map[middle] * font->height);
    else
      last = middle - 1;

    middle = (first + last) / 2;
  }

  return NULL;
}
