#ifndef FILES_H_
#define FILES_H_

#include <stdlib.h>
#include "parser.h"

#define LINE_LEN 81

#define MainFileEnding ".as"
#define AfterMacroEnding ".am"
#define ObjectFileEnding ".ob"
#define ExternFileEnding ".ext"
#define EntryFileEnding ".ent"
#define ReadFile "r"
#define WirtingFile "w"

/*  */
FILE* open_file(const char* file_name, char* ending, char* mode);
/*  */
char* get_full_file_name(const char* file_name, char* ending);
/*  */
void handle_file(const char* file_name, char* mode);
/*  */
void handle_output_file();




#endif
