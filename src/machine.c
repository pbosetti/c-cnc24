/*
  __  __            _     _
 |  \/  | __ _  ___| |__ (_)_ __   ___
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \
 | |  | | (_| | (__| | | | | | | |  __/
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|

*/

#include "machine.h"
#include "toml.h"

/*
  ____        __ _       _ _   _
 |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___
 | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
 | |_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/

*/

typedef struct machine {
  data_t A;                     // Maximum acceleration (m/s/s)
  data_t tq;                    // Sampling time (s)
  data_t max_error, error;      // Maximum and actual positioning error (mm)
  data_t fmax;                  // Maximum feedrate (mm/min)
  point_t *zero;                // Initial machine position
  point_t *setpoint, *position; // Setpoint and actual position
} machine_t;

#define BUFLEN 1024

/*
  _____                 _   _
 |  ___|   _ _ __   ___| |_(_) ___  _ __  ___
 | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
 |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

*/

/* LIFECYCLE ******************************************************************/

machine_t *machine_new(char const *config_path) {
  machine_t *m = NULL;
  FILE *ini_file = NULL;
  toml_table_t *conf = NULL;
  char errbuf[BUFLEN];

  // allocate memory
  m = malloc(sizeof(*m));
  if (!m) {
    eprintf("Could not allocate memory for machine object\n");
    return NULL;
  }

  // 1. Set defaults ===========================================================
  memset(m, 0, sizeof(*m));
  m->A = 100;
  m->max_error = 0.010;
  m->tq = 0.005;
  m->zero = point_new();
  m->setpoint = point_new();
  m->position = point_new();
  point_set_xyz(m->zero, 0, 0, 0);
  point_set_xyz(m->setpoint, 0, 0, 0);
  point_set_xyz(m->position, 0, 0, 0);

  // 2. Open the INI file ======================================================
  ini_file = fopen(config_path, "r");
  if (!ini_file) {
    eprintf("Could not open file %s\n", config_path);
    free(m);
    return NULL;
  }

  // 3. Parse the INI file =====================================================
  conf = toml_parse_file(ini_file, errbuf, BUFLEN);
  fclose(ini_file);
  if (!conf) {
    eprintf("Could not parse %s, error: %s\n", config_path, errbuf);
    free(m);
    return NULL;
  }

  // 4. Extract values from the INI file =======================================
  toml_datum_t d;
  toml_table_t *ccnc = toml_table_in(conf, "C_CNC");
  if (!ccnc) {
    eprintf("Missing mandatory section [C-CNC]\n");
    free(m);
    return NULL;
  }
  
  // We use a macro function for repetitive tasks
#define T_READ_D(d, machine, tab, key)                                         \
  d = toml_double_in(tab, #key);                                               \
  if (!d.ok) {                                                                 \
    wprintf("Missing key %s:%s, using default\n", toml_table_key(tab), #key);  \
  } else {                                                                     \
    machine->key = d.u.d;                                                      \
  }

  T_READ_D(d, m, ccnc, A);
  T_READ_D(d, m, ccnc, max_error);
  T_READ_D(d, m, ccnc, tq);
  T_READ_D(d, m, ccnc, fmax);

  // read arrays:
  toml_array_t *point = toml_array_in(ccnc, "zero");
  if (!point) {
    wprintf("Missing C-CNC:zero, using default\n");
  } else {
    point_set_xyz(m->zero, toml_double_at(point, 0).u.d,
                  toml_double_at(point, 1).u.d, toml_double_at(point, 2).u.d);
  }

  // 5. Clean memory and return ================================================
  toml_free(conf);
  return m;
}

void machine_free(machine_t *m) {}


/* ACCESSORS ******************************************************************/



/* METHODS ********************************************************************/

void machine_print_params(machine_t *m) {}
