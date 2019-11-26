#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

////////////////////////////////////////////////////////////////////////
//  function of i/o redirection using standard i/o
////////////////////////////////////////////////////////////////////////
void redirect(int sourcefd, char *sourcefile, int destfd, char *destfile, BOOLEAN append, BOOLEAN backgrnd)
{
	int flags, fd;

	
	// not exist opened source file descriptor and bg mode
	if (sourcefd == 0 && backgrnd) 
	{
		strcpy(sourcefile, "/dev/null");
		sourcefd = BADFD;
	}

	// //// case '<' //// //
	// exist opened source file descriptor
	if (sourcefd != 0) 
	{
		// close stdin
		if (close(0) == ERROR)
		{
			syserr("close");
		}

		if (sourcefd > 0) 
		{
			// override stdin
			if (dup(sourcefd) != 0)
			{
				fatal("dup");
			}
		}

		// open sourcefile
		else if (open(sourcefile, O_RDONLY, 0) == ERROR)
		{
			fprintf(stderr, "Cannot open %s\n", sourcefile);
			exit(0);
		}
	}
   
	// //// case '<' //// //
	// exist opened destination file descriptor
	if (destfd != 1) 
	{
		// close stdout
		if (close(1) == ERROR)
		{
			syserr("close");
		}

		// override stdout
		if (destfd > 1) 
		{
			if (dup(destfd) != 1)
			{
				fatal("dup");
			}
		}

		else
		{
			// file flag (will be used create file) - if not exist file -> create new file and set can write mode
			flags = O_WRONLY | O_CREAT;
			
			// when term == ">" -> clean file
			if (!append)
			{
				flags |= O_TRUNC;
			}

			// open file
			if (open(destfile, flags, 0666) == ERROR) 
			{
				fprintf(stderr, "Cannot create %s\n", destfile);
				exit(0);
			}

			// when term == ">>" find end of file
			if (append)	
			{
				if (lseek(1, 0L, 2) == ERROR) syserr("lseek");
			}
		}
	}

	// close file descriptor
	for (fd =3; fd < MAXFD; fd++)
	{
		close(fd);
	}

	return;
}
