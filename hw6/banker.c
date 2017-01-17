/**
 * CS143a - Fall 2015
 * HW #6
 * @authoer: Michael Oh
 *
 * Banker's Algorithm:
 * This program will prompt for the number of Processes(P), number of
 * Resources(R), the total available amount of each resource, the 
 * number of each resource currently used by each process, and the max number
 * of resources each process can access at a given time. Using this input,
 * the program will output whether the system is in a safe state or an unsafe state
 * and if it is in a safe state, it will also output the order of processes that
 * can access the resources to ensure there are no deadlocks.
 */
#include <stdio.h>
#include <stdlib.h>


void main()
{
    int P, R, i, j, safe, run, count = 0;    

    printf("Enter number of Processes: ");
    scanf("%d",&P);
    printf("\nEnter number of Resources: ");
    scanf("%d",&R);

    int available[R], current[P][R], max[P][R];
    int order[P], finished[P];
    for (i = 0; i < P; i++){
        finished[i] = 0;
    }
    
    printf("\nEnter max resource vector: ");
    for (i = 0; i < R; i++){
        scanf("%d", &available[i]);
    }
    
    printf("\nEnter current resource allocation table: ");
    for (i = 0; i < P; i++){
        for (j=0; j < R; j++){
            scanf("%d", &current[i][j]);
        }
    }

    // Remove currently used resources    
    for (i = 0 ; i< P; i++){
        for (j = 0; j< R; j++){
            available[j] -= current[i][j];
        }
    }

    printf("\nEnter maximum resource claim table: ");
    for (i = 0; i < P; i++){
        for (j = 0; j < R; j++){
            scanf("%d", &max[i][j]);
        }
    }
    while (count != P){
        safe = 0;
        for (i = 0 ; i < P; i++){
            if (!finished[i]){
                run = 1;
                // Check if there are enough available resources for current process to run
                for (j = 0 ; j < R; j++){
                    if (max[i][j] - current[i][j] > available[j]){   
                        run = 0;
                        break;
                    }
                }
                if (run){
                    // Process runs
                    order[count++] = i;
                    finished[i] = 1;
                    safe = 1;
                    // Process terminates and releases resources
                    for (j = 0; j < R; j++){
                        available[j] += current[i][j];
                    }
                    break;
                }
            }
        }
        if (!safe){
            printf("\n");
            printf("The system is in an unsafe state\n");
            break;
        }
    }
    if (safe){
        printf("\nSequence of process numbers for resource grants: ");
        for (i = 0 ; i < P; i++){
            printf("%d ", order[i]+1);
        }
        printf("\n");

        printf("The system is in a safe state\n");
    }





}

