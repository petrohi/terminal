#include "luminance.h"

#include "rgb.h"

static uint8_t luminance_table[RGB_TABLE_SIZE];

#define RED_LUMINANCE 0.2126
#define GREEN_LUMINANCE 0.7152
#define BLUE_LUMINANCE 0.0722

void luminance_init() {
  for (size_t i = 0; i < RGB_TABLE_SIZE; ++i) {
    rgb_t rgb = rgb_table[i];
    luminance_table[i] =
        (uint8_t)((float)(rgb & 0xff) * BLUE_LUMINANCE +
                  (float)((rgb >> 8) & 0xff) * GREEN_LUMINANCE +
                  (float)((rgb >> 16) & 0xff) * RED_LUMINANCE);
  }
}

void luminance_to_monochrome(color_t *active, color_t *inactive) {
  uint8_t active_luminance = luminance_table[*active];
  uint8_t inactive_luminance = luminance_table[*inactive];

  if (active_luminance > inactive_luminance) {
    *active = DEFAULT_ACTIVE_COLOR;
    *inactive = DEFAULT_INACTIVE_COLOR;
  } else {
    *active = DEFAULT_INACTIVE_COLOR;
    *inactive = DEFAULT_ACTIVE_COLOR;
  }
}
