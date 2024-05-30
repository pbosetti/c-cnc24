#include "pid_controller.h"

// PID sample time and 1DoF integration time
#define SAMPLE_TIME (0.01f) // [seconds]

// 1DoF model parameters
#define MASS    (20.0f) // [Kg]
#define K_STIFF (50.0f) // [N/m] 
#define C_DAMP  (10.0f) // [N*seconds/m]

// PID saturation value
#define PID_MAX_FORCE (10000.0f)  // [N]
#define PID_MIN_FORCE (-10000.0f) // [N]


// 1DoF Spring, mass and damper model
void SpringMassDamper_Model_Update(float input_force, float* position, float* velocity) {
    // Compute actual force
    float force = input_force - ((*position) * K_STIFF) - ((*velocity) * C_DAMP);
    // Retrieve acceleration
    float acceleration = force / MASS;

    // Integrate position and velocity (Euler approach)
    (*position) += (*velocity) * SAMPLE_TIME;
    (*velocity) += acceleration * SAMPLE_TIME;
}

void onSetPointCallback(PID_controller_t* me) {
    printf("Set point %.4f is reached!\n", me->set_point);
}


int main() {
    // Declarations
    PID_controller_t* PID_pt = NULL;
    PID_gains_t gains = {
        .kp = 100.0f,
        .ki = 40.0f,
        .kd = 20.0f,
    };

    // Initialize PID
    PID_Initialize(&PID_pt, &gains, SAMPLE_TIME);
    float set_point = 10.0f;
    PID_SetPoint(PID_pt, set_point);
    PID_SetCallback(PID_pt, onSetPointCallback);

    // Define intial model state
    float position = 5.0f;
    float velocity = 0.0f;
    float force    = 0.0f;

    // Print header
    printf("Step, position, velocity, force, set point\n");

    // PID for loop
    for (int i=0; i<3000; i++) {
        // Compute PID action
        force = PID_pt->compute(PID_pt, position);

        // PID output saturation
        if(force > PID_MAX_FORCE) {
            force = PID_MAX_FORCE;
        } else if (force < PID_MIN_FORCE) {
            force = PID_MIN_FORCE;
        }

        // Integrate model
        SpringMassDamper_Model_Update(force, &position, &velocity);
        printf("%i, %.4f, %.4f, %.4f, %.4f\n", i, position, velocity, force, set_point);
    }

    return 0;
}