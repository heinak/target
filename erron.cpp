#include<stdlib.h>
#include<stdio.h>
#include"erron.h"



void errif(bool mark, const char*message)
{
	if (mark) {
		perror(message);
		exit(EXIT_FAILURE);
	}
}
