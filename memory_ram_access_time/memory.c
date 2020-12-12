//
// Created by Push on 11/19/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "memory.h"

int REPEAT_TIMES = 10;
int ACCESS_TIMES = 1000;

double measure_ram_with_parameters(int array_size, int stride)
{
    double average_cycles = 0;

    for (int repeat = 0; repeat < REPEAT_TIMES; repeat++)
    {
        int *array = (int *)malloc((size_t)array_size * sizeof(int));
        for (int i = 0; i < array_size; i++)
        {
            array[i] = (i + stride) % array_size;
        }
        unsigned long start = rdtsc_begin();
        int index = 0;
        for (int i = 0; i < ACCESS_TIMES; i++)
        {
            index = array[index];
        }
        unsigned long end = rdtsc_end();
        average_cycles += (double)(end - start) / ACCESS_TIMES;
        free(array);
    }
    average_cycles /= REPEAT_TIMES;
    printf("access memory for %d array and %d stride cost %g cycles in average\n", array_size, stride, average_cycles);
    return average_cycles;
}

void measure_ram(int array_size_len, int stride_len, FILE *f)
{
    int array_sizes[array_size_len]; // 2K to 512M
    array_sizes[0] = 512;
    for (int i = 1; i < array_size_len; i++)
    {
        array_sizes[i] = array_sizes[i - 1] * 2;
    }
    int strides[stride_len];
    strides[0] = 1;
    for (int i = 1; i < stride_len; i++)
    {
        strides[i] = strides[i - 1] * 4;
    }
    for (int i = 0; i < array_size_len; i++)
    {
        for (int j = 0; j < stride_len; j++)
        {
            int array_size = array_sizes[i];
            int stride = strides[j];
            double cycles = measure_ram_with_parameters(array_size, stride);
            fprintf(f, "%d, %d, %g\n", array_size * 4, stride * 4, cycles);
        }
    }
}

#define REPEAT4(F) F F F F
#define REPEAT16(F) \
    REPEAT4(F)      \
    REPEAT4(F)      \
    REPEAT4(F)      \
    REPEAT4(F)
#define REPEAT64(F) \
    REPEAT16(F)     \
    REPEAT16(F)     \
    REPEAT16(F)     \
    REPEAT16(F)
#define REPEAT256(F) \
    REPEAT64(F)      \
    REPEAT64(F)      \
    REPEAT64(F)      \
    REPEAT64(F)
#define REPEAT1024(F) \
    REPEAT256(F)      \
    REPEAT256(F)      \
    REPEAT256(F)      \
    REPEAT256(F)
#define REPEAT4096(F) \
    REPEAT1024(F)     \
    REPEAT1024(F)     \
    REPEAT1024(F)     \
    REPEAT1024(F)
#define REPEAT16384(F) \
    REPEAT4096(F)      \
    REPEAT4096(F)      \
    REPEAT4096(F)      \
    REPEAT4096(F)

void memory_measure()
{
    FILE *f = fopen("record.csv", "w");
    measure_ram(19, 9, f);
}
