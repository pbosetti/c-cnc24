/*
  __  __            _     _            
 |  \/  | __ _  ___| |__ (_)_ __   ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \
 | |  | | (_| | (__| | | | | | | |  __/
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|
                                       
*/
#ifndef MACHINE_H
#define MACHINE_H

#include "defines.h"
#include "point.h"

/*
  ____            _                 _   _                 
 |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___ 
 | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
 | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
 |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
                                                          
*/

typedef struct machine machine_t;


/*
  _____                 _   _                 
 |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
 | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
 |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                              
*/

/* LIFECYCLE ******************************************************************/
machine_t *machine_new(char const *config_path);
void machine_free(machine_t *m);


/* ACCESSORS ******************************************************************/
data_t machine_A(machine_t const *m);
data_t machine_tq(machine_t const *m);
data_t machine_max_error(machine_t const *m);
data_t machine_error(machine_t const *m);
data_t machine_fmax(machine_t const *m);
point_t *machine_zero(machine_t const *m);
point_t *machine_setpoint(machine_t const *m);
point_t *machine_position(machine_t const *m);


/* METHODS ********************************************************************/
void machine_print_params(machine_t const *m, FILE *out);



#endif // MACHINE_H