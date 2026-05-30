#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void sam(char *arg)
{
    double val = atof(arg);
    double res = sin(val);
    printf("sam: you passed %s, sin is %.2f\n", arg, res);
}
