/**
 * CS143a - Fall 2015
 * HW #7
 * @author: Michael Oh
 * 
 * Simple implement of du command
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <strings.h>

char results[500][500];
int numPrint = 0;

void addToResult(char entry[])
{
    strcpy(results[numPrint++],entry);
}

int sortByNumber(const void* str1, const void* str2)
{
    char istr1[500], istr2[500];
    strcpy(istr1, str1);
    strcpy(istr2, str2);
    char* token1 = strtok(istr1, " ");
    char* token2 = strtok(istr2, " ");
    int int1, int2;
   // printf("what is it: %s\n", token1);
    int1 = atoi(token1);
    int2 = atoi(token2);   

    return int1 > int2;   

}


void printResults()
{
    int i;
    qsort(results, numPrint, 500, sortByNumber);
    
    for (i = 0; i < numPrint; i++)
    {
        printf("%s\n",results[i]);
    }
}

int processDiskUsage(char* pathname)
{
    DIR* dir = opendir(pathname);
    struct dirent* entry;
    int count = 0;
    char names[1000][500];
    
    while ((entry = readdir(dir)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") !=0)&&(strcmp(entry->d_name,"..")!=0))
        {
            char name[500];
            strcpy(name, pathname);
            strcat(name, "/");
            strcat(name, entry->d_name);
            strcpy(names[count++], name);
        }
    }
    closedir(dir);

    
    int i, total = 4;
    struct stat sb;

    for (i = 0; i < count; i++)
    {
        lstat(names[i], &sb);
        if (S_ISREG(sb.st_mode))
        {
            total += (sb.st_blocks*512)/1024;
        }
        else if (S_ISDIR(sb.st_mode))
        {
            total += processDiskUsage(names[i]);
        }
    }
    //stat(pathname, &sb);
    
    char buf[500];
    sprintf(buf,"%-10d", total);
    strcat(buf, pathname);
    addToResult(buf);

    return total;
}




int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        processDiskUsage(".");
    }
    else
    {
        processDiskUsage(argv[1]);
    }

    printResults();
    return 0;
}
