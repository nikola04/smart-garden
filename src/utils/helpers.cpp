#include "stdlib.h"
#include "math.h"

double fcalculate_mean(float* arr, int cnt) {
    if (cnt <= 0) return 0.0;

    double sum = 0.0;
    for (int i = 0; i < cnt; i++) {
        sum += arr[i];
    }
    return sum / cnt;
}

double fcalculate_std_dev(float* arr, int cnt, double mean) {
    double sum_sq_diff = 0;
    for (int i = 0; i < cnt; i++) {
        sum_sq_diff += (arr[i] - mean) * (arr[i] - mean);
    }
    return sqrt(sum_sq_diff / cnt);
}

float* filter_outliers(float* arr, int cnt, int* n){
    float* result = (float*) malloc(cnt * sizeof(float));
    *n = 0;

    if(cnt < 3) {
        for (int i = 0; i < cnt; i++) {
            result[i] = arr[i];
        }
        *n = cnt;
        return result;
    }

    double mean = fcalculate_mean(arr, cnt);
    double std_dev = fcalculate_std_dev(arr, cnt, mean);
    double threshold = 1.9;

    for(int i = 0; i < cnt; i++) {
        double z_score = fabs((arr[i] - mean) / std_dev);
        if (z_score <= threshold) {
            result[(*n)++] = arr[i];
        }
    }

    result = (float*) realloc(result, *n * sizeof(float));
    return result;
}
