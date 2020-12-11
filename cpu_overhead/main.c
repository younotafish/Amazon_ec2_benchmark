#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>

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

void measurement_overhead()
{
    int N = 1000000;
    unsigned long sum = 0;
    for(int i=0; i<N; i++)
    {
        unsigned long start = rdtsc_begin();
        // do nothing
        unsigned long end = rdtsc_end();
        sum += (end - start);
    }
    printf("Measurement overhead: %lu cycles\n", (sum / N));
}

void loop_overhead()
{
    int N = 1000000;
    unsigned long start = rdtsc_begin();
    for(int i = 0; i < N; i++) {
        // do nothing
    }
    unsigned long end = rdtsc_end();
    printf("Loop overhead: %f cycles per iteration\n", (double)(end - start) / (double)N);
}


// Reference: https://stackoverflow.com/questions/1474030/how-can-i-tell-gcc-not-to-inline-a-function
// Note: This noinline attribute is GCC only.
static __attribute__ ((noinline)) int func0() {return 0;}
static __attribute__ ((noinline)) int func1(int a1) {return 0;}
static __attribute__ ((noinline)) int func2(int a1, int a2) {return 0;}
static __attribute__ ((noinline)) int func3(int a1, int a2, int a3) {return 0;}
static __attribute__ ((noinline)) int func4(int a1, int a2, int a3, int a4) {return 0;}
static __attribute__ ((noinline)) int func5(int a1, int a2, int a3, int a4, int a5) {return 0;}
static __attribute__ ((noinline)) int func6(int a1, int a2, int a3, int a4, int a5, int a6) {return 0;}
static __attribute__ ((noinline)) int func7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) {return 0;}

#define PRE_CALL(func) \
start = rdtsc_begin(); \
for(int i=0; i<N; i++) \
{

#define POST_CALL(func) \
} \
end = rdtsc_end(); \
printf("Function call overhead for " #func ": %f cycles\n", (double)(end - start) /(double)N);

void procedure_call_overhead()
{
    int N = 1000000;
    unsigned long start;
    unsigned long end;

    PRE_CALL(func0);
    func0();
    POST_CALL(func0);

    PRE_CALL(func1);
    func1(0);
    POST_CALL(func1);

    PRE_CALL(func2);
    func2(0, 0);
    POST_CALL(func2);

    PRE_CALL(func3);
    func3(0, 0, 0);
    POST_CALL(func3);

    PRE_CALL(func4);
    func4(0, 0, 0, 0);
    POST_CALL(func4);

    PRE_CALL(func5);
    func5(0, 0, 0, 0, 0);
    POST_CALL(func5);

    PRE_CALL(func6);
    func6(0, 0, 0, 0, 0, 0);
    POST_CALL(func6);

    PRE_CALL(func7);
    func7(0, 0, 0, 0, 0, 0, 0);
    POST_CALL(func7);

}

void system_call_overhead()
{
    unsigned long start = rdtsc_begin();
    getpid();
    unsigned long end = rdtsc_end();
    printf("GetPID System call overhead: %lu cpu cycles\n", (end - start));
}

int main(int argc, char *argv[]) {
    cycle_per_second(); // takes 100 seconds for accuracy
    measurement_overhead();
    loop_overhead();
    procedure_call_overhead();
    system_call_overhead();
    return 0;
}
