#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "common.h"

////////////////////////////////////////////////////////////////////////
//  print fatal error
////////////////////////////////////////////////////////////////////////
void fatal(char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}

////////////////////////////////////////////////////////////////////////
//  print normal error
////////////////////////////////////////////////////////////////////////
void syserr(char *message)
{
	fprintf(stderr, "Error: %s (%d", message, errno);
	exit(1);
}

////////////////////////////////////////////////////////////////////////
//  // wait while child process die
////////////////////////////////////////////////////////////////////////
void waitfor(int pid)
{
	int wpid, status;

	while ((wpid = wait(&status)) != pid && wpid != ERROR);
}


////////////////////////////////////////////////////////////////////////
//  print shell prompt
////////////////////////////////////////////////////////////////////////
void print_prompt()
{
	char buf[1024];
	char nbuf[1024];
	char* home;
	int i, ret, len, buflen, homelen;

	//get current path
	getcwd(buf, 1024);
	//get home path
	home = getenv("HOME");
	
	// change $HOME directory to "~/" 
	if ( (ret = strstr(buf, getenv("HOME"))) != NULL)
		strcpy(nbuf, "\~\/");

	homelen = strlen(home)-1;
	buflen = strlen(buf);
	len = buflen-homelen;
	i = strlen(nbuf)-1;
	
	//make directory str to print
	while( i < len ){
		nbuf[i] = buf[homelen+i];
		i++;
	}

	// add null in end of nbuf
	nbuf[i] = '\0';

	printf(GREEN);
	//print user name
	printf(getenv("USER"));
	printf(RESET);
	printf(" : ");
	printf(BLUE);
	//print directory
	printf(nbuf);
	printf(RESET);
	printf("\$\>  ");
}

////////////////////////////////////////////////////////////////////////
//  execute command
////////////////////////////////////////////////////////////////////////
BOOLEAN shellcmd(int ac, char *av[], int sourcefd, int destfd)
{
	char *path;

	// cd
	if(!strcmp(av[0], "cd"))
	{
		cmd_cd(ac, av);
	}
	// ls
	else if(!strcmp(av[0], "ls"))
	{
		cmd_ls(ac, av);
	}
	// cp
	else if(!strcmp(av[0], "cp"))
	{
		cmd_cp(ac, av);
	}
	// rm
	else if(!strcmp(av[0], "rm"))
	{
		cmd_rm(ac, av);
	}
	// mv
	else if(!strcmp(av[0], "mv"))
	{
		cmd_mv(ac, av);
	}
	// mkdir
	else if(!strcmp(av[0], "mkdir"))
	{
		cmd_mkdir(ac, av);
	}
	// rmdir
	else if(!strcmp(av[0], "rmdir"))
	{
		cmd_rmdir(ac, av);
	}
	// cat
	else if(!strcmp(av[0], "cat"))
	{
		cmd_cat(ac, av);
	}
	// wc
	else if(!strcmp(av[0], "wc"))
	{
		cmd_wc(ac, av);
	}
	// ln
	else if(!strcmp(av[0], "ln"))
	{
		cmd_ln(ac, av);
	}
	// exit
	else if(!strcmp(av[0], "exit"))
	{
		cmd_exit();
	}
	else
	{
		return FALSE;
	}

	if (sourcefd != 0 || destfd != 1)
	{
		return FALSE;
	}

	return TRUE;
}
