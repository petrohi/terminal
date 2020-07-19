#include "terminal_config.h"

const static uint32_t baud_rates[] = {
    [BAUD_RATE_110] = 110,       [BAUD_RATE_150] = 150,
    [BAUD_RATE_300] = 300,       [BAUD_RATE_1200] = 1200,
    [BAUD_RATE_2400] = 2400,     [BAUD_RATE_4800] = 4800,
    [BAUD_RATE_9600] = 9600,     [BAUD_RATE_19200] = 19200,
    [BAUD_RATE_38400] = 38400,   [BAUD_RATE_57600] = 57600,
    [BAUD_RATE_115200] = 115200, [BAUD_RATE_230400] = 230400,
    [BAUD_RATE_460800] = 460800, [BAUD_RATE_921600] = 921600,
};

uint32_t
terminal_config_get_baud_rate(struct terminal_config *terminal_config) {
  return baud_rates[terminal_config->baud_rate];
}
