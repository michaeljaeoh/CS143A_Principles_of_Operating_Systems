/**
 * CS143A - Fall 2015
 * HW #3
 * @author: Michael Oh
 *
 *
 * This is a simple command shell that will be able to
 * interpret commands that are input by the user. The
 * commands will then be executed just as if it were
 * called from any command line shell, such as Bash.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>







void parseAndExecute(char* input){
    int pos = 0;
    const char* delim = " ";
    char* in = NULL;
    char* commands[300];
    in = strtok(input,delim);
    while (in != '\0'){
        commands[pos++] = in;
        in = strtok(NULL, delim);
    }
    
 //   int i;
    commands[pos] = NULL;    

    pid_t pid;
    int status;

    pid = fork();
    
    if (pid < 0){
        printf("Error: Process was unable to fork.\n");
        exit(1);
    }

    else if (pid == 0){
        if (execvp(commands[0], commands) < 0){
            char pcmd[300];
            int i;
            for (i = 0 ; i < pos; i++){strcat(pcmd,commands[i]);strcat(pcmd," ");}
            printf("Error: Execution of \' %s\'has failed.\n",pcmd);
            exit(1);
        }
    }
    else{
        while (wait(&status) != pid);
    }

}


int main()
{
     int i;
     char input[300];
     setbuf(stdout,NULL);
     while(1){

     printf("$ ");
    
     fgets(input, sizeof(input), stdin);

     if (feof(stdin)){exit(0);}
     i = strlen(input)-1;
     if (input[i] == '\n'){
         input[i] = '\0';
     }
     parseAndExecute(input);
     }
     return 0;
}









