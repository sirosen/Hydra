#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <sys/prctl.h>

void sigchld_handler(int sig);
void sigint_handler(int sig);
void sigtstp_handler(int sig);
void sigterm_handler(int sig);

char **argv; 

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    int i;
    for (i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

pid_t new_head() {
    pid_t pid;
    char name[8];
    gen_random(name, 7);
    if((pid = fork()) == 0) {
        //New process group
        setpgid(0, 0);
        //Set process name and ps aux name to a
        //randomly generated string
        prctl(PR_SET_NAME, name);
        strcpy(argv[0], name);
    }
    return pid;
}

int main(int argc, char **argv1) {
    argv = argv1;
    srand (time(NULL));
    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT,  sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGTERM, sigterm_handler);
   
    pid_t pid;
    //Start the hydra with one head
    new_head();
    
    
    
    while (1) {
        pid = getpid();
        printf("I am the hydra!!!! (%d)\n", pid);
        sleep(1);
    }
}

void sigchld_handler(int sig)
{
    //Respawn chopped off or restrained heads
    int child_status; 
    pid_t pid; 
    while ((pid = waitpid(-1, &child_status, WNOHANG | WUNTRACED)) > 0) { 
        //Check if they were stopped by a signal
        if (WIFSTOPPED(child_status)) {
            printf("Somebody tried to restrain my head %d\n", pid);
            //KEEP GOING!!!!
            kill(pid, SIGCONT);
        }
        //Check if they were killed by a signal
        if (WIFSIGNALED(child_status)) {
            printf("Somebody chopped off my head!!!! %d\n", pid);
            //Create two new heads (only from the parent)
            //If we want we could spaw new heads from each of the children
            if (new_head() != 0) {
                new_head();
            }  
        }
        //Check if they exited cleanly 
        //(idk why this would happen, maybe Heracles)
        else if (WIFEXITED(child_status)) {
            printf("My head got sleepy and wandered off....%d\n", pid);
        } 
    } 
}


void sigint_handler(int sig)
{
    printf("You can't interupt a hydra!\n");
}

void sigtstp_handler(int sig)
{
    printf("You can't stop a hydra!\n");
}

void sigterm_handler(int sig)
{
    printf("You can't terminate a hydra!\n");
}

