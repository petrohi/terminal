#pragma once

#include "keys.h"

enum keys_entry_type {
  IGNORE,
  CHR,
  STR,
  ESC,
  CSI,
  CSI_MOD_1,
  CSI_MOD_1_1,
  SS3,
  SS3_MOD_CSI,
  HANDLER,
  ROUTER,
};

struct keys_entry;

struct keys_router {
  size_t (*router)(struct terminal *);
  const struct keys_entry *entries;
};

union keys_variant {
  character_t character;
  const char *string;
  void (*handler)(struct terminal *);
  const struct keys_router *router;
};

struct keys_entry {
  enum keys_entry_type type;
  union keys_variant data;
};

#define KEY_CHR(c)                                                             \
  {                                                                            \
    CHR, { .character = c }                                                    \
  }
#define KEY_STR(s)                                                             \
  {                                                                            \
    STR, { .string = s }                                                       \
  }
#define KEY_ESC(s)                                                             \
  {                                                                            \
    ESC, { .string = s }                                                       \
  }
#define KEY_CSI(s)                                                             \
  {                                                                            \
    CSI, { .string = s }                                                       \
  }
#define KEY_CSI_MOD_1(s)                                                       \
  {                                                                            \
    CSI_MOD_1, { .string = s }                                                 \
  }
#define KEY_CSI_MOD_1_1(s)                                                     \
  {                                                                            \
    CSI_MOD_1_1, { .string = s }                                               \
  }
#define KEY_SS3(s)                                                             \
  {                                                                            \
    SS3, { .string = s }                                                       \
  }
#define KEY_SS3_MOD_CSI(s)                                                     \
  {                                                                            \
    SS3_MOD_CSI, { .string = s }                                               \
  }
#define KEY_HANDLER(h)                                                         \
  {                                                                            \
    HANDLER, { .handler = h }                                                  \
  }
#define KEY_ROUTER(r, ...)                                                     \
  {                                                                            \
    ROUTER, {                                                                  \
      .router = &(const struct keys_router) {                                  \
        r, (const struct keys_entry[]) { __VA_ARGS__ }                         \
      }                                                                        \
    }                                                                          \
  }

void terminal_keyboard_set_keys_entries(
    struct terminal *terminal, const struct keys_entry *keys_entries);
