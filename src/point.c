/*
  ____       _       _          _               
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___ 
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/
                                                
* This is the implementation of the Point class of C-CNC
*/
#include "point.h"
#include <math.h>

/*
  ____        __ _       _ _   _                                                                           
 |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___                                                           
 | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|                                                          
 | |_| |  __/  _| | | | | | |_| | (_) | | | \__ \                                                          
 |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/                                                          
                                                                                                           
*/

// Structure representing the Point class
typedef struct point {
  data_t x, y, z; // coordinates
  uint8_t m;      // bitmask
} point_t;

#define X_SET '\1'
#define Y_SET '\2'
#define Z_SET '\4'
#define XYZ_SET '\7'


/*
  _____                 _   _                 
 |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
 | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
 |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                              
*/

/* LIFECYCLE ******************************************************************/
point_t *point_new() {
  point_t *p = malloc(sizeof(point_t));
  if (!p) {
    printf("Error allocating memory for a point");
    return NULL;
  }
  memset(p, 0, sizeof(*p));
  return p;
}

void point_free(point_t *p) {
  assert(p);
  free(p);
  p = NULL; // Not mandatory but safer!
}

void point_inspect(point_t const *p, char **description) {}


/* ACCESSORS ******************************************************************/

// Setters
// Bitmask logic:
// xxxx xxxx -> initial value of bitmask
// 0000 0001 -> X_SET, value of '\1'
// ---------
// xxxx xxx1 -> result of a bitwise OR
void point_set_x(point_t *p, data_t x) {
  assert(p);
  p->x = x;
  p->m = p->m | X_SET;
}

// Bitmask logic:
// xxxx xxxx -> initial value of bitmask
// 0000 0010 -> X_SET, value of '\2'
// ---------
// xxxx xx1x -> result of a bitwise OR
void point_set_y(point_t *p, data_t y) {
  assert(p);
  p->y = y;
  p->m = p->m | Y_SET;
}

// Bitmask logic:
// xxxx xxxx -> initial value of bitmask
// 0000 0100 -> Z_SET, value of '\4'
// ---------
// xxxx x1xx -> result of a bitwise OR
void point_set_z(point_t *p, data_t z) {
  assert(p);
  p->z = z;
  p->m = p->m | Z_SET;
}

void point_set_xyz(point_t *p, data_t x, data_t y, data_t z) {
  assert(p);
  p->x = x;
  p->y = y;
  p->z = z;
  p->m = XYZ_SET;
}

// Getters
data_t point_x(point_t const *p) {
  assert(p);
  return p->x;
}

data_t point_y(point_t const *p) {
  assert(p);
  return p->y;
}
data_t point_z(point_t const *p) {
  assert(p);
  return p->z;
}


/* METHODS ********************************************************************/

data_t point_dist(point_t const *from, point_t const *to) {
  assert(from && to);
  return sqrt(
    pow(to->x - from->x, 2) +
    pow(to->y - from->y, 2) +
    pow(to->z - from->z, 2)
    );
}

void point_delta(point_t const *from, point_t const *to, point_t *delta) {
  assert(from && to && delta);
  point_set_xyz(delta,
    to->x - from->x,
    to->y - from->y,
    to->z - from->z
  );
}

void point_modal(point_t const *from, point_t *to) {}




