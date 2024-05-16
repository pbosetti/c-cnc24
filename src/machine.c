/*
  __  __            _     _
 |  \/  | __ _  ___| |__ (_)_ __   ___
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \
 | |  | | (_| | (__| | | | | | | |  __/
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|

*/

#include "machine.h"
#include "toml.h"
#include <mqtt_protocol.h>
#include <signal.h> // for signal handling
#include <unistd.h> // for usleep

/*
  ____        __ _       _ _   _
 |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___
 | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
 | |_| |  __/  _| | | | | | |_| | (_) | | | \__ \
 |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/

*/
#define BUFLEN 1024

typedef struct machine {
  data_t A;                     // Maximum acceleration (m/s/s)
  data_t tq;                    // Sampling time (s)
  data_t max_error, error;      // Maximum and actual positioning error (mm)
  data_t fmax;                  // Maximum feedrate (mm/min)
  point_t *zero;                // Initial machine position
  point_t *setpoint, *position; // Setpoint and actual position
  point_t *offset;              // Workpiece origin coordinates
  /* MQTT SECTION */
  char broker_address[BUFLEN];
  int broker_port;
  char pub_topic[BUFLEN];
  char sub_topic[BUFLEN];
  char msg_buffer[BUFLEN];
  struct mosquitto *mqt;
  struct mosquitto_message *msg;
  int connecting;
} machine_t;

// MQTT Callbacks:
static void on_connect(struct mosquitto *, void *, int);
static void on_message(struct mosquitto *, void *,
                       const struct mosquitto_message *);

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
  m->error = 0.0;
  m->tq = 0.005;
  m->zero = point_new();
  m->setpoint = point_new();
  m->position = point_new();
  m->offset = point_new();
  m->connecting = 1;
  point_set_xyz(m->zero, 0, 0, 0);
  point_set_xyz(m->setpoint, 0, 0, 0);
  point_set_xyz(m->position, 0, 0, 0);
  point_set_xyz(m->offset, 0, 0, 0);

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

  // We use a macro function for repetitive tasks
  // note that macros can be defined anywhere, including within functions

  // read a double
