/*
   ____ ____ _   _  ____                      
  / ___/ ___| \ | |/ ___|   _____  _____  ___ 
 | |  | |   |  \| | |      / _ \ \/ / _ \/ __|
 | |__| |___| |\  | |___  |  __/>  <  __/ (__ 
  \____\____|_| \_|\____|  \___/_/\_\___|\___|
                                              
*/

#include "../defines.h"
#include "../fsm.h"
#include <sys/time.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <syslog.h>

#define INI_FILE "machine.ini"

void handler(int signal) {}

int main(int argc, char const **argv) {
  struct itimerval itv;
  // create and populate the FSM data structure
  ccnc_state_data_t state_data = {
    .ini_file = INI_FILE,
    .prog_file = (char *)argv[1],
    .machine = machine_new(INI_FILE),
    .program = NULL
  };
  ccnc_state_t cur_state = CCNC_STATE_INIT;
  // timing values (note that machine.ini has values in seconds, so we need to 
  // convert to microseconds)
  useconds_t dt = machine_tq(state_data.machine) * 1E6;
  useconds_t dt_max = dt * 10;

  if (!state_data.machine) {
    eprintf("Error initializeng the machine object\n");
    exit(EXIT_FAILURE);
  }

  itv.it_interval.tv_sec = 0.0;
  itv.it_interval.tv_usec = dt;
  itv.it_value.tv_sec = 0.0;
  itv.it_value.tv_usec = dt;

  // define and empty function as signal handler
  signal(SIGALRM, handler);
  // prepare the timer
  if (setitimer(ITIMER_REAL, &itv, NULL)) {
    eprintf("Could not set the timer\n");
    exit(EXIT_FAILURE);
  }

  // Setup system logging
  // too see the logs, run the following in a separate terminal WHILE the 
  // program is running:
  // Mac:       log stream --predicate 'process == "ccnc"' --info --style syslog
  // Linux/WSL: tail -f /var/log/syslog | grep CCNC
  openlog("CCNC v" VERSION, LOG_PID, LOG_USER);
  syslog(LOG_INFO, "[FSM] Starting CCNC --->");

  // Main loop
  do {
    cur_state = ccnc_run_state(cur_state, &state_data);
    if (usleep(dt_max) == 0) {
      wprintf("Did not complete the loop iteration in less than %d us\n", dt_max);
    }
  } while (cur_state != CCNC_STATE_STOP);
  // run the final state once more
  ccnc_run_state(cur_state,  &state_data);

  syslog(LOG_INFO, "[FSM] Stopping CCNC <---");

  return 0;
}