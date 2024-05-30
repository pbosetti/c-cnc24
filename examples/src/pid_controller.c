#include "pid_controller.h"

#define ERROR_THRESHOLD (0.001f) // 1 mm

// Runtime variables struct (private)
struct PID_rt_vars {
    float int_error;   // Store actual intergral error
    float prev_error;  // Needed by derivative part
};

// Here if I want to declare later that PID_Initialize
// static void PID_Compute(PID_controller_t* me, float input); 
float PID_Compute(PID_controller_t* me, float input) {
    if(me == NULL || me->vars == NULL) {
        return 0.0f;
    }

    // Compute the errors
    float error = me->set_point - input; 
    me->vars->int_error += error * me->sample_time; // Intergal 
    float error_d = (error - me->vars->prev_error) / me->sample_time; // Derivative

    // Compute PID actions
    float p_term = error * me->gains.kp;
    float i_term = me->vars->int_error * me->gains.ki;
    float d_term = error_d * me->gains.kd;

    // Compute output
    me->output = p_term + i_term + d_term;

    // Refresh runtime variables
    me->vars->prev_error = error;

    // Trigger callback execution
    if(me->onSetPointClbk != NULL) {
        if(error <= ERROR_THRESHOLD) {
            // Attention! This will be triggered every time error 
            // is less than threshold and PID compute is called
            me->onSetPointClbk(me);
        }
    }

    return me->output;
}

// Constructor (for sure its public)
void PID_Initialize(PID_controller_t** pid, PID_gains_t* gains, float sample_time) {
    // Allocat PID "class"
    (*pid) = (PID_controller_t*)malloc(sizeof(PID_controller_t));
    if(*pid == NULL) {
        // Raise allocation error
        return;
    }

    // If PID is allocated then I can set "class" variables and methods
    if(gains == NULL) {
        // Set default PID gains
        (*pid)->gains.kp = 10.0f;
        (*pid)->gains.ki = 0.0f;
        (*pid)->gains.kd = 1.0f;
    } else {
        // Copy gains conntent
        (*pid)->gains.kp = gains->kp;
        (*pid)->gains.ki = gains->ki;
        (*pid)->gains.kd = gains->kd;
    }

    (*pid)->sample_time = sample_time;

    // Set defualt output and set_point
    (*pid)->set_point = 0.0f;
    (*pid)->output    = 0.0f;

    // Initialize runtime variables
    (*pid)->vars = (PID_rt_vars_t*)malloc(sizeof(PID_rt_vars_t));
    if((*pid)->vars == NULL) {
        // Allocation error
        return;
    }
    // Set runtime variables
    (*pid)->vars->prev_error = 0.0f;
    (*pid)->vars->int_error  = 0.0f;

    // Set operations/methods
    (*pid)->compute = PID_Compute;

    // Set default callback
    (*pid)->onSetPointClbk = NULL;
}

void PID_Destroy(PID_controller_t** pid) {
    if((*pid) != NULL) {
        if((*pid)->vars != NULL) {
            free((*pid)->vars);  // Free the memory
            (*pid)->vars = NULL; // Set NULL pointer
        }
        free((*pid));
        (*pid) = NULL;
    }
}

void PID_SetPoint(PID_controller_t* me, float set_point) {
    if(me != NULL) {
        me->set_point = set_point;
    }
}

void PID_SetCallback(PID_controller_t* me, void (*fun)(PID_controller_t* me)) {
    if(me != NULL) {
        me->onSetPointClbk = fun;
    }
}


