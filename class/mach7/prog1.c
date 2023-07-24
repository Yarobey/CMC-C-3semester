/* prog1.c */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd = 0;
    char c;
    if(argc == 2) {
        fd = open(argv[1], O_RDONLY);
        if(fd == -1) {
            perror(argv[1]);
            return 1;
        }
    }
    while(read(fd, &c, sizeof(char)) == 1)
        write(1, &c, sizeof(char));
    close(fd);
    return 0;
}