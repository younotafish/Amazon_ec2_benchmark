#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Reference: https://cseweb.ucsd.edu/classes/wi16/cse221-a/timing.html
static unsigned long inline rdtsc_begin(void) {
    unsigned lo, hi;
    __asm__ __volatile__("cpuid\n\t"
                         "rdtsc\n\t"
                         "mov %%edx, %0\n\t"
                         "mov %%eax, %1\n\t"
    : "=r" (hi), "=r" (lo)
    :: "%rax", "%rbx", "%rcx", "%rdx");
    return (((unsigned long)hi << 32) | lo);
}

// Reference: https://cseweb.ucsd.edu/classes/wi16/cse221-a/timing.html
static unsigned long inline rdtsc_end(void) {
    unsigned lo, hi;
    __asm__ __volatile__("rdtscp\n\t"
                         "mov %%edx, %0\n\t"
                         "mov %%eax, %1\n\t"
                         "cpuid\n\t"
    : "=r" (hi), "=r" (lo)
    :: "%rax", "%rbx", "%rcx", "%rdx");
    return (((unsigned long)hi << 32) | lo);
}

void cycle_per_second()
{
    unsigned int time = 100; // sleeping time is not exactly 100 seconds.
    unsigned long start = rdtsc_begin();
    sleep(time); // the longer it sleeps, the more accurate it is.
    unsigned long end = rdtsc_end();
    printf("CPU cycles per second: %lu\n", (end - start) / time);
}

void read_time(int N) {
    int stride = 34563349; // prime number
    char *A = (char*)malloc(1 << 28); // 256 MB
    unsigned long sum = 0;
    int a;
    int j = 0;
    for (int i = 0; i < N; i++) {
        unsigned long start = rdtsc_begin();
        a = A[j];
        unsigned long end = rdtsc_end();
        j += stride;
        j &= ((1 << 28) - 1);
        sum += (end - start);
    }
    printf("Reading %d bytes in %lu cycles\n", N, sum);
}

void write_time(int N) {
    int stride = 34563349; // prime number
    char *A = (char*)malloc(1 << 28); // 256 MB
    unsigned long sum = 0;
    int j = 0;
    for (int i = 0; i < N; i++) {
        unsigned long start = rdtsc_begin();
        A[j] = 0;
        unsigned long end = rdtsc_end();
        j += stride;
        j &= ((1 << 28) - 1);
        sum += (end - start);
    }
    printf("Writing %d bytes in %lu cycles\n", N, sum);
}

int main(int argc, char *argv[]) {
    int N;
    sscanf(argv[1], "%d", &N);
    read_time(N);
    write_time(N);
    return 0;
}
