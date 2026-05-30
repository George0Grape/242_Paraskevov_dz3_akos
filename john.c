#include <stdio.h>
#include <math.h>

void john(int arg)
{
    double res = sqrt((double)arg);
    printf("john: you passed %d, sqrt is %.2f\n", arg, res);
}
