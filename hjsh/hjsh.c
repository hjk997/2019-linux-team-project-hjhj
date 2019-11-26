#include <stdio.h>
#include <signal.h>
#include "common.h"

void s_handler(int signo);
void print_start();
////////////////////////////////////////////////////////////////////////
//  main
////////////////////////////////////////////////////////////////////////
int main()
{
	int pid, fd;
	SYMBOL term;
	
	//signal setting
	struct sigaction act;
	act.sa_handler = s_handler;
	sigfillset(&(act.sa_mask));
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTSTP, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	
	print_start();

	print_prompt();

	while (TRUE)
	{
		term = parse(&pid, FALSE, NULL);
		
		//not background state 
		if (term != S_AMP && pid != 0)
		{
			waitfor(pid);
		}

		//if user input just '\n'
		if (term == S_NL)
		{
			print_prompt();
		}

		//close file descriptor
		for (fd=3; fd<MAXFD; fd++)
		{
			close(fd);
		}
	}
}

////////////////////////////////////////////////////////////////////////
//  signal handler (Ctrl-C, Ctrl-Z, Ctrl-\ )
////////////////////////////////////////////////////////////////////////
void s_handler(int signo){
	printf("\nbye\n");
	exit(1);
}

////////////////////////////////////////////////////////////////////////
//  print program info first time
////////////////////////////////////////////////////////////////////////
void print_start(){
	printf("2019 System Programming Team Project\n");
	printf("/***********************************\n");
	printf("**20173116 shj, 20173237 hjk\n");
	printf("/***********************************\n");
	printf(": able command :\n");
	printf("1. ls \t 2. pwd \t 3. cd\n");
	printf("4. mkdir \t 5. rmdir \t 6. ln\n");
	printf("7. cp \t 8. rm \t 9. cat\n");
	printf("10. \& (background) \t 11. \>,\<,\>\> \t 12. \|\n");
	printf("13. Signal (Ctrl-C, Ctrl-Z, Ctrl-/ \t 14. exit\n");
	printf("\n");

}
