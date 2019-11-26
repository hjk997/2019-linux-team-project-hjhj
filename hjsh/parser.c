#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

////////////////////////////////////////////////////////////////////////
//  parsing command
////////////////////////////////////////////////////////////////////////
SYMBOL parse(int *waitpid, BOOLEAN makepipe, int *pipefdp)
{
	SYMBOL symbol, term;
	int argc, sourcefd, destfd;
	int pid, pipefd[2];
	char *argv[MAXARG+1], sourcefile[MAXFNAME];
	char destfile[MAXFNAME];
	char word[MAXWORD];
	BOOLEAN append;

	argc = 0;			
	sourcefd = 0;		
	destfd = 1;			

	
	while (TRUE) 
	{
		// get word and analyze
		switch (symbol = getsymbol(word))		
		{
			// normal word
			case S_WORD :			
				// too many arguments		
				if(argc == MAXARG) 
				{
					fprintf(stderr, "Too many args.\n");
					break;
				}

				// assigned new memory 
				argv[argc] = (char *) malloc(strlen(word)+1);
				
				//if fail
				if(argv[argc] == NULL) 
				{
					fprintf(stderr, "Out of arg memory.\n");
					break;
				}

				// copy word
				strcpy(argv[argc], word);
				
				// input next word
				argc++;
				continue;

			// < 
			case S_LT   : 
			
				// if pipe mode
				if(makepipe) 
				{
					fprintf(stderr, "Extra <.\n");
					break;
				}

				// check Symbol Value of sourcefile
				if(getsymbol(sourcefile) != S_WORD) 
				{
					fprintf(stderr, "Illegal <.\n");
					break;
				}

				sourcefd = BADFD;
				continue;

			// > or >>
			case S_GT   :
			case S_GTGT :

				// if not define destination file
				if(destfd != 1) 
				{
					fprintf(stderr, "Extra > or >>.\n");
					break;
				}

				// check Symbol Value of destination file
				if(getsymbol(destfile) != S_WORD) 
				{
					fprintf(stderr, "Illegal > or >>.\n");
					break;
				}

				destfd = BADFD;
				//if gtgt(>>) -> append mode
				append = (symbol == S_GTGT);
				continue;

			// |, &, ;, new line(\n)
			case S_BAR  :
			case S_AMP  :
			case S_SEMI :
			case S_NL   :
			
				argv[argc] = NULL;
				// pipe mode |
				if(symbol == S_BAR) 
				{
					if(destfd != 1) 
					{
						fprintf(stderr, "> or >> conflicts with |.\n");
						break;
					}
					
					// input next word
					term = parse(waitpid, TRUE, &destfd);
				}
				
				// set final symbol
				else
				{
					term = symbol;
				}

				// if pipe mode
				if (makepipe) 
				{	
					//make new pipe
					if (pipe(pipefd) == ERROR)
					{
						syserr("pipe");
					}
					*pipefdp = pipefd[1];
					sourcefd = pipefd[0];
				}

				// execute command
				pid = execute(argc, argv, sourcefd, sourcefile,
								destfd, destfile,append, term == S_AMP);

				// not pipe mode -> set waiting pid
				if (symbol != S_BAR)
				{
					*waitpid = pid;
				}

				// no argument
				if (argc == 0 && (symbol != S_NL || sourcefd > 1))
				{
					fprintf(stderr, "Missing command.\n");
				}

				// release memory
				while (--argc >= 0)
				{
					free(argv[argc]);
				}

				return term;

			// invaild command
			case S_EOF : 
				exit(0);
		} 
	}    
}
