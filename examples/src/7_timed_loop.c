#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep()
#include <time.h>
#include <sys/time.h> // Sistem timer 
#include <signal.h> // get SIGALRM

// Provide compiler a way to know where the statistics function is
extern void run_statistics(double x, size_t n, double *mean, double *std_dev);

// Definition of signal callback
void signal_handler(int signal) {}

int main(int argc, const char* argv[]) {
    // Declarations
    int i = 0, sleep_time = 5000u; // [us]
    // Time variables
    struct timespec ts;
    struct itimerval itv; // System timer
    double t0 = 0, t1 = 0, dt = 0;
    // Statistics
    double mean = 0, std_dev = 0;

    // Lets catch the SIGALRM
    signal(SIGALRM, signal_handler);

    // Lets initialize the system timer
    itv.it_interval.tv_sec = 0u;
    itv.it_interval.tv_usec = 5000u; // 5 [ms]
    itv.it_value.tv_sec = 0u;
    itv.it_value.tv_usec = 5000u;

    // Set the timer
    setitimer(ITIMER_REAL, &itv, NULL);

    // Lets get the machine time
    clock_gettime(CLOCK_MONOTONIC, &ts);
    t0 = ts.tv_sec + (ts.tv_nsec / 1.0e9); // Time as seconds

    // Application loop
    for(i=0; i<1000; i++) {
        // Usleep function will end on SIGALRM catch
        usleep(10000000); 

        // Perform my computations
        usleep(100);

        // Compute delta time
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t1 = ts.tv_sec + (ts.tv_nsec / 1.0E9);
        dt = t1 - t0;
        t0 = t1;

        // Compute the statistics
        run_statistics(dt, i, &mean, &std_dev);
    }

    printf("Mean: %.3f [ms] Std dev: %.3f [ms]\n", (mean*1000.0), (std_dev*1000));

    return 0;
}