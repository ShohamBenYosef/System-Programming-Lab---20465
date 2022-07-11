#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "error.h"
#include "parser.h"
#include "list_handling.h"
#include "pre_asm.h"


FILE* open_file(const char* file_name, char* ending, char* mode)
{
	FILE* file_p;
	char* full_file_name;
	
	full_file_name = get_full_file_name(file_name, ending);
	
	file_p = fopen(full_file_name, mode);
	free(full_file_name);
	if(!file_p) {
		fprintf(stderr, ErrorCantRead, full_file_name);
		fprintf(stderr, "\n");
		exit(EXIT_FAILURE);
	}
	return file_p;
}



char* get_full_file_name(const char* file_name, char* ending)
{
	char* file_name_base;
	
	file_name_base = (char*)malloc((strlen(file_name) + AdditionalChars));
	if (!file_name_base) {
		fatal_error(ErrorMemoryAlloc);
   	     exit(0);
	}
    
	file_name_base[0] = '\0';

	strcat(file_name_base,file_name);
	strcat(file_name_base,ending);
    
    	file_name_base[strlen(file_name_base)] ='\0';
	puts(file_name_base);
	return file_name_base;
}






void handle_file(const char* file_name, char* mode)
{
	char line[LINE_LEN];
	setParserData();  /* set to default every iteration of the main loop */
	
	parser_data.nameOfFile = (char*)malloc(sizeof(char)*strlen(file_name)+1);
	if(!parser_data.nameOfFile)
		fatal_error(ErrorMemoryAlloc);

	strcpy(parser_data.nameOfFile, file_name);
	pre_assembler();
	
	printf("after pre_asm--> %s\n",parser_data.nameOfFile);/**/
	parser_data.file = open_file(parser_data.nameOfFile, AfterMacroEnding ,ReadFile);
	
	/*until pase 11 */
	while(fgets(line, LINE_LEN, parser_data.file)){
		parser_data.line_number++;
		parse_line(line);
	}
	
	printf("End of first round of the asm.\n\n");
	print_label_table(parser_data.Shead);
	fseek(parser_data.file, 0, SEEK_SET); /* set pointer to the start of file. */
	/*part 2 of first move.*/
	
	while(fgets(line, LINE_LEN,parser_data.file)){
		call_func_to_11(line, 0);/*TODO - change name*/
	}
	
	
	fclose(parser_data.file);
}

