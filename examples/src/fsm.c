/******************************************************************************
Finite State Machine
Project: Example
Description: Example finite state machine

Generated by gv_fsm ruby gem, see https://rubygems.org/gems/gv_fsm
gv_fsm version 0.3.4
Generation date: 2024-05-09 11:00:49 +0200
Generated from: example.dot
The finite state machine has:
  3 states
  1 transition functions
Functions and types have been generated with prefix "example_"
******************************************************************************/

#include <syslog.h>
#include <stdio.h>
#include "fsm.h"

// SEARCH FOR Your Code Here FOR CODE INSERTION POINTS!

// GLOBALS
// State human-readable names
const char *example_state_names[] = {"init", "run", "stop"};

// List of state functions
state_func_t *const example_state_table[EXAMPLE_NUM_STATES] = {
  example_do_init, // in state init
  example_do_run,  // in state run
  example_do_stop, // in state stop
};

// Table of transition functions
transition_func_t *const example_transition_table[EXAMPLE_NUM_STATES][EXAMPLE_NUM_STATES] = {
  /* states:     init           , run            , stop            */
  /* init    */ {NULL           , NULL           , NULL           }, 
  /* run     */ {NULL           , NULL           , example_disable}, 
  /* stop    */ {NULL           , NULL           , NULL           }, 
};

/*  ____  _        _       
 * / ___|| |_ __ _| |_ ___ 
 * \___ \| __/ _` | __/ _ \
 *  ___) | || (_| | ||  __/
 * |____/ \__\__,_|\__\___|
 *                         
 *   __                  _   _                 
 *  / _|_   _ _ __   ___| |_(_) ___  _ __  ___ 
 * | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 * |  _| |_| | | | | (__| |_| | (_) | | | \__ \
 * |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
 */                                             

// Function to be executed in state init
// valid return states: EXAMPLE_STATE_RUN
example_state_t example_do_init(example_state_data_t *data) {
  example_state_t next_state = EXAMPLE_STATE_RUN;
  
  syslog(LOG_INFO, "[FSM] In state init");
  data->n = 0;
  
  switch (next_state) {
    case EXAMPLE_STATE_RUN:
      break;
    default:
      syslog(LOG_WARNING, "[FSM] Cannot pass from init to %s, remaining in this state", example_state_names[next_state]);
      next_state = EXAMPLE_NO_CHANGE;
  }
  
  return next_state;
}


// Function to be executed in state run
// valid return states: EXAMPLE_NO_CHANGE, EXAMPLE_STATE_RUN, EXAMPLE_STATE_STOP
example_state_t example_do_run(example_state_data_t *data) {
  example_state_t next_state = EXAMPLE_NO_CHANGE;
  
  syslog(LOG_INFO, "[FSM] In state run");
  if (data->n > 10) {
    next_state = EXAMPLE_STATE_STOP;
  }

  printf("Running, data->n = %d\n", data->n);
  data->n++;
  
  switch (next_state) {
    case EXAMPLE_NO_CHANGE:
    case EXAMPLE_STATE_RUN:
    case EXAMPLE_STATE_STOP:
      break;
    default:
      syslog(LOG_WARNING, "[FSM] Cannot pass from run to %s, remaining in this state", example_state_names[next_state]);
      next_state = EXAMPLE_NO_CHANGE;
  }
  
  return next_state;
}


// Function to be executed in state stop
// valid return states: EXAMPLE_NO_CHANGE
example_state_t example_do_stop(example_state_data_t *data) {
  example_state_t next_state = EXAMPLE_NO_CHANGE;
  
  syslog(LOG_INFO, "[FSM] In state stop");
  // Nothing to do
  
  switch (next_state) {
    case EXAMPLE_NO_CHANGE:
      break;
    default:
      syslog(LOG_WARNING, "[FSM] Cannot pass from stop to %s, remaining in this state", example_state_names[next_state]);
      next_state = EXAMPLE_NO_CHANGE;
  }
  
  return next_state;
}


/*  _____                    _ _   _              
 * |_   _| __ __ _ _ __  ___(_) |_(_) ___  _ __   
 *   | || '__/ _` | '_ \/ __| | __| |/ _ \| '_ \
 *   | || | | (_| | | | \__ \ | |_| | (_) | | | | 
 *   |_||_|  \__,_|_| |_|___/_|\__|_|\___/|_| |_| 
 *                                                
 *   __                  _   _                 
 *  / _|_   _ _ __   ___| |_(_) ___  _ __  ___ 
 * | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 * |  _| |_| | | | | (__| |_| | (_) | | | \__ \
 * |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
 */    
                                         
// This function is called in 1 transition:
// 1. from run to stop
void example_disable(example_state_data_t *data) {
  syslog(LOG_INFO, "[FSM] State transition example_disable");
  printf("Disabling machine, data->n = %d\n", data->n);
}


/*  ____  _        _        
 * / ___|| |_ __ _| |_ ___  
 * \___ \| __/ _` | __/ _ \
 *  ___) | || (_| | ||  __/ 
 * |____/ \__\__,_|\__\___| 
 *                          
 *                                              
 *  _ __ ___   __ _ _ __   __ _  __ _  ___ _ __ 
 * | '_ ` _ \ / _` | '_ \ / _` |/ _` |/ _ \ '__|
 * | | | | | | (_| | | | | (_| | (_| |  __/ |   
 * |_| |_| |_|\__,_|_| |_|\__,_|\__, |\___|_|   
 *                              |___/           
 */

example_state_t example_run_state(example_state_t cur_state, example_state_data_t *data) {
  example_state_t new_state = example_state_table[cur_state](data);
  if (new_state == EXAMPLE_NO_CHANGE) new_state = cur_state;
  transition_func_t *transition = example_transition_table[cur_state][new_state];
  if (transition)
    transition(data);
  return new_state;
};

#ifdef TEST_MAIN
#include <unistd.h>
int main() {
  example_state_t cur_state = EXAMPLE_STATE_INIT;
  example_state_data_t data;
  data.n = 0;
  openlog("SM", LOG_PID | LOG_PERROR, LOG_USER);
  syslog(LOG_INFO, "Starting SM");
  do {
    cur_state = example_run_state(cur_state, &data);
    usleep(200000);
  } while (cur_state != EXAMPLE_STATE_STOP);
  example_run_state(cur_state, &data);
  return 0;
}
#endif
