
#include <stdio.h>
#include <unistd.h>
#include "cpu.h"

void inline measured_function(volatile int *var)
{
	(*var) = 1;
}



int main()
{
    cpu_measure();
	return 0;
}



