#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief Function to update mean and standard deviation given new sample.
 * 
 * @param x New sample value
 * @param n Number of samples including the new sample
 * @param mean In/Out pointer to current mean value
 * @param std_dev In/Out pointer to current standard deviation
 */
void run_statistics(double x, size_t n, double *mean, double *std_dev) {
    if (n <= 1) {
        // If there is one or fewer samples, set the mean to the sample value and std_dev to 0
        (*mean) = x;
        (*std_dev) = 0;
    } else {
        // Calculate the previous count of samples
        double n1 = n - 1;
        double n_div = 1.0 / n; // Precompute reciprocal of n for efficiency
        
        // Update mean:
        // New mean is derived from the old mean adjusted with the new sample value.
        // It computes the new mean by weighing the old mean by the previous number of samples (n-1)
        // and then adding the new sample. Finally, it divides the sum by the new sample count (n).
        double old_mean = *mean;
        (*mean) = (old_mean * n1 + x) * n_div; 
        
        // Update standard deviation:
        // Calculate n-2 which is used in the variance computation
        double n2 = n - 2;
        // Precompute reciprocal of n-1 for efficiency
        double n1_div = 1.0 / n1;
        // Adjustment factor for the new variance calculation
        double nn1 = n * n1_div;

        // New standard deviation calculation:
        // Uses the previous std_dev squared multiplied by n-2 to adjust for the new variance,
        // plus the factor that accounts for the deviation of the new mean from the new sample.
        // This is then scaled by the reciprocal of n and the square root is taken to update std_dev.
        (*std_dev) = sqrt(n_div * (n2 * pow(*std_dev, 2) + nn1 * pow(old_mean - x, 2)));
    }
}