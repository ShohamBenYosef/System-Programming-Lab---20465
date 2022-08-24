#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "files.h"
#include "lists.h"
#define ReadFile "r"


/* 
* MAIN FUNC OF ASM
 */
int main(int argc, char* argv[])
{
	int i = 1;
	if(argc >= 2)
		for(i = 1; i < argc; i++){
			handle_file(argv[i], ReadFile);
		}
	else
		fatal_error(ErrorMissingArgument);
		
	return 0;
}


