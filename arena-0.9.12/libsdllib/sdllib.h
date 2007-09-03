#ifndef LIBSDL_H
#define LIBSDL_H

#include <sys/types.h>
#include <sys/stat.h>

#include "../libmisc/misc.h"
#include "../libruntime/runtime.h"
#include "../libeval/eval.h"

/*
 * Init functions
 */
value *init_init(arena_state *s, unsigned int argc, value **argv);

/*
 * Register functions
 */
void sdllib_register(arena_state *s, int argc, char **argv);

#endif
