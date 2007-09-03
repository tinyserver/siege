#include <stdio.h>
#include <stdlib.h>

#include "sdllib.h"

/*
 * Init SDL
 */
value *init_init(arena_state *s, unsigned int argc, value **argv)
{
  return value_make_void();
}
