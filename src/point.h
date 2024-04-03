/*
  ____       _       _          _               
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___ 
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/
                                                
* This is the header for the Point class of C-CNC
*/
#ifndef POINT_H
#define POINT_H

#include "defines.h"

/*
  ____            _                 _   _                 
 |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___ 
 | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
 | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
 |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
                                                          
*/

// Opaque structure representing the Point class
typedef struct point point_t;


/*
  _____                 _   _                 
 |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
 | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
 |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                              
*/

/* LIFECYCLE ******************************************************************/
point_t *point_new();
void point_free(point_t *p);
// Provides description of p in a nuely allocated string 
void point_inspect(point_t const *p, char **description);


/* ACCESSORS ******************************************************************/
// Setters
void point_set_x(point_t *p, data_t x);
void point_set_y(point_t *p, data_t y);
void point_set_z(point_t *p, data_t z);
void point_set_xyz(point_t *p, data_t x, data_t y, data_t z);

// Getters
data_t point_x(point_t const *p);
data_t point_y(point_t const *p);
data_t point_z(point_t const *p);


/* METHODS ********************************************************************/
// Calculates the distance between two points
data_t point_dist(point_t const *from, point_t const *to);

// Calculates the projection of a segment joining from->to, puts the results
// into delta
void point_delta(point_t const *from, point_t const *to, point_t *delta);

// Copies coordinate missing in point to, from point from
void point_modal(point_t const *from, point_t *to);


#endif // POINT_H


