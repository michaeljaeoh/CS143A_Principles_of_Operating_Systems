/**
 * CS143a - Fall 2015
 * HW #4
 * @author: Michael Oh
 *
 * This program will read in an input file of integers and will
 * calculate the max, min, and average. The work for calculating
 * these values will be divided up amongst 4 threads, including
 * main. The results will then be printed.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// Holds information about the integer array segment that
// the thread is in charge of handling.
struct thread_info{
    int index;
    int max;
    int min;
    int sum;
    int* nums;
};

// Thread Start Routine
// Calculates the max, min, and sum of the array segment
void* myFunc(void* thread_data){
    struct thread_info* my_data = (struct thread_info*) thread_data;
    int i;
    for (i = 0; i < my_data->index; i++){
        my_data->max = my_data->max<my_data->nums[i]?my_data->nums[i]:my_data->max;
        my_data->min = my_data->min<my_data->nums[i]?my_data->min:my_data->nums[i];
        my_data->sum += my_data->nums[i];
    }
    return my_data;    
}    

int main (){
    int i, line, rc;
    int count = 0;
    int num[1000];
    int max = -10000000;
    int min = 10000000;
    int sum = 0;
    float average;
    pthread_t threads[3];

//    char* filename = argv[1];
//    FILE* fp = fopen(filename,"r");
    
//    if (fp == NULL){printf("Failed to open file!\n");exit(1);}
    while (scanf("%d", &line)==1){
        num[count++] = line;
    }
    
    if (count == 0){printf("No numbers entered!\n");exit(1);}
    
    // Initialize thread_data integer values
    struct thread_info thread_data[4];
    for (i = 0; i<4; i++){
        thread_data[i].min = 10000000;
        thread_data[i].max = -10000000;
        thread_data[i].sum = 0;
    }
    
    // Calculates how to divide integer array into segments.
    int divide = count/4;
    int leftover = count%4;
    
    // Determines segment of integer array for each thread to iterate through
    // and the length of the segment.
    for (i = 0; i < 4; i++){
        thread_data[i].index = i<3?divide:(divide+leftover);
        thread_data[i].nums = num + (i*divide); // pointer arithmetic
    }
    
    
    for (i = 0; i<4; i++){
        if (i<3){        
            rc = pthread_create(&threads[i], NULL, myFunc, (void*)&thread_data[i]);
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
        else{
            myFunc((void*)&thread_data[i]);
            for (i = 0; i < 3; i++){
                rc = pthread_join(threads[i], NULL);
                if (rc){
                    printf("ERROR; return code from pthread_join() is %d\n",rc);
                    exit(-1);
                }
            }
        }
    }  
    
    for (i = 0; i < 4; i++){
        max = thread_data[i].max<max?max:thread_data[i].max;
        min = thread_data[i].min<min?thread_data[i].min:min;
        sum+= thread_data[i].sum;
    }
    
    average = sum / count;
    
    printf("max: %d\n", max);
    printf("min: %d\n", min);
    printf("average: %.2f\n", average);


    pthread_exit(NULL);
    return 0;
}

