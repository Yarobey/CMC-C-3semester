#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 256

struct {
    long mtype;
    char data;
} message;

int main(int argc, char** argv)
{
    char *shmaddr = NULL;
    int msgid, shmid, i, count;
    key_t key = ftok("./server", 0);;
    if(key == -1) {
        perror("key");
        exit(1);
    }
    /* connecting queue, shared memory, shm address */
    msgid = msgget(key, 0666);
    shmid = shmget(key, SIZE, 0666);
    shmaddr = shmat(shmid, NULL, 0);
    while(1) {
        count = 0;
        while(1)
            if(msgrcv(msgid, (struct msgbuf*)(&message), sizeof(int), 4, 0))
                break;
        if(message.data == 1)
            break;
        for(i = 0; shmaddr[i]; i++)
            if((shmaddr[i] >= '0') && (shmaddr[i] <= '9'))
                count++;
        message.mtype = 3;
        message.data = count;
        msgsnd(msgid, (struct msgbuf*)(&message), sizeof(int), 0);
    }
    shmdt(shmaddr);
    return 0;
}