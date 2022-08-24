#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/*
* Fatal error - report and exit.
*/
void fatal_error(const char* message) {
	fprintf(stderr, "Fatal error: %s\n", message);
	exit(EXIT_FAILURE);
}

