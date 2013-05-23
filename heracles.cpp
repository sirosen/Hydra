#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

int main() {
    printf("                  /|\n"
"   _______________)|..\n"
" <'______________<(,_|)\n"
"            .((()))| ))        << BY THE POWER OF ZEUS!!! >>\n"
"            (======)| \\\n"
"           ((( \"_\"()|_ \\\n"
"          '()))(_)/_/ ' )\n"
"          .--/_\\ /(  /./\n"
"         /'._.--\\ .-(_/\n"
"        / / )\\___:___)\n"
"       ( -.'.._  |  /\n"
"        \\  \\_\\ ( | )\n"
"         '. /\\)_(_)|\n"
"           '-|  XX |\n"
"            %%%%%%%%%%%%%%%%\n"
"           / %%%%%%%%%%%%%%\\\n"
"          ( /.-'%%%%%%. \\\n"
"         /(.'   %%%%\\ :|\n"
"        / ,|    %%  ) )\n"
"      _|___)   %%  (__|_\n"
"      )___/       )___(\n"
"       |x/      mrf\\ >\n"
"       |x)         / '.\n"
"       |x\\       _(____''.__\n"
"     --\\ -\\--\n"
"      --\\__|--");
    int shmid = shmget(1337, sizeof(int), 0666);
    char *address = ((char *) shmat(shmid, NULL, 0));
    if(address != NULL) {
        strcpy(address + 4, "By the power of Zeus!");
    }
}
