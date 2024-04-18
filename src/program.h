/*
  ____                                             _               
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___ 
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/                                            
*/

#ifndef PROGRAM_H
#define PROGRAM_H

#include "defines.h"
#include "block.h"
#include "machine.h"

/*
  ____            _                 _   _                 
 |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___ 
 | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
 | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
 |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
                                                          
*/

typedef struct program program_t;


/*
  _____                 _   _                 
 |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
 | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
 |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                              
*/

/* Lifecycle ******************************************************************/
program_t *program_new(char const *filename);
void program_free(program_t *program);
void program_print(program_t *program, FILE *output);

/* Accessors ******************************************************************/
char *program_filename(program_t const *p);
size_t program_length(program_t const *p);
block_t *program_first(program_t const *p);
block_t *program_current(program_t const *p);
block_t *program_last(program_t const *p);


/* Methods ********************************************************************/
ccnc_error_t program_parse(program_t *program, machine_t const *machine);
void program_reset(program_t *program);
block_t *program_next(program_t *program);




#endif // PROGRAM_H