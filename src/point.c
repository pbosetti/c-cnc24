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
    eprintf("Error allocating memory for a point\n");
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

#define FIELD_SIZE 8
#define FORMAT "[" RED "%s " GRN "%s " BLU "%s" CRESET "]"
void point_inspect(point_t const *p, char **desc) {
  assert(p);
  char str_x[FIELD_SIZE + 1];
  char str_y[FIELD_SIZE + 1];
  char str_z[FIELD_SIZE + 1];
  if (p->m & X_SET) {
    sprintf(str_x, "%*.3f", FIELD_SIZE, p->x);
  } else {
    sprintf(str_x, "%*s", FIELD_SIZE, "---");
  }
  if (p->m & Y_SET) {
    sprintf(str_y, "%*.3f", FIELD_SIZE, p->y);
  } else {
    sprintf(str_y, "%*s", FIELD_SIZE, "---");
  }
  if (p->m & Z_SET) {
    sprintf(str_z, "%*.3f", FIELD_SIZE, p->z);
  } else {
    sprintf(str_z, "%*s", FIELD_SIZE, "---");
  }
  if (*desc) {
    snprintf(*desc, strlen(*desc) + 1, FORMAT, str_x, str_y, str_z);
  } else {
    if (asprintf(desc, FORMAT, str_x, str_y, str_z) == -1) {
      eprintf("Could not allocate memory for point description\n");
      exit(EXIT_FAILURE);
    }
  }
}

/* ACCESSORS ******************************************************************/

void point_set_xyz(point_t *p, data_t x, data_t y, data_t z) {
  assert(p);
  p->x = x;
  p->y = y;
  p->z = z;
  p->m = XYZ_SET;
}

// D.R.Y.
#define point_accessors(axis, bitmask)                                         \
  void point_set_##axis(point_t *p, data_t value) {                            \
    assert(p);                                                                 \
    p->axis = value;                                                           \
    p->m = p->m | bitmask;                                                     \
  }                                                                            \
  data_t point_##axis(point_t const *p) {                                      \
    assert(p);                                                                 \
    return p->axis;                                                            \
  }

point_accessors(x, X_SET);
point_accessors(y, Y_SET);
point_accessors(z, Z_SET);

/* METHODS ********************************************************************/

data_t point_dist(point_t const *from, point_t const *to) {
  assert(from && to);
  return sqrt(pow(to->x - from->x, 2) + pow(to->y - from->y, 2) +
              pow(to->z - from->z, 2));
}

void point_delta(point_t const *from, point_t const *to, point_t *delta) {
  assert(from && to && delta);
  point_set_xyz(delta, to->x - from->x, to->y - from->y, to->z - from->z);
}

void point_modal(point_t const *from, point_t *to) {
  assert(from && to);
  if (!(to->m & X_SET) && (from->m & X_SET)) {
    point_set_x(to, from->x);
  }
  if (!(to->m & Y_SET) && (from->m & Y_SET)) {
    point_set_y(to, from->y);
  }
  if (!(to->m & Z_SET) && (from->m & Z_SET)) {
    point_set_z(to, from->z);
  }
}

/*
  _____         _                     _
 |_   _|__  ___| |_   _ __ ___   __ _(_)_ __
   | |/ _ \/ __| __| | '_ ` _ \ / _` | | '_ \
   | |  __/\__ \ |_  | | | | | | (_| | | | | |
   |_|\___||___/\__| |_| |_| |_|\__,_|_|_| |_|

*/

#ifdef POINT_MAIN
int main() {
  point_t *p1, *p2, *p3;
  data_t distance;
  char *desc = NULL;
  printf(BGRN "Point class test executable, version %s (%s)\n" CRESET, VERSION,
         BUILD_TYPE);

  // Create three points
  p1 = point_new();
  p2 = point_new();
  p3 = point_new();

  // Initialize points p1 and p2
  point_set_xyz(p1, 0, 0, 12);
  point_set_x(p2, 100);
  point_set_y(p2, 100);
  point_inspect(p2, &desc);
  printf("Initial p2: %s\n", desc);

  // Modal:
  point_modal(p1, p2);
  point_inspect(p2, &desc);
  printf("After modal p2: %s\n", desc);
  
  // Distance;
  distance = point_dist(p1, p2);
  printf("Distance p1->p2: %f\n", distance);
  
  // Projections:
  point_delta(p1, p2, p3);
  point_inspect(p3, &desc);
  printf("Delta p1-p2: %s\n", desc);

  // Free the memory
  point_free(p1);
  point_free(p2);
  point_free(p3);
  free(desc);
  return 0;
}

#endif