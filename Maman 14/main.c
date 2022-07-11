#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "files.h"

#define ReadFile "r"

int main(int argc, char* argv[])
{
	int i = 1;
	if(argc >= 2)
		for(i = 1; i < argc; i++){
			printf("name of file %s\n", argv[i]); /* TO_DEL*/
			handle_file(argv[i], ReadFile);
	}
	else
		fatal_error(ErrorMissingArgument);
	
	return 0;
}


