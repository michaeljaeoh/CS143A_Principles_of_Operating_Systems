/**
 * CS 143A - Fall 2015
 * HW #3
 * @author: Michael Oh
 *
 * This program will create a total of 4 processes that will
 * each print one letter of the alphabet from A to D, K times.
 * The default value of K is 10.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>






// Determines which letter the process should be printing K times
void printLetter(int pnum, int k)
{
            int j;
            if (pnum == 0)
            {
                for (j=0;j<k;j++)
                {
                    printf("A");
                    fflush(stdout);
                }
            }
            else if (pnum == 1)
            {
                for (j=0; j < k; j++)
                {
                    printf("B");
                    fflush(stdout);
                }
            }
            else if (pnum == 2)
            {
                for (j = 0; j < k; j++)
                {
                    printf("C");
                    fflush(stdout);
                }
            }
            else if (pnum == 3)
            {
                for (j = 0; j < k; j++)
                {
     
                    printf("D");
                    fflush(stdout);
                }
            }
    return;
}




int main(int argc, char ** args)
{
    int K = 10;
    char* str_k;
    
    if (argc > 1)
    {
        str_k = args[1];
        K = atoi(str_k);
    }

    pid_t pid, wpid;
    int i;
    int status = 0;
    
    

    for (i = 1; i <= 3; i++){
        if ((pid = fork()) == 0){
            if (pid == 0){
                printLetter(i, K);
                exit(0);
            }
        }
    }
    if (pid > 0){
        printLetter(0, K);
    }
    while ((wpid = wait(&status)) > 0);
    
    // To match output formatting
    printf("\n");
    

    return 0;
}






