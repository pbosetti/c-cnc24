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

#ifdef __cplusplus
extern "C" {
#endif

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
program_t *program_new(machine_t *machine);
void program_free(program_t *program);
void program_print(program_t *program, FILE *output);

/* Accessors ******************************************************************/
char *program_filename(program_t const *p);
size_t program_length(program_t const *p);
block_t *program_first(program_t const *p);
block_t *program_current(program_t const *p);
block_t *program_last(program_t const *p);


/* Methods ********************************************************************/
ccnc_error_t program_parse(program_t *program, char const *filename);
ccnc_error_t program_add_block(program_t *program, char const *block);
void program_reset(program_t *program);
block_t *program_next(program_t *program);
void program_clear(program_t *p);



#ifdef __cplusplus
}
#endif
#endif // PROGRAM_H