#include "cpu.h"
#include <stdlib.h>
#include <pthread.h>

void cpu_measure()
{

    processCreate_overhead();
    threadCreate_overhead();
    processSwitch_overhead();
    threadSwitch_overhead();

}

void processCreate_overhead()
{
    unsigned long start, end;
    unsigned long sum = 0;
    start = rdtsc_begin();
    for(int i=0; i<10000; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
            exit(0);
        else
        {
            wait(NULL);   //recall child process
        }
    }
    end = rdtsc_end();
    printf("The average overhead for fork() is %f cpu cycles\n", (double)(end - start) / 10000.0);
}

void* runThread(void* c)
{
    pthread_exit(NULL);
}

void threadCreate_overhead()
{
    pthread_t td;
    unsigned long start, end;
    start = rdtsc_begin();
    for(int i=0; i<10000; i++)
    {
        pthread_create(&td, NULL, runThread, NULL);
        pthread_join(td, NULL);   //force switch
    }
    end = rdtsc_end();
    printf("The average overhead for pthread_create() is %f cpu cycles\n", (double)(end - start) / 10000.0);
}


unsigned long processSwithchTime(int *fd)
{
    unsigned long start, end;
    pid_t  pid;
    unsigned long res = 0;
    if((pid = fork()) != 0)
    {
        start = rdtsc_begin();
        wait(NULL);
        read(fd[0], (void*)&end, sizeof(unsigned long));
    }
    else
    {
        end = rdtsc_end();
        write(fd[1], (void*)&end, sizeof(unsigned long));
        exit(0);
    }
    if(end > start)
        res += (end - start);
    return res;
}

void processSwitch_overhead()
{
    int fd[2];
    pipe(fd);
    unsigned long sum = 0;
    for(int i=0; i<10000; i++)
    {
        unsigned long res = processSwithchTime(fd);   //res is the time pass
        if(res > 0)
            sum += res;
    }
    printf("The average overhead for process context switch is %f cpu cycles\n", (double)(sum) / 10000.0);
}



static inline void *thread_run(void * a) {
    unsigned long t = rdtsc_end();

    write(fd[1], (void*)&t, sizeof(unsigned long));
    pthread_exit(NULL);
}

unsigned long threadSwitchTime()
{
    unsigned long start, end;
    pthread_t pt;
    pipe(fd);
    pthread_create(&pt, NULL, thread_run, NULL);
    start = rdtsc_begin();
    pthread_join(pt, NULL);
    read(fd[0], (void*)&end, sizeof(unsigned long));
    return end - start;
}

void threadSwitch_overhead()
{
    unsigned long sum = 0;
    for(int i=0; i<10000; i++)
    {
        unsigned long res = threadSwitchTime();
        if(res > 0)
            sum += res;
    }
    printf("The average overhead for thread context switch is %f cpu cycles\n", (double)(sum) / 10000.0);
}

