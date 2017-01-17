/**
 * CS143a - Fall 2015
 * HW #7
 * @author: Michael Oh
 *
 *
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
    printf("%-5s", pwd->pw_name);
    printf("%-5s", grp->gr_name);
    
    printf("%-5i", sb.st_size);

    struct tm* time = localtime(&(sb.st_mtime));
    char buf[40];
    if (time->tm_year + 1900 == 2015)
    {
        strftime(buf, 40, "%b %d %Y", time);
    }
    else
    {
        strftime(buf, 40, "%b %d %R", time);
    }
    printf(buf);

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

    //qsort(names, count, sizeof(char)*500, sortByName);

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
    struct stat sb;
    
    if (argc < 2)
    {
        printf("Error - Usage: myls <pathname>\n");
        exit(1);
    }
    if (stat(argv[1], &sb) == -1)
    {
        perror("stat");
        exit(2);
    }

    if (S_ISREG(sb.st_mode))
    {
        printStat(argv[1]);
    }
    else if (S_ISDIR(sb.st_mode))
    {
        readDirectory(argv[1]);
    }


}