#define T_READ_D(d, machine, tab, key)                                         \
  d = toml_double_in(tab, #key);                                               \
  if (!d.ok) {                                                                 \
    wprintf("Missing key %s:%s, using default\n", toml_table_key(tab), #key);  \
  } else {                                                                     \
    machine->key = d.u.d;                                                      \
  }

  // Read an integer
#define T_READ_I(d, machine, tab, key)                                         \
  d = toml_int_in(tab, #key);                                                  \
  if (!d.ok) {                                                                 \
    wprintf("Missing key %s:%s, using default\n", toml_table_key(tab), #key);  \
  } else {                                                                     \
    machine->key = d.u.i;                                                      \
  }

  // Read a string
#define T_READ_S(d, machine, tab, key)                                         \
  d = toml_string_in(tab, #key);                                               \
  if (!d.ok) {                                                                 \
    wprintf("Missing key %s:%s, using default\n", toml_table_key(tab), #key);  \
  } else {                                                                     \
    strncpy(machine->key, d.u.s, BUFLEN);                                      \
  }

  // Reading the C-CNC section
  toml_datum_t d;
  toml_table_t *ccnc = toml_table_in(conf, "C-CNC");
  if (!ccnc) {
    eprintf("Missing mandatory section [C-CNC]\n");
    machine_free(m);
    return NULL;
  }
  T_READ_D(d, m, ccnc, A);
  T_READ_D(d, m, ccnc, max_error);
  T_READ_D(d, m, ccnc, tq);
  T_READ_D(d, m, ccnc, fmax);

  // Arrays must be read in a different way (using toml_double_at()):
  toml_array_t *point = toml_array_in(ccnc, "zero");
  if (!point) {
    wprintf("Missing C-CNC:zero, using default\n");
  } else {
    point_set_xyz(m->zero, toml_double_at(point, 0).u.d,
                  toml_double_at(point, 1).u.d, toml_double_at(point, 2).u.d);
  }
  point = toml_array_in(ccnc, "offset");
  if (!point) {
    wprintf("Missing C-CNC:offset, using default\n");
  } else {
    point_set_xyz(m->offset, toml_double_at(point, 0).u.d,
                  toml_double_at(point, 1).u.d, toml_double_at(point, 2).u.d);
  }

  // read the MQTT section
  toml_table_t *mqtt = toml_table_in(conf, "MQTT");
  if (!mqtt) {
    eprintf("Missing MQTT section\n");
    machine_free(m);
    return NULL;
  }
  T_READ_S(d, m, mqtt, broker_address);
  T_READ_I(d, m, mqtt, broker_port);
  T_READ_S(d, m, mqtt, pub_topic);
  T_READ_S(d, m, mqtt, sub_topic);

  // Initialize MQTT library
  if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS) {
    eprintf("Could not initialize mosquitto library\n");
    machine_free(m);
    return NULL;
  }

  // 5. Clean memory and return ================================================
  toml_free(conf);
  return m;
}

void machine_free(machine_t *m) {
  assert(m);
  if (m->zero)
    point_free(m->zero);
  if (m->position)
    point_free(m->position);
  if (m->setpoint)
    point_free(m->setpoint);
  free(m);
  m = NULL;
}

/* ACCESSORS ******************************************************************/

#define machine_getter(typ, par)                                               \
  typ machine_##par(machine_t const *m) {                                      \
    assert(m);                                                                 \
    return m->par;                                                             \
  }

machine_getter(data_t, A);
machine_getter(data_t, tq);
machine_getter(data_t, max_error);
machine_getter(data_t, error);
machine_getter(data_t, fmax);
machine_getter(point_t *, zero);
machine_getter(point_t *, setpoint);
machine_getter(point_t *, position);
machine_getter(int, connecting);

/* METHODS ********************************************************************/

void machine_print_params(machine_t const *m, FILE *out) {
  fprintf(out, BGRN "Machine parameters:\n" CRESET);
  fprintf(out, BBLK "C-CNC:A:         " CRESET "%f\n", m->A);
  fprintf(out, BBLK "C-CNC:tq:        " CRESET "%f\n", m->tq);
  fprintf(out, BBLK "C-CNC:fmax:      " CRESET "%f\n", m->fmax);
  fprintf(out, BBLK "C-CNC:max_error: " CRESET "%f\n", m->max_error);
  fprintf(out, BBLK "C-CNC:zero:      " CRESET "[%.3f, %.3f, %.3f]\n",
          point_x(m->zero), point_y(m->zero), point_z(m->zero));
  fprintf(out, BBLK "MQTT:broker_addr: " CRESET "%s\n", m->broker_address);
  fprintf(out, BBLK "MQTT:broker_port: " CRESET "%d\n", m->broker_port);
  fprintf(out, BBLK "MQTT:pub_topic: " CRESET "%s\n", m->pub_topic);
  fprintf(out, BBLK "MQTT:sub_topic: " CRESET "%s\n", m->sub_topic);
}

// Connect with the broker and setup callbacks
ccnc_error_t machine_connect(machine_t *m, machine_on_message callback) {
  assert(m);
  m->mqt = mosquitto_new(NULL, 1, m);
  if (!m->mqt) {
    eprintf("Could not create a MQTT client\n");
    return MQTT_ERR;
  }
  mosquitto_connect_callback_set(m->mqt, on_connect);
  if (!callback)
    mosquitto_message_callback_set(m->mqt, on_message);
  else
    mosquitto_message_callback_set(m->mqt, callback);

  if (mosquitto_connect(m->mqt, m->broker_address, m->broker_port, 10) !=
      MOSQ_ERR_SUCCESS) {
    eprintf("Invalid broker parameters\n");
    return MQTT_ERR;
  }
  return NO_ERR;
}

// synchronize the machine object setpoint with the physical machine
// i.e. publish m->setpoint via MQTT
ccnc_error_t machine_sync(machine_t *m, int rapid) {
  assert(m && m->mqt);
  // Transmit a setpoint description in JSON like this:
  // {"x":1.1,"y":23.0,"z":123.0,"rapid":1}
  snprintf(m->msg_buffer, BUFLEN, "{\"x\":%f,\"y\":%f,\"z\":%f,\"rapid\":%d}",
           point_x(m->setpoint) + point_x(m->offset),
           point_y(m->setpoint) + point_y(m->offset),
           point_z(m->setpoint) + point_z(m->offset), rapid);
  if (mosquitto_publish(m->mqt, NULL, m->pub_topic, strlen(m->msg_buffer),
                        m->msg_buffer, 0, 0) != MOSQ_ERR_SUCCESS) {
    eprintf("Could not send message %s\n", m->msg_buffer);
    return MQTT_ERR;
  }
  mosquitto_loop(m->mqt, 1, 1);
  return NO_ERR;
}

// enable receiving MQTT messages from physical machine
ccnc_error_t machine_listen_start(machine_t *m) {
  assert(m && m->mqt);
  if (mosquitto_subscribe(m->mqt, NULL, m->sub_topic, 0) != MOSQ_ERR_SUCCESS) {
    eprintf("Could not subscribe to topic %s\n", m->sub_topic);
    return MQTT_ERR;
  }
  iprintf("Subscribed to topic %s\n", m->sub_topic);
  return NO_ERR;
}

// disable receiving MQTT messages from physical machine
ccnc_error_t machine_listen_stop(machine_t *m) {
  assert(m && m->mqt);
  if (mosquitto_unsubscribe(m->mqt, NULL, m->sub_topic) != MOSQ_ERR_SUCCESS) {
    eprintf("Could not unsubscribe from %s\n", m->sub_topic);
    return MQTT_ERR;
  }
  iprintf("Unsubscribed from topic %s\n", m->sub_topic);
  return NO_ERR;
}

// Disconnect from MQTT broker and stop network operations
void machine_disconnect(machine_t *m) {
  assert(m && m->mqt);
  // Wait for network ops to be completed
  while (mosquitto_want_write(m->mqt)) {
    mosquitto_loop(m->mqt, 1, 1);
    usleep(10000); // avoid consuming 100% of a CPU core for nothing
  }
  mosquitto_disconnect(m->mqt);
  m->connecting = 1;
}

/*
  ____  _        _   _         __                  _   _
 / ___|| |_ __ _| |_(_) ___   / _|_   _ _ __   ___| |_(_) ___  _ __  ___
 \___ \| __/ _` | __| |/ __| | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
  ___) | || (_| | |_| | (__  |  _| |_| | | | | (__| |_| | (_) | | | \__ \
 |____/ \__\__,_|\__|_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

*/
static void on_connect(struct mosquitto *mqtt, void *obj, int rc) {
  machine_t *m = (machine_t *)obj;
  if (rc == CONNACK_ACCEPTED) {
    iprintf("Connection successful to %s:%d\n", m->broker_address,
            m->broker_port);
    if (mosquitto_subscribe(mqtt, NULL, m->sub_topic, 0) != MOSQ_ERR_SUCCESS) {
      eprintf("Could not subscribe to %s\n", m->sub_topic);
      exit(EXIT_FAILURE);
    }
  } else {
    eprintf("Connection error");
    exit(EXIT_FAILURE);
  }
  m->connecting = 0;
}

// Deal with messages coming on topic m->sub_topic, e.g. "c-cnc/status/#"
// Typ. # can be either "error" or "position"
static void on_message(struct mosquitto *mqtt, void *obj,
                       const struct mosquitto_message *msg) {
  machine_t *m = (machine_t *)obj;
  mosquitto_message_copy(m->msg, msg);
  // get last component of topic:
  char *subtopic = strrchr(msg->topic, '/') + 1;
  if (strcmp(subtopic, "error") == 0) {
    m->error = atof(msg->payload);
  } else if (strcmp(subtopic, "position") == 0) {
    char *nxt = msg->payload;
    point_set_x(m->position, strtod(nxt, &nxt));
    point_set_y(m->position, strtod(nxt + 1, &nxt));
    point_set_z(m->position, strtod(nxt + 1, &nxt));
  } else {
    eprintf("Got unexpected subtopic %s\n", msg->topic);
  }
}

/*
  __  __            _     _              _            _
 |  \/  | __ _  ___| |__ (_)_ __   ___  | |_ ___  ___| |_
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \ | __/ _ \/ __| __|
 | |  | | (_| | (__| | | | | | | |  __/ | ||  __/\__ \ |_
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \__\___||___/\__|

*/
#ifdef MACHINE_MAIN
static int _running = 1;

void sighandler(int signum) { _running = 0; }

int main(int argc, char const **argv) {
  // variables declatarion
  machine_t *m = NULL;
  point_t *pos = NULL;
  char *p_desc = NULL;

  // install a signal handler
  signal(SIGINT, sighandler);

  // Check command line arguments
  if (argc != 2) {
    eprintf("Please provide the path to a, INI file! \n");
    exit(EXIT_FAILURE);
  }
  // Create a new machine instance
  m = machine_new(argv[1]);
  if (!m) {
    eprintf("Could not create machine object!\n");
    exit(EXIT_FAILURE);
  }
  // Print machine parameters
  machine_print_params(m, stdout);

  // Test MQTT connectivity
  machine_connect(m, &on_message);
  machine_listen_start(m);
  while (_running) {
    machine_sync(m, 0);
    pos = machine_position(m);
    point_inspect(pos, &p_desc);
    // use \r rather than \n to go back with cursor at the beginning 
    // of the same line rather than on a new line
    // Note: \r does not update the console, as \n does, so we need to 
    // manually call fflush(stout) eventually
    printf("Position: %s, error: %.3f\r", p_desc, machine_error(m));
    usleep(10000);
    fflush(stdout);
  }

  // free memory
  machine_listen_stop(m);
  machine_disconnect(m);
  machine_free(m);
  free(p_desc);
  return 0;
}

#endif