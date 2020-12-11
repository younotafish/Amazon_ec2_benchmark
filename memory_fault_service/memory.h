#ifndef MEMORY_H
#define MEMORY_H

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


void page_fault_service();
#endif
