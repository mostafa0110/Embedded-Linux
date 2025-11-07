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
    shmctl(shmid, IPC_RMID, NULL);
    printf("\nWriter exiting and shared memory removed.\n");
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

    printf("Writer running. Type messages.\n");

    while (1) {
        printf("Enter message: ");
        fflush(stdout);
        if (fgets(shared_mem, SHM_SIZE, stdin) == NULL)
            break;
        shared_mem[strcspn(shared_mem, "\n")] = 0; // remove newline
    }

    cleanup(0);
    return 0;
}
