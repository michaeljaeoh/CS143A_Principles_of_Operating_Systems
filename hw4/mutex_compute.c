/**
 * CS143a - Fall 2015
 * HW #4
 * @author: Michael Oh
 *
 *
 *
 * This program will compute the max, min, and average
 * of a number of integers and print the results. A mutex
 * will be used to ensure only one thread is reading stdin
 * and aggregating values at a given time.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t mutexsum;


struct shared_info{
    int max;
    int min;
    int sum;
    int count;
    int endOfInput;
};

void* myFunc(void* shared_data){
    int line;
    struct shared_info* my_data = (struct shared_info*) shared_data;
    while(!my_data->endOfInput){
        // Entering Critical Section
        pthread_mutex_lock(&mutexsum);
        if (scanf("%d",&line) <= 0){
            my_data->endOfInput = 1;
            pthread_mutex_unlock(&mutexsum);
            break;
        }
        my_data->max = my_data->max<line?line:my_data->max;
        my_data->min = my_data->min<line?my_data->min:line;
        my_data->sum += line;
        my_data->count++;
        // Leaving Critical Section
        pthread_mutex_unlock(&mutexsum);    
    }
    return my_data;
}

int main (int argc, char** argv){
    int i, rc;
    float average;
    struct shared_info shared_data;
    pthread_t threads[3];


    shared_data.max = -10000000;
    shared_data.min = 10000000;
    shared_data.sum = 0;
    shared_data.count = 0;
    shared_data.endOfInput = 0;

    pthread_mutex_init(&mutexsum, NULL);

    for (i = 0; i < 4; i++){
        if (i<3){
            rc = pthread_create(&threads[i], NULL, myFunc, (void*) &shared_data);
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
        else{
            myFunc((void*)&shared_data);
            for (i = 0; i < 3; i++){
                rc = pthread_join(threads[i], NULL);
                if (rc){
                    printf("ERROR; return code from pthread_join() is %d\n", rc);
                    exit(-1);
                }
            }
        }
    }
    
    average = shared_data.sum / shared_data.count;


    printf("max: %d\n", shared_data.max);
    printf("min: %d\n", shared_data.min);
    printf("average: %.2f\n", average);

    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
    return 0;
}


