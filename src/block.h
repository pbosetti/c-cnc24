/*
  ____  _            _           _               
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___ 
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/
                                                 
*/
#ifndef BLOCK_H
#define BLOCK_H

#include "defines.h"
#include "point.h"
#include "machine.h"

/*
  ____            _                 _   _                 
 |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___ 
 | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
 | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
 |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
                                                          
*/

typedef struct block block_t;

typedef enum {
  RAPID = 0,
  LINE,
  CWA,
  CCWA,
  NO_MOTION
} block_type_t;

/*
  _____                 _   _                 
 |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
 | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
 |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                              
*/

/* LIFECYCLE ******************************************************************/
block_t *block_new(char const *line, block_t *prev, machine_t *machine);
void block_free(block_t *b);
void block_print(block_t const *b, FILE *out);


/* ACCESSORS ******************************************************************/
data_t block_length(block_t const *b);
data_t block_dtheta(block_t const *b);
block_type_t block_type(block_t const *b);
data_t block_dt(block_t const *b);
char *block_line(block_t const *b);
size_t block_n(block_t const *b);
data_t block_r(block_t const *b);
point_t *block_center(block_t const *b);
point_t *block_target(block_t const *b);
block_t *block_next(block_t const *b);


/* METHODS ********************************************************************/
ccnc_error_t block_parse(block_t *b);
data_t block_lambda(block_t *b, data_t time, data_t *speed);
point_t *block_interpolate(block_t *b, data_t lambda);



#endif // BLOCK_H