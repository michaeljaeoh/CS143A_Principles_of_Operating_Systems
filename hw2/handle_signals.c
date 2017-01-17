/**
 * CS143a - Fall 2015
 * HW #2
 * @author: Michael Oh
 *
 * This program waits in an infinite loop and gives 
 * feedback when certain signals(interrupt,quit,
 * terminal stop) are received. After the third 
 * terminal stop signal, a summary of the number of
 * signals received will be printed.
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int count_c = 0;
int count_q = 0;
int count_z = 0;
struct sigaction act;


// Determines what actions to take upon receiving a signal
void sighandler(int signum, siginfo_t *info, void *ptr)
{
    if (signum == SIGINT)
    {
        count_c++;
        printf("I");
    }
    else if (signum == SIGQUIT)
    {
        count_q++;
        printf("Q");
    }
    else if (signum == SIGTSTP)
    {
        count_z++;
        printf("S");
        if (count_z == 3)
        {
            printf("\nInterrupt: %d\n",count_c);
            printf("Stop: %d\n",count_z);
            printf("Quit: %d\n",count_q);
            exit(0);
        }
    }
    fflush(stdout);
}

void init_signal_handlers()
{    
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sighandler;
    act.sa_flags = SA_SIGINFO;
}

int main()
{
    init_signal_handlers();
    while (1)
    {
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGQUIT, &act, NULL);
        sigaction(SIGTSTP, &act, NULL);
        sleep(1);
    }
    return 0;
}
