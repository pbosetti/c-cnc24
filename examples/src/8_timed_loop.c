#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep()
#include <time.h>
#include <stdint.h>

// Provide compiler a way to know where the statistics function is
extern void run_statistics(double x, size_t n, double *mean, double *std_dev);

// Function that get system time as nanoseconds
uint64_t now_ns() {
    uint64_t now;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    now = ts.tv_nsec + (ts.tv_sec * 1.0e9);
    return now;
}

// Wait for interval time [ns]
void wait_next(uint64_t interval) {
    static uint64_t t0 = 0; // Function start time
    uint64_t elapsed_time = 0, delay = 0;

    // Initialize start time
    if(t0 == 0) { t0 = now_ns(); }

    // Time interval loop
    while(elapsed_time <= interval) {
        elapsed_time = now_ns() - t0;
    }

    // Calculate the delay to adjust for the time spent past the interval
    delay = elapsed_time - interval; // Calculate the actual time difference that exceeds the interval

    // Refresh t0 to the current time minus the delay, to compensate for the function's execution delay
    // This helps in keeping the start time of the next wait interval accurate
    t0 = now_ns() - delay;
}

int main(int argc, const char* argv[]) {
    // Declarations
    int i = 0, sleep_time = 5000u; // [us]
    // Time variables
    struct timespec ts;
    double t0 = 0, t1 = 0, dt = 0;
    // Statistics
    double mean = 0, std_dev = 0;

    // Lets get the machine time
    clock_gettime(CLOCK_MONOTONIC, &ts);
    t0 = ts.tv_sec + (ts.tv_nsec / 1.0e9); // Time as seconds

    // Application loop
    for(i=0; i<1000; i++) {
        // Compute delta time
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t1 = ts.tv_sec + (ts.tv_nsec / 1.0E9);
        dt = t1 - t0;
        t0 = t1;

        // Compute the statistics
        run_statistics(dt, i, &mean, &std_dev);

        // Wait function (CPU polling)
        wait_next(5.0e6); // Wait for some nanoseconds 
    }

    printf("Mean: %.3f [ms] Std dev: %.3f [ms]\n", (mean*1000.0), (std_dev*1000));

    return 0;
}