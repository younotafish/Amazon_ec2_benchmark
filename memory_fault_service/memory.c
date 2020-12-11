#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <zconf.h>
#include "memory.h"


void page_fault_service() {
    double average_cycles = 0;
    int fd = open("../bigFile", O_RDWR | O_CREAT | O_TRUNC, 0600);
    lseek(fd, 268435456*4 - 1, SEEK_SET);
    write(fd, "", 1);
    close(fd);
    for (int repeat = 0; repeat < 10; repeat++) {
        int f = open("../bigFile", O_RDWR);
        if (f < 0) {
            printf("open file failed\n");
            exit(-1);
        }
        const int OFFSET = 1024 * 4;  //4 KB
        const int FILE_SIZE = 256 * 1024 * 1024; //256M
        const int times = FILE_SIZE / OFFSET;
        char* map =(char*) mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
        volatile char value = 0;
        unsigned long start = rdtsc_begin();
        for (int i = 0; i < times; i++) {
            value = map[i * OFFSET];
        }
        unsigned long end = rdtsc_end();
        unsigned long cycles = end - start;
        cycles /= times;
        average_cycles += cycles;
        munmap(map, FILE_SIZE);
    }
    average_cycles /= 10;
    printf("average cycles is %g\n", average_cycles);
}


