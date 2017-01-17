/**
 * CS143a - Fall 2015
 * HW #7
 * @author: Michael Oh
 *
 * Simple implement of ls command
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <strings.h>


int sortByName(const void* str1, const void* str2)
{    
    return strcasecmp(str1,str2);
}


void printStat(char* pathname)
{
    struct stat sb;
    stat(pathname, &sb);

    printf(S_ISDIR(sb.st_mode)?"d":"-");
    printf((sb.st_mode & S_IRUSR)?"r":"-");
    printf((sb.st_mode & S_IWUSR)?"w":"-");
    printf((sb.st_mode & S_IXUSR)?"x":"-");
    printf((sb.st_mode & S_IRGRP)?"r":"-");
    printf((sb.st_mode & S_IWGRP)?"w":"-");
    printf((sb.st_mode & S_IXGRP)?"x":"-");
    printf((sb.st_mode & S_IROTH)?"r":"-");
    printf((sb.st_mode & S_IWOTH)?"w":"-");
    printf((sb.st_mode & S_IXOTH)?"x":"-");
     
    printf(" %i", sb.st_nlink);
    
    struct passwd* pwd = getpwuid(sb.st_uid);
    struct group* grp = getgrgid(sb.st_gid);
    printf(" %s", pwd->pw_name);
    printf(" %s", grp->gr_name);
    
    printf(" %-5i", sb.st_size);

    struct tm* time = localtime(&(sb.st_mtime));
    char stringTime[30];
    if (time->tm_year + 1900 < 2015)
    {
        strftime(stringTime, 30, " %b %e  %Y", time);
    }
    else
    {
        strftime(stringTime, 30, " %b %e  %R", time);
    }
    printf(stringTime);

    char str[200];
    strcpy(str, pathname);
    char* token = strtok(str, "/");
    char* filename;
    while (token != NULL)
    {
        filename = token;
        token = strtok(NULL, "/");
    }
    printf(" %s\n", filename);
}

void readDirectory(char* pathname)
{
    printf("%s:\n", pathname);
    DIR* dir = opendir(pathname);
    struct dirent* entry;
    int count = 0;
    char names[1000][500];
    
    while ((entry = readdir(dir)) != NULL)
    {
        if ((strcmp(entry->d_name,".")!=0)&&(strcmp(entry->d_name,"..")!=0))
        {
            char name[500];
            strcpy(name, pathname);
            strcat(name, "/");
            strcat(name, entry->d_name);
            strcpy(names[count++], name);
        }
    }

    closedir(dir);

    qsort(names, count, 500, sortByName);

    int i, total = 0;
    struct stat sb;
    for (i = 0; i < count; i++)
    {
        stat(names[i], &sb);
        if (S_ISREG(sb.st_mode))
        {
            total += sb.st_blocks;
        }
        else if (S_ISDIR(sb.st_mode))
        {
            total += 8;
        }
    }   
    printf("total %i\n", total/2);

    for (i = 0; i < count; i++)
    {
        printStat(names[i]);
    }
    printf("\n");
    for (i = 0; i < count; i++)
    {
        stat(names[i], &sb);
        if (S_ISDIR(sb.st_mode))
        {
            readDirectory(names[i]);
        }
    }
}



int main(int argc, char* argv[])
{   
    if (argc < 2)
    {
        readDirectory(".");
    }
    else
    {
        readDirectory(argv[1]);
    }
    return 0;
}

