#ifndef ERROR_H_
#define ERROR_H_


/* 
* List of common errors: 
*/
#define ErrorMaxLengthLine "there is too many characters in this line."
#define ErrorMissingArgument "Missing argument."
#define ErrorCantRead "Could not open file for reading."
#define ErrorMemoryAlloc "Memory allocation failed."
#define ErrorInAssemblyCode "There's error in the code - exit without create files."
#define InvalidCommand "Invalid Command."


/*
* Prints an error message and the program exits
*/
void fatal_error(const char* message);


#endif
