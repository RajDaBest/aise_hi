#include <unistd.h>
#include <fcntl.h>

char x[64];

fgets(x, (size_t) 64, stdin);

int i = 0;

while(x [i] != '\n')
{
    i++;
}

x [i] = '\0';

atoi(x);