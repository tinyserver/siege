/*
 * Register function
 * (C) 2006, Pascal Schmidt <arena-language@ewetel.net>
 * see file ../doc/LICENSE for license
 */

#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdllib.h"

/*
 * sdllib function definition
 */
typedef struct {
  char		*name;
  call_func	vector;
  unsigned int	args;
  char		*proto;
  char		rettype;
} sdllib_func;

/*
 * sdllib functions to register
 */
static sdllib_func sdl[] = {
  /* init functions */
  { "sdl_init",  init_init,  0,  NULL,  0 },
  
  /* list terminator */
  { NULL,  NULL,  0,  NULL,  0 }
};

/*
 * Register sdllib functions
 *
 * This function registers the sdllib functions in the current
 * symbol table. Usually this means the global symbol table.
 */
void sdllib_register(arena_state *s, int argc, char **argv)
{
  signature sig;
  unsigned int i;
  
  sig.type = FUNCTION_TYPE_BUILTIN;

  i = 0;
  while (sdl[i].name) {
    sig.args  = sdl[i].args;
    sig.name  = sdl[i].name;
    sig.proto = sdl[i].proto;
    sig.call_u.builtin_vector = sdl[i].vector;
    sig.rettype = sdl[i].rettype;
    symtab_stack_add_function(s, sdl[i].name, &sig);
    i++;
  }

  // file_init(s);  
  // preg_init(s);
}
