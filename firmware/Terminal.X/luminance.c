#include "luminance.h"

#include "luminance_table.h"

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
