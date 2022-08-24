#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "error.h"
#include "parser.h"
#include "lists.h"
#include "pre_asm.h"
#include "translate.h"

#define EXTERN 4
#define ENTRY 5




/* open file and return ptr */
FILE* open_file(const char* file_name, char* ending, char* mode)
{
	FILE* file_p;
	char* full_file_name;
	
	full_file_name = get_full_file_name(file_name, ending);
	
	file_p = fopen(full_file_name, mode);
	free(full_file_name);
	
	if(!file_p) {
		fatal_error(ErrorCantRead);
	}
	return file_p;
}



/* connect the original file name with the ending */
char* get_full_file_name(const char* file_name, char* ending)
{
	char* file_name_base;
	
	file_name_base = (char*)malloc((strlen(file_name) + AdditionalChars));
	if (!file_name_base)
		fatal_error(ErrorMemoryAlloc);
    
	file_name_base[0] = '\0';

	strcat(file_name_base,file_name);
	strcat(file_name_base,ending);
    
    file_name_base[strlen(file_name_base)] ='\0'; /* setting null */

	return file_name_base;
}





/* Getting a file name and adds the correct ending,
 send it to the pre-assembler and then read it line by line and send for processing.  */
void handle_file(const char* file_name, char* mode)
{
	char line[LINE_LEN];
	setParserData();  /* set to default every iteration of the main loop */
	
	parser_data.nameOfFile = (char*)malloc(sizeof(char)*strlen(file_name)+1);
	if(!parser_data.nameOfFile)
		fatal_error(ErrorMemoryAlloc);

	strcpy(parser_data.nameOfFile, file_name);
	pre_assembler(); /* send it to pre_asm */
	
	parser_data.file = open_file(parser_data.nameOfFile, AfterMacroEnding ,ReadFile); /*open the correct file */
	
	/* read line by line and send the char[] to parser. */
	while(fgets(line, LINE_LEN, parser_data.file)){
		parser_data.line_num++;
		parse_line(line);
	}
	/* TODO - delete */
	print_label_table(parser_data.Shead);
	printf("\n");
	print_debbug_data(parser_data.Dhead);

	fseek(parser_data.file, 0, SEEK_SET); /* set pointer to the start of file. */
	
	while(fgets(line, LINE_LEN,parser_data.file)){
		sec_pas(line, 0);
	}
	
	print_debbug_data(parser_data.Dhead); /* TODO - delete */
	handle_output_file(); 
	
	fclose(parser_data.file);
}




/* open the correct files and print that (in 32base) */
void handle_output_file(){
	/* extern file */
	if(parser_data.ext_flag > 0){
		print_ext_file(parser_data.Shead, ExternFileEnding, EXTERN);
	}
	/* entry file */
	if (parser_data.ent_flag > 0){
		print_ext_file(parser_data.Shead, EntryFileEnding, ENTRY);
	}
	/* object file */
	print_data_to_file(parser_data.nameOfFile, ObjectFileEnding);
	
}

