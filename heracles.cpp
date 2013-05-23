#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

int main() {
    printf("No worries citizen. I will save you!\n");
    int shmid = shmget(1337, sizeof(int), 0666);
    char *address = ((char *) shmat(shmid, NULL, 0));
    if(address != NULL) {
        strcpy(address + 4, "By the power of Zeus!");
    }
}
