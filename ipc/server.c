#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define SIZE 256

struct {
    long mtype;
    int data;
} message;

int main(int argc, char** argv)
{
    FILE *file;
    char *shmaddr;
    int msgid, shmid;
    key_t key;

    if(argc != 2)
        return 0;
    
    file = fopen(argv[1], "r");
    if(!file) {
        perror(argv[1]);
        exit(1);
    }
    key = ftok("./server", 0);
    if(key == -1) {
        perror("key");
        exit(1);
    }
    /* creating queue, shared memory, shm address */
    msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
    if(msgid == -1) {
        perror("msgget");
        exit(1);
    }
    shmid = shmget(key, SIZE, 0666 | IPC_CREAT | IPC_EXCL);
    if(shmid == -1) {
        perror("shmget");
        exit(1);
    }
    shmaddr = shmat(shmid, NULL, 0);

    while(!feof(file)) {
        if(fgets(shmaddr, SIZE, file)) {
            message.mtype = 2;
            message.data = 0;
            msgsnd(msgid, (struct msgbuf*)(&message), sizeof(int), 0);
            message.mtype = 4;
            message.data = 0;
            msgsnd(msgid, (struct msgbuf*)(&message), sizeof(int), 0);
            while(1)
                if(msgrcv(msgid, (struct msgbuf*)(&message), sizeof(int), 1, 0)) {
                    printf("spaces: %d,", message.data);
                    break;
                }
            while(1)
                if(msgrcv(msgid, (struct msgbuf*)(&message), sizeof(int), 3, 0)) {
                    printf(" numbers: %d\n", message.data);
                    break;
                }
        }
    }
    printf("exit...\n");
    message.mtype = 2;
    message.data = 1;
    msgsnd(msgid, (struct msgbuf*)(&message), sizeof(int), 0);
    message.mtype = 4;
    message.data = 1;
    msgsnd(msgid, (struct msgbuf*)(&message), sizeof(int), 0);
    sleep(1);   /* synchronization */
    msgctl(msgid, IPC_RMID, NULL);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    fclose(file);
    return 0;
}