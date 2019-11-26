#ifndef __MINISH_H__
#define __MINISH_H__

#define ERROR		(-1)
#define BADFD		(-2)

#define MAXFNAME	10
#define MAXARG		10
#define MAXWORD		20
#define MAXFD		20
#define MAXVAR		50
#define MAXNAME		20

#define TRUE		1
#define FALSE		0

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

typedef int BOOLEAN;

// decide command type
// "", |, &, ;, >, >>, <, \n, EOF
typedef enum
{
	S_WORD,
	S_BAR,
	S_AMP,
	S_SEMI,
	S_GT,
	S_GTGT,
	S_LT,
	S_NL,
	S_EOF
} SYMBOL;

#endif
