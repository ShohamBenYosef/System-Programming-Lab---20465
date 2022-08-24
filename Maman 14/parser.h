#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include "lists.h"
#define LINE_LEN 81
#define NUM_OF_COMMANDS 16


typedef enum { immediateOperand, SymbolOperand, RealativeOperand, RegisterOperand} operandType;

enum { dataLine, InstructionLine };

typedef struct {
	label_list* Shead; /* Head of symbol table */
	LineData_list* Dhead; /* Head of data table */
	int err_count;	/**/
	char* nameOfFile; /**/
	FILE* file;
	/* Command and data counters. */
	int ent_flag;
	int ext_flag;
	int line_num;
	unsigned int IC;
	unsigned int DC;
} ParserData;

ParserData parser_data;



int is_register(char* op);
int is_direct(char* op);
int is_labelA(char* op);
int is_imidiate (char* op);


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
