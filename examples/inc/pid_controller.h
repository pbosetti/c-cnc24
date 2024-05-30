#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif

// C "Object" like style example

// Class like definition
typedef struct PID_controller PID_controller_t;

// Runtime variables defintion
typedef struct PID_rt_vars PID_rt_vars_t;

// PID gains object
typedef struct {
    float kp, ki, kd;
} PID_gains_t;

// // Runtime variables struct (public)
// struct PID_rt_vars {
//     float int_error;   // Store actual intergral error
//     float prev_error;  // Needed by derivative part
// };

// PID controller "class" definition
struct PID_controller {
    // Variables (public/private)
    PID_gains_t gains;
    // PID_rt_var_t vars; // Public variables
    PID_rt_vars_t* vars; // Private variables
    float sample_time; // Time between one PID update and one other
    float output;      // Computed output
    float set_point;   // Actual PID setpoint 
    // Operations/Methods
    float (*compute)(PID_controller_t* me, float input); // PID compute operation
    // Callbacks
    void (*onSetPointClbk)(PID_controller_t* me); // Triggere when set point is reached
};

// Constructor (for sure its public)
void PID_Initialize(PID_controller_t** pid, PID_gains_t* gains, float sample_time);

// Destructor
void PID_Destroy(PID_controller_t** pid);

// Public method
void PID_SetPoint(PID_controller_t* me, float set_point);

// Proved method that allows user to set callback
void PID_SetCallback(PID_controller_t* me, void (*fun)(PID_controller_t* me));

#ifdef __cplusplus
} // Extern "C" closing brackets
#endif

#endif /* __PID_CONTROLLER__ */