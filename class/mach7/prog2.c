/* prog2.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    char buf[255];
    int i;
    long count = 3;
    if(argc == 2)
        count = strtol(argv[1], NULL, 10);
    while((count != 0) && (fgets(buf, sizeof(buf), stdin))) {
        for(i = 0; i < sizeof(buf) && buf[i]; i++)
            if(buf[i] == '\n'){
                count--;
                break;
            }
        fprintf(stdout, "%s", buf);
    }
    return 0;
}