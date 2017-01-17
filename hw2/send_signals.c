/**
 * CS143a - Fall 2015
 * HW #2
 * @author: Michael Oh
 *
 *
 * This program waits in an infinite loop and waits
 * for a SIGINT or SIGUSR1 signal. When receiving a
 * SIGUSR1 signal, the receiving process will return
 * a SIGUSR2 signal to the sending process. When a
 * SIGINT signal is received, the number of SIGUSR1
 * signals received will be printed and the program
 * will exit.
 */
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct sigaction act;
int count = 0;

void sighandler(int signum, siginfo_t *info, void *ptr)
{
    if (signum == SIGUSR1)
    {
        count++; 
      
        // Sends SIGUSR2 signal to the process that sent SIGUSR1 signal
        kill(info->si_pid, SIGUSR2);
    }
    else if (signum == SIGINT)
    {   
        printf("Signals: %d\n", count);
        exit(0);
    }
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
    while(1)
    {
        sigaction(SIGUSR1, &act, NULL);
        sigaction(SIGINT, &act, NULL);
        sleep(1);
    }

    return 0;
}
