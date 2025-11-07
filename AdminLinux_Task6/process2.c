#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define KEY 1234

int shmid;
char *shared_mem;

void cleanup(int sig) {
    shmdt(shared_mem);
    printf("\nReader detached. Shared memory still exists.\n");
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    shmid = shmget(KEY, SHM_SIZE, IPC_CREAT);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    shared_mem = (char *)shmat(shmid, NULL, 0);
    if (shared_mem == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Reader running. Waiting for messages... \n");

    char last_msg[SHM_SIZE] = "";

    while (1) {
        if (strcmp(shared_mem, last_msg) != 0) {
            printf("New message: %s\n", shared_mem);
            strcpy(last_msg, shared_mem);
        }
        sleep(1);
    }

    cleanup(0);
    return 0;
}
