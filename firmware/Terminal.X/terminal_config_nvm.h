#include "terminal.h"

// This is the workaround for xc32 v3 compiler bug causing
// "Incompatible section flags for identifier terminal_config" error.
//
// See: https://www.microchip.com/forums/m1173831.aspx

extern volatile const struct terminal_config terminal_config;