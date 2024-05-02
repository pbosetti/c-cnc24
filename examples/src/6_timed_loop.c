#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> // Use posix thread
#include <time.h> // Get machine time
#include <unistd.h> // usleep()
// #include <math.h>

// Provide compiler a way to know where the statistics function is
extern void run_statistics(double x, size_t n, double *mean, double *std_dev);

// /**
//  * @brief Function to update mean and standard deviation given new sample.
//  * 
//  * @param x New sample value
//  * @param n Number of samples including the new sample
//  * @param mean In/Out pointer to current mean value
//  * @param std_dev In/Out pointer to current standard deviation
//  */
// void run_statistics(double x, size_t n, double *mean, double *std_dev) {
//     if (n <= 1) {
//         // If there is one or fewer samples, set the mean to the sample value and std_dev to 0
//         (*mean) = x;
//         (*std_dev) = 0;
//     } else {
//         // Calculate the previous count of samples
//         double n1 = n - 1;
//         double n_div = 1.0 / n; // Precompute reciprocal of n for efficiency
//        
//         // Update mean:
//         // New mean is derived from the old mean adjusted with the new sample value.
//         // It computes the new mean by weighing the old mean by the previous number of samples (n-1)
//         // and then adding the new sample. Finally, it divides the sum by the new sample count (n).
//         double old_mean = *mean;
//         (*mean) = (old_mean * n1 + x) * n_div; 
//        
//         // Update standard deviation:
//         // Calculate n-2 which is used in the variance computation
//         double n2 = n - 2;
//         // Precompute reciprocal of n-1 for efficiency
//         double n1_div = 1.0 / n1;
//         // Adjustment factor for the new variance calculation
//         double nn1 = n * n1_div;
//        
//         // New standard deviation calculation:
//         // Uses the previous std_dev squared multiplied by n-2 to adjust for the new variance,
//         // plus the factor that accounts for the deviation of the new mean from the new sample.
//         // This is then scaled by the reciprocal of n and the square root is taken to update std_dev.
//         (*std_dev) = sqrt(n_div * (n2 * pow(*std_dev, 2) + nn1 * pow(old_mean - x, 2)));
//     }
// }

// Sleep thead function
void* wait_thread(void* arg) {
    // First cast to correct type and then de-referentiate it
    int sleep_time = *((int*) arg);
    usleep(sleep_time);
    return NULL;
}

int main(int argc, const char* argv[]) {
    // Declarations
    int i = 0, sleep_time = 5000u; // [us]
    pthread_t pt1;
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
        // Create and launch the sleep thread
        pthread_create(&pt1, NULL, wait_thread, &sleep_time);

        // Perform my computations
        usleep(100);

        // Compute delta time
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t1 = ts.tv_sec + (ts.tv_nsec / 1.0E9);
        dt = t1 - t0;
        t0 = t1;

        // Compute the statistics
        run_statistics(dt, i, &mean, &std_dev);
         
        // Join the thread. Wait thread execution.
        pthread_join(pt1, NULL);
    }

    printf("Mean: %.3f [ms] Std dev: %.3f [ms]\n", (mean*1000.0), (std_dev*1000));

    return 0;
}