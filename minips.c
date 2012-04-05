/*
 * Written by Li Yao, April 5, 2012.
 */
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

/* define the output format */
#define FORMAT "%5s %s\t%8s %s\n"

int main(int argc, char** argv)
{
    /* used to iterate the directory /proc */
    DIR *dir;
    struct dirent *ent;

    int i, fd_self, fd;
    unsigned long time, stime;
    char flag, *tty;
    char cmd[256], tty_self[256], path[256], time_s[256];
    FILE* file;

    /* find out which tty executes this program */
    dir = opendir("/proc");
    fd_self = open("/proc/self/fd/0", O_RDONLY);
    sprintf(tty_self, "%s", ttyname(fd_self));

    printf(FORMAT, "PID", "TTY", "TIME", "CMD");

    /* iterate over the directory */
    while ((ent = readdir(dir)) != NULL)
    {
	flag = 1;
	for (i = 0; ent->d_name[i]; i++)
	    if (!isdigit(ent->d_name[i]))
	    {
		flag = 0;
		break;
	    }

	/* if the file presents a process */
	if (flag)
	{
	    /* find out the tty */
	    sprintf(path, "/proc/%s/fd/0", ent->d_name);
	    fd = open(path, O_RDONLY);
	    tty = ttyname(fd);

	    /* if the process has the same tty of this program */
	    if (tty && strcmp(tty, tty_self) == 0)
	    {
		/* find out the CMD */
		sprintf(path, "/proc/%s/stat", ent->d_name);
		file = fopen(path, "r");
		fscanf(file, "%d%s%c%c%c", &i, cmd, &flag, &flag, &flag);
		cmd[strlen(cmd) - 1] = '\0';

		/* find out the TIME */
		for (i = 0; i < 11; i++)
		    fscanf(file, "%lu", &time);
		fscanf(file, "%lu", &stime);
		time = (int)((double)(time + stime) / CLOCKS_PER_SEC);
		sprintf(time_s, "%02lu:%02lu:%02lu", 
		    (time / 3600) % 3600, (time / 60) % 60, time % 60);

		/* print the information */
		printf(FORMAT, ent->d_name, tty + 5, time_s, cmd + 1);

		fclose(file);
	    }

	    close(fd);
	}
    }

    close(fd_self);

    return 0;
}
