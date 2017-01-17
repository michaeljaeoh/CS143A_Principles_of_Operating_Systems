/**
 * CS143a - Fall 2015
 * HW #1
 * #author: Michael Oh
 *
 * This program will read in a file of integer numbers from stdin until
 * end of file. It should print out the min, the max, and the average.
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    float line;
    float max = -10000000;
    float min = 10000000;
    float sum = 0;
    float average;
    int count = 0;

    // reads stdin line by line and computes max/min
    while (scanf("%f", &line) == 1)
    {
        count++;
	max = line>max?line:max;
	min = line<min?line:min;
	sum += line;
    }
    
    // Exit if there are no numbers in the file
    if (count == 0)
    {
	printf("No numbers entered!\n");
	exit(1);
    }
    average = sum / count;
    
    printf("max: %f\n", max);
    printf("min: %f\n", min);
    printf("average: %.2f\n", average);



    return 0;
}
