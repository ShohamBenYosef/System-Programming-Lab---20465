#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "error.h"
#include "parser.h"
#include "list_handling.h"

struct {
	char* name;
	int numOfOperands;
	
} cmds[] = {
	{"mov", 2},
	{"cmp", 2},
	{"add", 2},
	{"sub", 2},
	{"not", 1},
	{"clr", 1},
	{"lea", 2},
	{"inc", 1},
	{"dec", 1},
	{"jmp", 1},
	{"bne", 1},
	{"get", 1},
	{"prn", 1},
	{"jsr", 1},
	{"rts", 0},
	{"hlt", 0},

};

char** registers = {"r0","r1","r2","r3","r4","r5","r6","r7"};


/* global variables */
int ic = 100;
int dc = 0;

int label_flag = 0;


void forward(char* line ,int *ptr_curr)
{
	int curr = *ptr_curr;
    	while( isspace(line[curr]) && line[curr] != '\n')
		curr++;
	*ptr_curr = curr;
}


int check_comma(char* line, int *ptr_curr)
{
    int curr = *ptr_curr, comma = 0;;
    forward(line, &curr);
    
    if(line[curr] == ',')
	    comma = 1;
    return comma;
}


void jump_comma(char* line, int *ptr_curr)
{
    	int curr = *ptr_curr;
    	forward(line, &curr);
	if(line[curr] == ',')
		curr++;
	
	forward(line, &curr);
	*ptr_curr = curr;
}


void get_next_word(char* line, char* word, int* ptr_curr)
{
	
	int curr = *ptr_curr, j; 
    	forward(line, &curr);

	for(j = 0; !isspace(line[curr]) && line[curr] != '\n' && line[curr] != ','; curr++)
		word[j++] = line[curr];
	word[j] = '\0';
	
	forward(line, &curr);
	*ptr_curr = curr; 
}



void parse_line(char* line, int l_cnt, label_list* symbols)
{
	int curr = 0, i;
	char* first_word,* sec_word;
	first_word = (char*)malloc(sizeof(char)* LINE_LEN);
	sec_word = (char*)malloc(sizeof(char)* LINE_LEN);
	if(!first_word || !sec_word)
		fatal_error(ErrorMemoryAlloc);
	
	
	if(line[curr] != '\n' && line[curr] != ";") /* comment line */
	{
		get_next_word(line, first_word, &curr);
		if(first_word[strlen(first_word)-1] == ':'){
			
			label_flag = 1;
			
			first_word[strlen(first_word)-1] = '\0';

			/* TODO - another func to handle lable. */
			
			get_next_word(line, sec_word ,&curr);
			if(sec_word[0] == '.') {
				add_label_to_list(symbols, first_word, ic, 0, 0);
				parse_data(line,&curr);
			}
			else {
				add_label_to_list(symbols, first_word, ic, 1, 0);
				parse_instruction(line, sec_word,&curr, symbols);
			}
		/* TODO - else if to another word  - no command.. */
		} else {
		
			parse_instruction(line, first_word, &curr, symbols);
		}
		free(sec_word);
		free(first_word);
	}
}


parse_data(char* line,int *ptr_curr){
	int curr = *ptr_curr, count = 0,i;
	char* data = (char*)malloc(sizeof(char)* LINE_LEN);
	if(!data)
		fatal_error(ErrorMemoryAlloc);
		
	get_next_word(line, data, &curr);
	for(i = 0; i < strlen(data); i++) {
		if(isalnum(data[i]))
			count++;
	}
	
	ic += count;
	dc = ic;
	*ptr_curr = curr;
	free(data);
}


void parse_instruction(char* line, char* command_name,int* ptr_curr, label_list* symbols){
	int curr = *ptr_curr, i,j;
	
	for(i = 0; i < 16; i++) /*define 16!!*/{ 
		if(!strcmp(command_name, cmds[i].name)){
			break;
		}
	}
	if(i == 16){
		error_log(InvalidCommand, ic);
		return;
	} 
	else {
		for(j = 0; j < cmds[i].numOfOperands; j++){
			int op_kind = parse_operands(line, cmds[i].name , &curr, symbols);
			/*switch*/
		}
		ic += (cmds[i].numOfOperands+1);
	}
	*ptr_curr = curr;
}


int parse_operands(char* line, char* command, int* ptr_curr, label_list* symbols){ /* TODO  -  func to check operands. */
	printf("123\n");
	int i, curr = *ptr_curr;
	char* op = (char*)malloc(sizeof(char)* LINE_LEN);
	if(!op)
		fatal_error(ErrorMemoryAlloc);
		
	get_next_word(line, op, &curr);
	puts(op);
	/* check if register */
	for(i = 0; i <= 8; i++){
		if(!strcmp(op, registers[i])) {
			return 1;		
		}
	}
	/* check if number */
	if(op[0] == '#')
		return 2;
		
	else {/*
		label_list* l = symbols;
		while(l.head != NULL){
			if(!strcmp(l.head.label, op))
				return 3;
			l.head = l.head.next;
		}*/
	}
}
