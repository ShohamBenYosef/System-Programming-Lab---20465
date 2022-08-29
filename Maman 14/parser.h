#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include "lists.h"


#define LINE_LEN 81
#define NUM_OF_COMMANDS 16



typedef enum { immediateOperand, SymbolOperand, RealativeOperand, RegisterOperand} operandType;

enum { dataLine, InstructionLine };


typedef struct {
	label_list* Shead;	/* Head of symbol table. */
	LineData_list* Dhead;	/* Head of data table. */
	int err_count;		/* count the error in asembly file. */
	char* nameOfFile; 	/* name of file that we handle now. */
	FILE* file;		/* ptr to current file. */
	
	/* Command and data counters. */
	int ent_flag;		/* save if there is entry label in the asm file */
	int ext_flag;		/* save if there is extern label in the asm file */
	int line_num;		/* current line number*/
	unsigned int IC;	/* Instruction count */
	unsigned int DC;	/* Data count */
} ParserData;


ParserData parser_data; /* var */


/* func for operands */
int is_register(char* op);
int is_direct(char* op);
int is_labelA(char* op);
int is_imidiate (char* op);

/* func for start and end asm on file */
void setParserData();
void freeParserData();


parse_operand(char* line, int commandIndex, int* ptr, int opNumber);
int check_num_of_operands(char* line, int command_num, int* ptr);
handle_instruction(char* line,char* command, int* ptr);
int parse_command(char* line,char* command, int* ptr_curr);
parse_extern(char* line, int* ptr);


int get_next_number(char* line, int* ptr_curr);
void handle_string(char* line, char* str, int *ptr);
void handle_data(char* line, char* str,  int *ptr);
void handle_struct(char* line, char* str,  int *ptr);
void parse_data(char* line, char* sec, int *ptr);


void parse_line( char* line);
void sec_pas(char* line, int l_cnt);
void get_next_word(char* line, char* word, int* ptr_curr);
void jump_comma(char* line, int *ptr_curr);
int check_comma(char* line, int *ptr_curr);
void forward(char* line ,int *ptr_curr);


#endif
