/*
  ____  _            _           _               
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___ 
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/
                                                 
*/
#include "block.h"
#include "defines.h"


/*
  ____        __ _       _ _   _                 
 |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___ 
 | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
 | |_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
                                                 
*/

// Velocity profile data
typedef struct {
  data_t a, d;             // actual accelerations
  data_t f, l;             // actual feedrate and length
  data_t fs, fe;           // initial and final feedrate
  data_t dt_1, dt_m, dt_2; // durations
  data_t dt;               // total duration
} block_profile_t;

typedef struct block {
  char *line;               // G-code line as a string
  block_type_t type;        // block type
  size_t n;                 // block number
  size_t tool;              // tool number
  data_t feedrate;          // nominal feedrate
  data_t arc_feedrate;      // actual feedrate along an arc
  data_t spindle;           // spindle rotational speed
  point_t *target;          // target position
  point_t *delta;           // segment projections
  point_t *center;          // arc center coordinates
  data_t length;            // segment/arc length
  data_t i, j, r;           // arc parameters (offsets and radius)
  data_t theta_0, dtheta;   // initial angle and arc angle
  data_t acc;               // actual acceleration
  machine_t const *machine; // the machine reference
  block_profile_t *prof;    // the speed profile
  struct block *next;       // reference to the next block
  struct block *prev;       // reference to the previous block
} block_t;