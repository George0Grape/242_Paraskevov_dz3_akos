#include <stdio.h>

void bill(char *arg)
{
    printf("INTERPOSE (Load-time): bill is called with %s\n", arg);
}

void sam(char *arg)
{
    printf("INTERPOSE (Load-time): sam is called with %s\n", arg);
}
