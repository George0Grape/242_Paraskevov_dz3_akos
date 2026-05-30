#include <stdio.h>

void fred(int arg)
{
    printf("INTERPOSE (Link-time): fred is called with %d\n", arg);
}

void john(int arg)
{
    printf("INTERPOSE (Link-time): john is called with %d\n", arg);
}
