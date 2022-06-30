#include "checker.h"

#define NUM_SUGGESTS 5
#define MAX_LEN_WORD 30

typedef struct _WrongWord {
	char *word;
	char *suggests[NUM_SUGGESTS];
	int num;
} *WrongWord;



