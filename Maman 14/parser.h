#ifndef PARSER_H_
#define PARSER_H_

#include "list_handling.h"
#define LINE_LEN 81

typedef enum {
	immediateOperand,
	SymbolOperand,
	RealativeOperand,
	RegisterOperand
} operandType;





/*
* Jump over white characters and lines.
*/
void forward(char* line ,int *ptr_curr);

/*
* extruct the next word from the line.
*/
void get_next_word(char* line, char* word, int* ptr_curr);

/*
* check if there is a comma .
* the func won't move the 'curr' pointer.
* return int as boolean flag.
*/
int check_comma(char* line, int *ptr_curr);

/*
* jump over comma and white chars.
* should came after 'check comma' method.
*/
void jump_comma(char* line, int *ptr_curr);

/*
* first parse of the assembler.
* check ic and dc counters, add symbol to table and looking for bugs in the inputs.
*/
void parse_line(char* line, int l_cnt, label_list* symbols);

/*
* count dc and ic for label line with .data or .string.
*/
void parse_instruction(char* line, char* command_name,int* ptr_curr, label_list* symbols);

/*
* count ic and check valid command and operands.
*/
parse_data(char* line, int* curr);
int parse_operands(char* line, char* command, int* ptr_curr, label_list* symbols);

#endif
