#include "terminal.h"

__attribute__((aligned(1024), space(prog), section(".nvm")))
const struct terminal_config terminal_config = {
    .format_rows = FORMAT_24_ROWS,
    .monochrome_transform = MONOCHROME_TRANSFORM_LUMINANCE,

    .baud_rate = BAUD_RATE_115200,
    .stop_bits = STOP_BITS_1,
    .parity = PARITY_NONE,
    .serial_inverted = false,

    .charset = CHARSET_UTF8,
    .keyboard_compatibility = KEYBOARD_COMPATIBILITY_PC,
    .keyboard_layout = KEYBOARD_LAYOUT_US,
    .receive_c1_mode = C1_MODE_8BIT,
    .transmit_c1_mode = C1_MODE_7BIT,

    .auto_wrap_mode = true,
    .screen_mode = false,

    .send_receive_mode = true,

    .new_line_mode = false,
    .cursor_key_mode = false,
    .auto_repeat_mode = true,
    .ansi_mode = true,
    .backspace_mode = false,
    .application_keypad_mode = false,

    .flow_control = true,

    .start_up = START_UP_MESSAGE,
};