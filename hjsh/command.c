#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"

////////////////////////////////////////////////////////////////////////
//  check whether argument exists or not
////////////////////////////////////////////////////////////////////////
int check_arg(char *av[], const char *opt)
{
	int count = 0;

	// check all argument.
	while(*av != '\0')
	{
		// is exist opt->data ?
		if(!strcmp(av[count], opt))
		{
			return TRUE;
		}

		av++;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
//  change directory (cd)
////////////////////////////////////////////////////////////////////////
void cmd_cd(int ac, char *av[])
{
	char *path;

	// setting path if exist argument
	if(ac > 1)
	{
		path = av[1];
	}
	
	// set HOME directory
	else if((path = (char*)getenv("HOME")) == NULL)
	{
		// if doesn't have $HOME path, set path "."
		path = ".";
	}

	// change directory
	if(chdir(path) == ERROR)
	{
		fprintf(stderr, "%s: bad directory.\n", path);
	}
}

////////////////////////////////////////////////////////////////////////
//  exit program
////////////////////////////////////////////////////////////////////////
void cmd_exit()
{
	printf("\nbye\n");
	exit(1);
}

////////////////////////////////////////////////////////////////////////
//  print directory list
////////////////////////////////////////////////////////////////////////
void cmd_ls(int ac, char *av[])
{
	DIR *dp;
	struct dirent *entry;
	char *path;
	int count;
	int opt_a;
	int opt_l;

	// doesn't have enough argument
	if(ac < 2)
	{
		path = ".";
	}
	
	// exist
	else
	{
		path = av[1];
	}

	// open directory
	if((dp = opendir(path)) == NULL)
	{
		fprintf(stderr, "Can't open directory: %s", av[1]);
		return;
	}

	// set option
	opt_a = check_arg(av, "-a");
	opt_l = check_arg(av, "-l");

	count = 0;

	// read file or directory
	while((entry = readdir(dp)) != NULL)
	{
		// doesn't have -a option
		if(!opt_a)
		{
			if(entry->d_name[0] == '.')
			{
				continue;
			}
		}
		
		// print
		printf("%s\t", entry->d_name);

		// -l option
		if(opt_l)
		{
			printf("\n");
		}
		
		// print 3 words in 1 word
		else
		{
			if(count > 3)
			{
				printf("\n");
				count = 0;
			}
			else
			{
				count++;
			}
		}
	}

	// close directory
	closedir(dp);
	printf("\n");
}

////////////////////////////////////////////////////////////////////////
//  file copy function
////////////////////////////////////////////////////////////////////////
void cmd_cp(int ac, char *av[])
{
	FILE *src;
	FILE *dst;
	char ch;

	// doesn't have enough argument
	if(ac < 3)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

	// open file(read mode)
	if((src = fopen(av[1], "r")) == NULL)
	{
		fprintf(stderr, "%s: Can't open file.\n", av[1]);
		return;
	}

	// open file(write mode)
	if((dst = fopen(av[2], "w")) == NULL)
	{
		fprintf(stderr, "%s: Can't open file.\n", av[2]);
		return;
	}

	// copy new file
	while(!feof(src))
	{
		ch = (char) fgetc(src);

		if(ch != EOF)
		{
			fputc((int)ch, dst);
		}
	}

	// check -v option
	if(check_arg(av, "-v"))
	{
		printf("cp %s %s\n", av[1], av[2]);
	}

	fclose(src);
	fclose(dst);
}

////////////////////////////////////////////////////////////////////////
//  delete file
////////////////////////////////////////////////////////////////////////
void cmd_rm(int ac, char *av[])
{
	// doesn't have enough argument
	if(ac < 2)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

	// release link
	unlink(av[1]);

	// check -v option
	if(check_arg(av, "-v"))
	{
		printf("rm %s\n", av[1]);
	}
}

////////////////////////////////////////////////////////////////////////
//  move file
////////////////////////////////////////////////////////////////////////
void cmd_mv(int ac, char *av[])
{
	int src;
	int dst;
	char ch;
	char buff[1024];
	char buf[12];
	ssize_t rcnt;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	getcwd(buff, 1024);

	// doesn't have enough argument

	if(ac == 3){

		// open read file
		if((src = open(av[1], O_RDONLY)) == -1)
		{
			fprintf(stderr, "%s: Can't open file.\n", av[1]);
			return;
		}
	
		chdir(av[2]);

		// create write file
		if((dst = creat(av[1], mode)) == -1)
		{
			fprintf(stderr, "%s: Can't open file.\n", av[2]);
			return;
		}
	
		// move
		while( (rcnt=read(src, buf, 10) ) > 0)
		{
			write(dst, buf, rcnt);
		}

		close(src);
		close(dst);

	
		chdir(buff);

		// release link
		unlink(av[1]);

		// check -v option
		if(check_arg(av, "-v"))
		{
			printf("mv %s %s\n", av[1], av[2]);
		}
	}
	else{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

}

////////////////////////////////////////////////////////////////////////
//  create new directory
////////////////////////////////////////////////////////////////////////
void cmd_mkdir(int ac, char *av[])
{
	// doesn't have enough argument
	if(ac < 2)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

	// create directory
	if(mkdir(av[1], 0755))
	{
		fprintf(stderr, "Make directory failed.\n");
	}
}

////////////////////////////////////////////////////////////////////////
//  delete directory
////////////////////////////////////////////////////////////////////////
void cmd_rmdir(int ac, char *av[])
{
	// doesn't have enough argument
	if(ac < 2)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

	// delete directory
	if(rmdir(av[1]))
	{
		fprintf(stderr, "Remove directory failed.\n");
	}
}

////////////////////////////////////////////////////////////////////////
//  print file contents
////////////////////////////////////////////////////////////////////////
void cmd_cat(int ac, char *av[])
{
	int ch;
	FILE *fp;

	// open read file
	if((fp = fopen(av[1], "r")) == NULL)
	{
		return;
	}

	// print contents
	while((ch = getc(fp)) != EOF)
	{
		putchar(ch);
	}

	fclose(fp);
}
////////////////////////////////////////////////////////////////////////
//  create link
////////////////////////////////////////////////////////////////////////
void cmd_ln(int ac, char *av[]){
	int ch;

	// doesn't have enough argument
	if(ac < 3)
	{
		fprintf(stderr, "Not enough arguments");
		return;
	}

	// create link
	if (link( av[1], av[2]) == -1){
		fprintf(stderr, "create link fail.\n");
		return;
	}

}


void cmd_wc(int ac, char *av[]) {
	
	FILE *fp;
	char ch, prev_ch = ' ';
	int line_count, word_count, char_count;
	line_count = 0;
	word_count = 0;
	char_count = 1;
	
	if((fp = fopen(av[1], "r")) == NULL)
	{
		fprintf(stderr, "%s: Can't open file.\n", av[1]);
		return;
	}	

	while( (ch = fgetc(fp)) != EOF ) {
		if( ch=='\n' || line_count==0 ) line_count++;

		if( (prev_ch=='\n' || prev_ch=='\t' || prev_ch==' ') && !(ch=='\n' || ch=='\t' || ch==' ') )
		word_count++;

		char_count++;

		prev_ch = ch;
	}

	printf("l:%d\t", line_count);
	printf("w:%d\t", word_count);
	printf("c:%d\t", char_count);
	printf("%s\n", av[1]);
}

