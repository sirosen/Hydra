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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include <vector>

void sigchld_handler(int sig);
void sigint_handler(int sig);
void sigtstp_handler(int sig);
void sigterm_handler(int sig);

char **argv; 
int *rage;
char *weakness;

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
    *rage = *rage + 1;
    if((pid = fork()) == 0) {
        //Reseed the random number generator
        srand(getpid());
        //New process group
        setpgid(0, 0);
        //Set process name and command name to a
        //randomly generated string (prevent killall
        //or greping ps aux)
        prctl(PR_SET_NAME, name);
        strcpy(argv[0], name);
    }
    return pid;
}

//A day in the life of a hydra
void hydra_day_to_day() {
    //Yeah I'm using a vector here... sue me
    std::vector<pid_t> pids;
    pid_t pid;
    
    //Let them know who we are
    printf("I am the hydra!!!! (%d) I'm at a rage of %d\n", getpid(), *rage);
    
    //Find a victim to eat
    DIR* proc = opendir("/proc");
    struct dirent* process;
    //Read through /proc and pull out any number looking things
    //these are probably pids
    while ((process = readdir(proc)) != NULL) {
        if ((pid = atoi(process->d_name)) > 0) {
              pids.push_back (pid);
        }
    }
    int target = rand() % pids.size();
    printf("I will eat %d\n", pids[target]);
    
    //EAT THEM!!!!
    //kill(pids[target], SIGKILL);
    
    //Take a rest (more rage -> shorter rests)
    sleep(10 - *rage);
}

void check_weakness() {
    if (!strcmp((char *) (rage+1), "By the power of Zeus!")) {
        exit(0);
    }
}
int main(int argc, char **argv1) {
    argv = argv1;
    srand (time(NULL));
    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT,  sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGTERM, sigterm_handler);

    //Set up shared memory to store rage
    //(A hydra never forgets the damage done to it's ancestors)
    int shmid = shmget(1337, sizeof(int), IPC_CREAT | 0666);
    rage = (int *) shmat(shmid, NULL, 0);
    *rage = -2;
    *(rage+1) = '\0';

    //Start the hydra with two heads
    if (new_head() != 0) {
        new_head();
    }
    
    while (1) {
        hydra_day_to_day();
        check_weakness();
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
            //Create a new head which will act as a parent to two new
            //children (see following diagram)
            /*
            Original hydra:
                 2
            1--<
                 2
            After chopping off the lower head and respawining
            
                   2
            1 -- <        3
                   2 -- < 
                          3
            The top level will react to the killing of any second level. In
            turn, the second level will react to the killing of any third level,
            etc...
            */
            
            if (new_head() == 0) {
                if(new_head() != 0) {
                    new_head();
                }
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



